#ifndef __ZOOMER_HPP__
#define __ZOOMER_HPP__

#include <qwt_plot_zoomer.h>
#include <qwt_plot_canvas.h>

class Zoomer: public QwtPlotZoomer
{
    public:

        Zoomer(int xAxis, int yAxis, QwtPlotCanvas* canvas);
};

#endif /* __ZOOMER_HPP__ */