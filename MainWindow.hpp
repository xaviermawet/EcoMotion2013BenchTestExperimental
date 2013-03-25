#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Plot/Plot.hpp"
#include "Utils/QException.hpp"
#include "Utils/QCSVParser.hpp"
#include <qwt_plot_curve.h>
#include <qwt_legend_item.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:

        explicit MainWindow(QWidget* parent = NULL);
        virtual ~MainWindow(void);

    protected:

        void centerOnScreen(void);
        void createPlotZone(void);

    private slots:

        // Autoconnect
        void on_actionImportData_triggered(void);
        void on_actionQuit_triggered(void);
        void on_addCurvePushButton_clicked(void);

        // Personal slots
        void setPlotCurveVisibile(QwtPlotItem* item, bool visible);

    protected:

        // GUI
        Ui::MainWindow* ui;

        // Plot
        Plot* plot;

        // CSV parser
        QCSVParser parser;

};

#endif /* __MAINWINDOW_HPP__ */
