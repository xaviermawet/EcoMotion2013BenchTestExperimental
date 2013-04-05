#ifndef __DOUBLEYAXISPLOT_HPP__
#define __DOUBLEYAXISPLOT_HPP__

#include "Plot.hpp"
#include "Zoomer.hpp"
#include <qwt_plot_rescaler.h>

class DoubleYAxisPlot : public Plot
{
    Q_OBJECT

    public:

        explicit DoubleYAxisPlot(QString const& title, QWidget* parent = NULL);
        explicit DoubleYAxisPlot(QwtText const& title, QWidget* parent = NULL);
        virtual ~DoubleYAxisPlot(void);

        virtual void zoom(QwtPlotItem const* item);

    protected:

        Zoomer*          _yRightZoomer;
        QwtPlotRescaler* _rescaler;
};

#endif /* __DOUBLEYAXISPLOT_HPP__ */
