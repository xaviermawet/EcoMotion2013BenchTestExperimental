#ifndef __PLOTCURVE_HPP__
#define __PLOTCURVE_HPP__

#include <qwt_plot_curve.h>

class PlotCurve : public QwtPlotCurve
{
    public:

        explicit PlotCurve(QString const& title, QPen const& pen);
        explicit PlotCurve(QwtText const& title, QPen const& pen);
};

#endif /* __PLOTCURVE_HPP__ */
