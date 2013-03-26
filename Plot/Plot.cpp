#include "Plot.hpp"

Plot::Plot(const QString &title, QWidget *parent) :
    Plot(QwtText(title), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

Plot::Plot(const QwtText &title, QWidget *parent) :
    QwtPlot(title, parent), legend(NULL), grid(NULL), crossLine(NULL),
    panner(NULL), zoomer(NULL), magnifier(NULL)
{
    /* ---------------------------------------------------------------------- *
     *                         Add a legend for curves                        *
     * ---------------------------------------------------------------------- */
    this->legend = new QwtLegend(this);
    this->legend->setItemMode(QwtLegend::CheckableItem);

    // Insert the legend into the plot
    this->insertLegend(this->legend, QwtPlot::BottomLegend);

    /* ---------------------------------------------------------------------- *
     *                             Ajout d'une grille                         *
     * ---------------------------------------------------------------------- */
    this->grid = new QwtPlotGrid();
    this->grid->setTitle(tr("Grille"));
    this->grid->setMajPen(QPen(Qt::gray, Qt::SolidLine));
    this->grid->attach(this);

    /* ---------------------------------------------------------------------- *
     *                            Ajout d'une croix                           *
     * ---------------------------------------------------------------------- */
    this->crossLine = new QwtPlotMarker();
    this->crossLine->setLineStyle(QwtPlotMarker::Cross);
    this->crossLine->setValue(0, 0);
    //this->crossLine->attach(this);

    /* ---------------------------------------------------------------------- *
     *                      Manage panning for the plot                       *
     * ---------------------------------------------------------------------- *
     * MidButton for the panning                                              *
     * ---------------------------------------------------------------------- */
    this->panner = new QwtPlotPanner(this->canvas());
    this->panner->setAxisEnabled( QwtPlot::yRight, false );
    this->panner->setMouseButton(Qt::MidButton);

    /* ---------------------------------------------------------------------- *
     *                         Manage zoom for the plot                       *
     * ---------------------------------------------------------------------- *
     * LeftButton for the zooming                                             *
     * RightButton: zoom out by 1                                             *
     * Ctrl+RighButton: zoom out to full size                                 *
     * Mouse wheel or Ctrl + +/- : zoom in/out by 1                           *
     * ---------------------------------------------------------------------- */
    this->zoomer = new QwtPlotZoomer(this->canvas());

    // Display coordinates at mouse position every time
    this->zoomer->setTrackerMode(QwtPicker::AlwaysOn);

    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                                  Qt::RightButton, Qt::ControlModifier);
    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
                                  Qt::RightButton );

    // Manage zoom with the mouse wheel and keyborad
    this->magnifier = new QwtPlotMagnifier(this->canvas());
    this->magnifier->setMouseFactor(magnifier->keyFactor() / 2);
    this->magnifier->setZoomOutKey(Qt::Key_Minus, Qt::ControlModifier);
    this->magnifier->setZoomInKey(Qt::Key_Plus,
                                  Qt::ControlModifier | Qt::ShiftModifier);

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
    delete this->zoomer;
    delete this->panner;
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

void Plot::setGridVisible(bool visible)
{
    visible ? this->grid->attach(this) : this->grid->detach();
    this->replot();

    //this->grid->setVisible(visible);
}

void Plot::setCrossLineVisible(bool visible)
{
    visible ? this->crossLine->attach(this) : this->crossLine->detach();
    this->replot();

    //this->crossLine->setVisible(visible);
}
