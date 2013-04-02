#include "PlotCurve.hpp"

PlotCurve::PlotCurve(const QString &title, const QPen &pen) :
    PlotCurve(QwtText(title), pen)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

PlotCurve::PlotCurve(const QwtText &title, const QPen &pen) :
    QwtPlotCurve(title)
{
    this->setPen(pen);
    this->setRenderHint(QwtPlotItem::RenderAntialiased);
    this->setItemAttribute(QwtPlotItem::Legend, true); // true default value
    this->setLegendAttribute(QwtPlotCurve::LegendShowLine);
}
