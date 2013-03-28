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
#include "Megasquirt/MSManager.hpp"
#include "Megasquirt/MSFileConverterDialog.hpp"
#include <qwt_plot_curve.h>
#include <qwt_legend_item.h>

#define TAB_COUPLE_AND_POWER 0
#define TAB_MEGASQUIRT_DATA  1

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

        void  centerOnScreen(void);
        void  createMSPlotZone(void);
        void  createCPPlotZone(void);
        Plot* currentPlot(void) const;
        void  updateMenus(void);

    private slots:

        // Autoconnect
        void on_actionImportData_triggered(void);
        void on_actionQuit_triggered(void);
        void on_addCurvePushButton_clicked(void);
        void on_actionIncreaseAccuracy_triggered(void);
        void on_actionReduceAccuracy_triggered(void);
        void on_actionShowGrid_triggered(bool visible);
        void on_actionShowLabelPosition_triggered(bool visible);
        void on_actionShowCrossLine_triggered(bool visible);
        void on_mainTabWidget_currentChanged(int index);
        void on_actionLoadCSV_triggered(void);
        void on_actionDatToCSV_triggered(void);

        // Personal slots
        void setPlotCurveVisibile(QwtPlotItem* item, bool visible);

    protected:

        // GUI
        Ui::MainWindow* ui;

        // Plots
        Plot* MSPlot;
        Plot* CPPlot;

        // CSV parser
        QCSVParser parser;

        // Megasquirt
        MSManager MegasquirtManager;

};

#endif /* __MAINWINDOW_HPP__ */
