#include "Zoomer.hpp"

Zoomer::Zoomer(int xAxis, int yAxis, QwtPlotCanvas *canvas) :
    QwtPlotZoomer(xAxis, yAxis, canvas)
{
    this->setTrackerMode( QwtPicker::AlwaysOff );
    this->setRubberBand( QwtPicker::NoRubberBand );

    // RightButton: zoom out by 1
    // Ctrl+RightButton: zoom out to full size

    this->setMousePattern( QwtEventPattern::MouseSelect2,
                           Qt::RightButton, Qt::ControlModifier );
    this->setMousePattern( QwtEventPattern::MouseSelect3,
                           Qt::RightButton );
}
