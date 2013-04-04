/* TODO
 * -------
 * 1) Menu contextuel lors d'un clic droit sur un élément de la légende
 *      - Centrer sur
 *      - Effacer
 *      - Changer la couleur
 */

#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Plot/Plot.hpp"
#include "Plot/PlotCurve.hpp"
#include "Utils/QException.hpp"
#include "Utils/QCSVParser.hpp"
#include "Megasquirt/MSManager.hpp"
#include "Megasquirt/MSFileConverterDialog.hpp"
#include "Megasquirt/MSFileParameterDialog.hpp"
#include "Megasquirt/MSDataParameterDialog.hpp"
#include <qwt_legend_item.h>
#include <qwt_plot_renderer.h>

#define TAB_COUPLE_AND_POWER 0
#define TAB_MEGASQUIRT_DATA  1

#define KEY_INERTIE        "Inertie"
#define KEY_PROTOWHEEL     "ProtoWheel"
#define KEY_MEGASQUIRT_DAT "MegasquirtDAT"
#define KEY_MEGASQUIRT_CSV "MegasquirtCSV"

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
        void  createMSPlotZone(void);
        void  createCPPlotZone(void);
        Plot* currentPlot(void) const;
        void  updateMenus(void);
        void  readSettings(void);
        void  writeSettings(void) const;
        void  initSettings(void) const;

        // Megasquirt management methods
        void checkFolderContent(QDir const& MSDir) const;
        void createCoupleAndPowerCurves(QString const& inertieCSVFilename);
        void createCoupleAndPowerCurves_old(QString const& megasquirtCSVFilename);

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
        void on_actionConfigureMegasquirtFileName_triggered(void);
        void on_actionExportToPDF_triggered(void);

        // Personal slots - Legend actions management
        void eraseCurve(void);
        void centerOnCurve(void);
        void changeCurveColor(void);
        void setPlotCurveVisibile(QwtPlotItem* item, bool visible);
        void showLegendContextMenu(QwtPlotItem const* item, QPoint const& pos);

    protected:

        // GUI
        Ui::MainWindow* ui;

        // Plots
        Plot*      MSPlot;
        Plot*      CPPlot;
        QMenu*     legendContextMenu;
        PlotCurve* curveAssociatedToLegendItem;

        // CSV parser
        QCSVParser MSPlotParser;
};

#endif /* __MAINWINDOW_HPP__ */
