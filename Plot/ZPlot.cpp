#include "ZPlot.hpp"

ZPlot::ZPlot(const QString &title, QWidget *parent) :
    ZPlot(QwtText(title), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

ZPlot::ZPlot(const QwtText &title, QWidget *parent) :
    QwtPlot(title, parent), _legend(NULL), _grid(NULL), _crossLine(NULL),
    _panner(NULL), _yRightZoomer(NULL), _yLeftZoomer(NULL), _magnifier(NULL)
{
    this->setAutoReplot(false);

    /* ---------------------------------------------------------------------- *
     *                         Add a legend for curves                        *
     * ---------------------------------------------------------------------- */
    this->_legend = new QwtLegend(this);
    this->_legend->setItemMode(QwtLegend::CheckableItem);
    this->_legend->setContextMenuPolicy(Qt::CustomContextMenu);
    this->insertLegend(this->_legend, QwtPlot::BottomLegend);

    connect(this->_legend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showLegendContextMenu(QPoint)));

    /* ---------------------------------------------------------------------- *
     *                                Add a grid                              *
     * ---------------------------------------------------------------------- */
    this->_grid = new QwtPlotGrid();
    this->_grid->setTitle(tr("Grille"));
    this->_grid->setMajPen(QPen(Qt::gray, Qt::SolidLine));
    this->_grid->attach(this);

    /* ---------------------------------------------------------------------- *
     *                            Add a cross marker                          *
     * ---------------------------------------------------------------------- */
    this->_crossLine = new QwtPlotMarker();
    this->_crossLine->setLineStyle(QwtPlotMarker::Cross);
    this->_crossLine->setValue(0, 0);
    //this->crossLine->attach(this); // Detached by default

    /* ---------------------------------------------------------------------- *
     *                      Manage panning for the plot                       *
     * ---------------------------------------------------------------------- *
     * MidButton for the panning                                              *
     * ---------------------------------------------------------------------- */
    this->_panner = new QwtPlotPanner(this->canvas());
    this->_panner->setMouseButton(Qt::MidButton);

    /* ---------------------------------------------------------------------- *
     *                         Manage zoom for the plot                       *
     * ---------------------------------------------------------------------- *
     * Left button               : dragging rect                              *
     * Right button              : zoom out by 1                              *
     * Ctrl + Righ button        : zoom out to full size                      *
     * Mouse wheel or Ctrl + +/- : zoom in/out by 1                           *
     * ---------------------------------------------------------------------- */
    this->_yLeftZoomer = new Zoomer(this->canvas());

    // Display coordinates at mouse position every time
    this->_yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOn);

    // Allow to zoom with left button by dragging a rect
    this->_yLeftZoomer->setRubberBand(QwtPicker::RectRubberBand);


    // Manage zoom with the mouse wheel and keyborad
    this->_magnifier = new QwtPlotMagnifier(this->canvas());
    this->_magnifier->setMouseFactor(_magnifier->keyFactor() / 2);
    this->_magnifier->setZoomOutKey(Qt::Key_Minus, Qt::ControlModifier);
    this->_magnifier->setZoomInKey(Qt::Key_Plus,
                                  Qt::ControlModifier | Qt::ShiftModifier);

    /* ---------------------------------------------------------------------- *
     *                        Add a y axis on the right                       *
     * ---------------------------------------------------------------------- *
     * A zoomer controls only one x and one y axis. If you want to zoom 2 y   *
     * axis you need a second zoomer (without tracker and rubber band)        *
     * ---------------------------------------------------------------------- */
    this->enableAxis(QwtPlot::yRight);

    // Zoomer for the new axis
    this->_yRightZoomer = new Zoomer(xTop, yRight, this->canvas());

    /* ---------------------------------------------------------------------- *
     *       Rescaler takes care of fixed aspect ratios for plot scales       *
     * ---------------------------------------------------------------------- */
    this->_rescaler = new QwtPlotRescaler(this->canvas());
    this->_rescaler->setReferenceAxis(ZPlot::yLeft);
    this->_rescaler->setAspectRatio(ZPlot::yRight, 0.01);
    this->_rescaler->setAspectRatio(ZPlot::xBottom, 0.0);
    this->_rescaler->setAspectRatio(ZPlot::xTop, 0.0);
    this->setAxisScale(ZPlot::yLeft, 0, 10);

    /* ---------------------------------------------------------------------- *
     *                      Some customization options                        *
     * ---------------------------------------------------------------------- */
    this->setCanvasBackground(Qt::darkGray);

    this->setAutoReplot(true);
}

