#include "Plot.hpp"

Plot::Plot(const QString &title, QWidget *parent) :
    Plot(QwtText(title), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

Plot::Plot(const QwtText &title, QWidget *parent) :
    QwtPlot(title, parent), legend(NULL), grid(NULL), crossLine(NULL),
    panner(NULL), yRightZoomer(NULL), yLeftZoomer(NULL), magnifier(NULL)
{
    this->setAutoReplot(false);

    /* ---------------------------------------------------------------------- *
     *                         Add a legend for curves                        *
     * ---------------------------------------------------------------------- */
    this->legend = new QwtLegend(this);
    this->legend->setItemMode(QwtLegend::CheckableItem);
    this->legend->setContextMenuPolicy(Qt::CustomContextMenu);
    this->insertLegend(this->legend, QwtPlot::BottomLegend);

    connect(this->legend, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showLegendContextMenu(QPoint)));

    /* ---------------------------------------------------------------------- *
     *                                Add a grid                              *
     * ---------------------------------------------------------------------- */
    this->grid = new QwtPlotGrid();
    this->grid->setTitle(tr("Grille"));
    this->grid->setMajPen(QPen(Qt::gray, Qt::SolidLine));
    this->grid->attach(this);

    /* ---------------------------------------------------------------------- *
     *                            Add a cross marker                          *
     * ---------------------------------------------------------------------- */
    this->crossLine = new QwtPlotMarker();
    this->crossLine->setLineStyle(QwtPlotMarker::Cross);
    this->crossLine->setValue(0, 0);
    //this->crossLine->attach(this); // Detached by default

    /* ---------------------------------------------------------------------- *
     *                      Manage panning for the plot                       *
     * ---------------------------------------------------------------------- *
     * MidButton for the panning                                              *
     * ---------------------------------------------------------------------- */
    this->panner = new QwtPlotPanner(this->canvas());
    this->panner->setMouseButton(Qt::MidButton);

    /* ---------------------------------------------------------------------- *
     *                         Manage zoom for the plot                       *
     * ---------------------------------------------------------------------- *
     * Left button               : dragging rect                              *
     * Right button              : zoom out by 1                              *
     * Ctrl + Righ button        : zoom out to full size                      *
     * Mouse wheel or Ctrl + +/- : zoom in/out by 1                           *
     * ---------------------------------------------------------------------- */
    this->yLeftZoomer = new Zoomer(this->canvas());

    // Display coordinates at mouse position every time
    this->yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOn);

    // Allow to zoom with left button by dragging a rect
    this->yLeftZoomer->setRubberBand(QwtPicker::RectRubberBand);


    // Manage zoom with the mouse wheel and keyborad
    this->magnifier = new QwtPlotMagnifier(this->canvas());
    this->magnifier->setMouseFactor(magnifier->keyFactor() / 2);
    this->magnifier->setZoomOutKey(Qt::Key_Minus, Qt::ControlModifier);
    this->magnifier->setZoomInKey(Qt::Key_Plus,
                                  Qt::ControlModifier | Qt::ShiftModifier);

    /* ---------------------------------------------------------------------- *
     *                        Add a y axis on the right                       *
     * ---------------------------------------------------------------------- *
     * A zoomer controls only one x and one y axis. If you want to zoom 2 y   *
     * axis you need a second zoomer (without tracker and rubber band)        *
     * ---------------------------------------------------------------------- */
    this->enableAxis(QwtPlot::yRight);

    // Zoomer for the new axis
    this->yRightZoomer = new Zoomer(xTop, yRight, this->canvas());

    /* ---------------------------------------------------------------------- *
     *       Rescaler takes care of fixed aspect ratios for plot scales       *
     * ---------------------------------------------------------------------- */
    this->rescaler = new QwtPlotRescaler(this->canvas());
    this->rescaler->setReferenceAxis(Plot::yLeft);
    this->rescaler->setAspectRatio(Plot::yRight, 0.01);
    this->rescaler->setAspectRatio(Plot::xBottom, 0.0);
    this->rescaler->setAspectRatio(Plot::xTop, 0.0);
    this->setAxisScale(Plot::yLeft, 0, 10);

    /* ---------------------------------------------------------------------- *
     *                      Some customization options                        *
     * ---------------------------------------------------------------------- */
    this->setCanvasBackground(Qt::darkGray);

    this->setAutoReplot(true);
}

Plot::~Plot(void)
{
    qDebug() << "Plot Début destructeur";

    delete this->legend;
    delete this->grid;
    delete this->crossLine;
    delete this->panner;
    delete this->yRightZoomer;
    delete this->yLeftZoomer;
    delete this->magnifier;

    qDebug() << "Plot fin destructeur";
}

bool Plot::isGridVisible(void) const
{
    return this->grid->plot() != NULL;
}

bool Plot::isCrossLineVisible(void) const
{
    return this->crossLine->plot() != NULL;
}

bool Plot::isLabelPositionVisible(void) const
{
    return this->yLeftZoomer->trackerMode() == QwtPlotPicker::AlwaysOn;
}

void Plot::setGridVisible(bool visible)
{
    visible ? this->grid->attach(this) : this->grid->detach();
    this->replot();
}

void Plot::setCrossLineVisible(bool visible)
{
    if (visible)
    {
        this->crossLine->attach(this);
        connect(this->yLeftZoomer, SIGNAL(mousePosChanged(QPointF)),
                this, SLOT(updateCrossLinePosition(QPointF)));
    }
    else
    {
        this->crossLine->detach();
        disconnect(this->yLeftZoomer, SIGNAL(mousePosChanged(QPointF)),
                   this, SLOT(updateCrossLinePosition(QPointF)));
    }

    this->replot();
}

void Plot::setLabelPositionVisible(bool visible)
{
    visible ? this->yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOn) :
              this->yLeftZoomer->setTrackerMode(QwtPicker::AlwaysOff);

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

void Plot::updateCrossLinePosition(const QPointF& pos)
{
    this->crossLine->setValue(pos);
}

void Plot::showLegendContextMenu(const QPoint& pos)
{
    // Stop if the user doesn't right clic on a legend item
    QwtLegendItem* legendItem = qobject_cast<QwtLegendItem*>(
                this->legend->childAt(pos));
    if (legendItem == NULL)
        return;

    foreach(QwtPlotItem* plotItem, this->itemList())
    {
        if (plotItem->title().text() == legendItem->text().text())
        {
            emit this->legendRightClicked(plotItem,
                                          this->legend->mapToGlobal(pos));
            break;
        }
    }
}

//void Plot::centerOnCurve(void)
//{
//    if (this->curveAssociatedToLegendItem == NULL)
//        return;

//    this->yLeftZoomer->zoom(this->curveAssociatedToLegendItem->boundingRect());

//    // FIXME : voir si les deux echelles s'adaptent correctement !!!

//    /* FIXME :
//     * --------------
//     * Attention que si il y a deux axes, avant de se centrer sur une des
//     * courbes, il faut regarder si la courbe a ses coordonnées en
//     * fonction de l'axe y de droite ou de gauche !!!!
//     */
//}

void Plot::adaptYRightAxis(const QRectF &rect)
{
    QRectF rescaledRect(rect);
    rescaledRect.setTop(rect.top() * 100); // FIXME : le rapport (ici 100) doit etre une variable membre
    rescaledRect.setBottom(rect.bottom() * 100); // FIXME : le rapport (ici 100) doit etre une variable membre

    this->yRightZoomer->zoom(rescaledRect);
}
