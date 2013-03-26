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
    /* ---------------------------------------------------------------------- *
     *                         Add a legend for curves                        *
     * ---------------------------------------------------------------------- */
    this->legend = new QwtLegend(this);
    this->legend->setItemMode(QwtLegend::CheckableItem);
    this->insertLegend(this->legend, QwtPlot::BottomLegend);

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
     *                      Some customization options                        *
     * ---------------------------------------------------------------------- */
    this->setCanvasBackground(Qt::darkGray);
    this->setAxisTitle(QwtPlot::xBottom, tr("Temps (s)"));
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
}

void Plot::updateCrossLinePosition(const QPointF& pos)
{
    this->crossLine->setValue(pos);
}
