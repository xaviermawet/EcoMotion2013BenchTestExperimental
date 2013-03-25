#include "Plot.hpp"

Plot::Plot(const QString &title, QWidget *parent) :
    Plot(QwtText(title), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

Plot::Plot(const QwtText &title, QWidget *parent) :
    QwtPlot(title, parent), legend(NULL), zoomer(NULL)
{
    /* ---------------------------------------------------------------------- *
     *                         Add a legend for curves                        *
     * ---------------------------------------------------------------------- */
    this->legend = new QwtLegend(this);
    this->legend->setItemMode(QwtLegend::CheckableItem);

    // Insert the legend into the plot
    this->insertLegend(this->legend, QwtPlot::BottomLegend);

    /* ---------------------------------------------------------------------- *
     *                         Manage zoom for the plot                       *
     * ---------------------------------------------------------------------- *
     * LeftButton for the zooming                                             *
     * RightButton: zoom out by 1                                             *
     * Ctrl+RighButton: zoom out to full size                                 *
     * ---------------------------------------------------------------------- */
    this->zoomer = new QwtPlotZoomer(this->canvas());

    // Display coordinates at mouse position every time
    this->zoomer->setTrackerMode(QwtPicker::AlwaysOn);

    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
                                  Qt::RightButton, Qt::ControlModifier);
    this->zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
                                  Qt::RightButton );

    /* ---------------------------------------------------------------------- *
     *                      Some customization options                        *
     * ---------------------------------------------------------------------- */
    this->enableAxis(QwtPlot::yRight);
    this->setCanvasBackground(Qt::darkGray);
    this->setAxisTitle(QwtPlot::xBottom, tr("Temps (s)"));
}

Plot::~Plot(void)
{
    qDebug() << "Plot début destructeur";

    delete this->legend;
    delete this->zoomer;

    qDebug() << "Plot fin destructeur";
}
