#ifndef __PLOT_HPP__
#define __PLOT_HPP__

#include <qwt_plot.h>
#include <qwt_legend.h>
#include <qwt_plot_zoomer.h>

class Plot : public QwtPlot
{
    Q_OBJECT

    public:

        explicit Plot(QString const& title, QWidget* parent = NULL);
        explicit Plot(QwtText const& title, QWidget* parent = NULL);
        virtual ~Plot(void);

    protected:

        QwtLegend*     legend;
        QwtPlotZoomer* zoomer;
};

#endif /* __PLOT_HPP__ */
