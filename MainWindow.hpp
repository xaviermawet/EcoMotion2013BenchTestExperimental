#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "Plot/Plot.hpp"

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

    protected:

        // GUI
        Ui::MainWindow* ui;

        // Plot
        Plot* plot;
};

#endif /* __MAINWINDOW_HPP__ */