ZPlot::~ZPlot(void)
{
    qDebug() << "Plot Début destructeur";

    delete this->_legend;
    delete this->_grid;
    delete this->_crossLine;
    delete this->_panner;
    delete this->_yRightZoomer;
    delete this->_yLeftZoomer;
    delete this->_magnifier;

    qDebug() << "Plot fin destructeur";
}

bool ZPlot::isGridVisible(void) const
{
    return this->_grid->plot() != NULL;
}

bool ZPlot::isCrossLineVisible(void) const
{
    return this->_crossLine->plot() != NULL;
}

bool ZPlot::isLabelPositionVisible(void) const
{
    return this->_yLeftZoomer->trackerMode() == QwtPlotPicker::AlwaysOn;
}

void ZPlot::setGridVisible(bool visible)
{
    visible ? this->_grid->attach(this) : this->_grid->detach();
    this->replot();
}

void ZPlot::setCrossLineVisible(bool visible)
{
    if (visible)
    {
        this->_crossLine->attach(this);
        connect(this->_yLeftZoomer, SIGNAL(mousePosChanged(QPointF)),
                this, SLOT(updateCrossLinePosition(QPointF)));
    }
    else
    {
        this->_crossLine->detach();
        disconnect(this->_yLeftZoomer, SIGNAL(mousePosChanged(QPointF)),
                   this, SLOT(updateCrossLinePosition(QPointF)));
    }

    this->replot();
}

void ZPlot::setLabelPositionVisible(bool visible)
{
    visible ? this->_yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOn) :
              this->_yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOff);

    this->replot();

    /* FIXME
     * Attention que si j'ajoute la posibilité de choisir si le label
     * au curseur affiche la valeur en y pour l'axe de gauche ou l'axe
     * de droite, je dois savoir (variable membre de type int qui vaudra YRight
     * ou YLeft ou bool showLeftYValue) quel est l'axe courant
     * choisi pour lequel on doit afficher la position
     *
     * exemple de code :
     *
     * if (this->yRightZoomer != NULL)
     * OU
     * if (this->axisEnable(yRight)
     *
     * alors dans ces cas là, vérifier pour quel y on doit afficher dans le label
     */
}

void ZPlot::updateCrossLinePosition(const QPointF& pos)
{
    this->_crossLine->setValue(pos);
}

void ZPlot::showLegendContextMenu(const QPoint& pos)
{
    // Stop if the user doesn't right clic on a legend item
    QwtLegendItem* legendItem = qobject_cast<QwtLegendItem*>(
                this->_legend->childAt(pos));
    if (legendItem == NULL)
        return;

    foreach(QwtPlotItem* plotItem, this->itemList())
    {
        if (plotItem->title().text() == legendItem->text().text())
        {
            emit this->legendRightClicked(
                        plotItem, this->_legend->mapToGlobal(pos));
            break;
        }
    }
}

void ZPlot::adaptYRightAxis(const QRectF &rect)
{
    QRectF rescaledRect(rect);
    rescaledRect.setTop(rect.top() * 100); // FIXME : le rapport (ici 100) doit etre une variable membre
    rescaledRect.setBottom(rect.bottom() * 100); // FIXME : le rapport (ici 100) doit etre une variable membre

    this->_yRightZoomer->zoom(rescaledRect);
}
