#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Plot/Plot.hpp"
#include "Plot/DoubleYAxisPlot.hpp"
#include "Plot/PlotCurve.hpp"
#include "Utils/QException.hpp"
#include "Utils/QCSVParser.hpp"
#include "Megasquirt/MSManager.hpp"
#include "Megasquirt/MSFileConverterDialog.hpp"
#include "Megasquirt/MSFileParameterDialog.hpp"
#include "Megasquirt/MSDataParameterDialog.hpp"
#include "polifitgsl.hpp"
#include <qwt_legend_item.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_rescaler.h>

#define TAB_BENCH_TEST       0
    #define TAB_COUPLE_AND_POWER          0
    #define TAB_COUPLE_AND_SPECIFIC_POWER 1
    #define TAB_REDUCTION_RATIO           2
    #define TAB_DISTANCE                  3
#define TAB_MEGASQUIRT_DATA  1

#define KEY_INERTIE        "Inertie"
#define KEY_PROTOWHEEL     "ProtoWheel"
#define KEY_MEGASQUIRT_DAT "MegasquirtDAT"
#define KEY_MEGASQUIRT_CSV "MegasquirtCSV"

typedef QList<Plot*> PlotList;

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

        virtual void closeEvent(QCloseEvent* event);

        void  centerOnScreen(void);
        void  createPlotLegendContextMenu(void);
        void  createMegasquirtDataPlotZone(void);
        void  createCouplePowerPlotZone(void);
        void  createCoupleSpecificPowerPlotZone(void);
        void  createReductionRatioPlotZone(void);
        void  createDistancePlotZone(void);
        Plot* currentPlot(void) const;
        void  readSettings(void);
        void  writeSettings(void) const;
        void  initSettings(void) const;

        // Megasquirt management methods
        QDir getMegasquirtDataFolder(void);
        void getTimesFromCSV(QVector<double>& timeValues,
                             QString const& csvFilePath) const;
        void createCoupleAndPowerCurves(QVector<double> const& inertieTimes,
                                        MSDataParameterDialog const& param);

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
        void on_actionLoadCSV_triggered(void);
        void on_actionDatToCSV_triggered(void);
        void on_actionConfigureMegasquirtFileName_triggered(void);
        void on_actionExportToPDF_triggered(void);

        // Personal slots
        void  updateMenus(void);

        // Legend actions management
        void eraseCurve(void);
        void centerOnCurve(void);
        void changeCurveColor(void);
        void createPolynomialTrendline(void);
        void setPlotCurveVisibile(QwtPlotItem* item, bool visible);
        void showLegendContextMenu(QwtPlotItem const* item, QPoint const& pos);

    protected:

        // GUI
        Ui::MainWindow* ui;

        // Plot context Menu
        QMenu*     legendContextMenu;
        PlotCurve* curveAssociatedToLegendItem;

        // Megasquirt data plots
        Plot*      megasquirtDataPlot;
        QCSVParser MSPlotParser;

        // Bench test plots
        DoubleYAxisPlot* couplePowerPlot;
        DoubleYAxisPlot* coupleSpecificPowerPlot;
        Plot* reductionRatioPlot;
        Plot* distancePlot;
        QCSVParser benchParser;

        // List of all Plots
        PlotList plots;
};

#endif /* __MAINWINDOW_HPP__ */
