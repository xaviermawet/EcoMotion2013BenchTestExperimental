#include "DoubleYAxisPlot.hpp"

DoubleYAxisPlot::DoubleYAxisPlot(const QString& title, QWidget* parent) :
    DoubleYAxisPlot(QwtText(title), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

DoubleYAxisPlot::DoubleYAxisPlot(const QwtText& title, QWidget* parent) :
    Plot(title, parent), _yRightZoomer(NULL), _rescaler(NULL)
{
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
    this->_rescaler->setReferenceAxis(QwtPlot::yLeft);
    this->_rescaler->setAspectRatio(QwtPlot::yRight, 0.01);
    this->_rescaler->setAspectRatio(QwtPlot::xBottom, 0.0);
    this->_rescaler->setAspectRatio(QwtPlot::xTop, 0.0);
    this->setAxisScale(QwtPlot::yLeft, 0, 10);
}

DoubleYAxisPlot::~DoubleYAxisPlot(void)
{
    qDebug() << "DoubleYAxisPlot Début destructeur";

    delete this->_yRightZoomer;
    delete this->_rescaler;

    qDebug() << "DoubleYAxisPlot fin destructeur";
}

void DoubleYAxisPlot::zoom(const QwtPlotItem* item)
{
    // Vérifier en fonction de quel axe y les coordonnées de l'élément sont exprimées
    if (item->yAxis() == QwtPlot::yRight)
    {
        qDebug() << "On zoom sur l'axe de droite ...";
    }
    else
    {
        qDebug() << "On zoome sur l'axe de gauche ...";
    }
}
