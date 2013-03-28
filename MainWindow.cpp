#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    MSPlot(NULL), CPPlot(NULL), parser()
{
    // Display Configuration
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // GUI Configuration
    this->ui->setupUi(this);

    // Plots configuration
    this->createMSPlotZone();
    this->createCPPlotZone();
    this->updateMenus();

    // Center the window on the screen
    this->centerOnScreen();
}

MainWindow::~MainWindow(void)
{
    qDebug() << "MainWindow début destructeur";

    delete this->ui;
    delete this->MSPlot;
    delete this->CPPlot;

    qDebug() << "MainWindow Fin destructeur";
}

void MainWindow::centerOnScreen(void)
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    this->move(screenCenterX - width() / 2, screenCenterY - height() / 2);
}

void MainWindow::createMSPlotZone(void)
{
    this->MSPlot = new Plot("Données du Megasquirt", this);
    this->MSPlot->setAxisTitle(Plot::xBottom, tr("Temps (s)"));
    this->ui->MSDataHLayout->addWidget(this->MSPlot);

    // Connect plot signals to slots
    connect(this->MSPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
}

void MainWindow::createCPPlotZone(void)
{
    this->CPPlot = new Plot("Couple - Puissance - Puissance spécifique", this);
    this->CPPlot->setAxisTitle(Plot::xBottom, tr("Tours par minute (rad/s)"));
    this->ui->coupleAndPowerHLayout->addWidget(this->CPPlot);

    // Connect plot signals to slots
    connect(this->CPPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
}

Plot* MainWindow::currentPlot(void) const
{
    switch (this->ui->mainTabWidget->currentIndex())
    {
        case TAB_COUPLE_AND_POWER:
            return this->CPPlot;
        case TAB_MEGASQUIRT_DATA:
            return this->MSPlot;
        default:
            return NULL;
    }
}

void MainWindow::updateMenus(void)
{
    // Get the current plot
    Plot* plot = this->currentPlot();

    // Update menu edit actions
    this->ui->actionShowGrid->setChecked(plot->isGridVisible());
    this->ui->actionShowCrossLine->setChecked(plot->isCrossLineVisible());
    this->ui->actionShowLabelPosition->setChecked(
                plot->isLabelPositionVisible());

    // Update menu file actions
    int currentTab = this->ui->mainTabWidget->currentIndex();

    this->ui->actionImportData->setVisible(currentTab == TAB_COUPLE_AND_POWER);
    this->ui->actionDatToCSV->setVisible(currentTab == TAB_MEGASQUIRT_DATA);
    this->ui->actionLoadCSV->setVisible(currentTab == TAB_MEGASQUIRT_DATA);
}

void MainWindow::on_actionImportData_triggered(void)
{
    QMessageBox::information(this, tr("Couple - Puissance"),
                             tr("Importation de données"));
}

void MainWindow::on_actionQuit_triggered(void)
{
    // Close the main window
    qApp->quit();
}

void MainWindow::on_addCurvePushButton_clicked(void)
{
    QString curveName = this->ui->paramMegasquirtComboBox->currentText();

    try
    {
        if (curveName.isEmpty())
            throw QException(tr("Aucune courbe sélectionnée"));

        qDebug() << "Courbe a afficher : " << curveName;

        // Récupération du temps exprimé en microsecondes
        QCSVRow secondes = this->parser["times"];

        // Récupération de la colonne de données a représenter par une courbe
        QCSVRow data     = this->parser[curveName];

        // Création de la liste de points de la courbe
        QVector<QPointF> vect;
        for (int i(0); i < data.count(); ++i)
        {
            qreal xTps = secondes.at(i).toDouble() / 1000000;
            qreal yData = data.at(i).toDouble();
            vect.append(QPointF(xTps, yData));
        }

        QwtPointSeriesData* serieData = new QwtPointSeriesData(vect);

        // Create a new curves
        QwtPlotCurve* curve = new QwtPlotCurve(curveName);
        curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        curve->setItemAttribute(QwtPlotItem::Legend);
        curve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
        curve->setPen(QPen(Qt::darkRed, 1));
        curve->setData(serieData);
        curve->attach(this->MSPlot);

        this->setPlotCurveVisibile(curve, true);
    }
    catch (QException const& ex)
    {
        QMessageBox::information(this, tr("Ajout de la courbe annulé"),
                                 ex.what());
    }
}

void MainWindow::on_actionIncreaseAccuracy_triggered(void)
{
    // Get the current plot
    Plot* plot = this->currentPlot();

    // Set the maximum number of major scale intervals for a specified axis
    plot->setAxisMaxMajor(QwtPlot::yLeft,
                          plot->axisMaxMajor(QwtPlot::yLeft) + 1);
    plot->setAxisMaxMajor(QwtPlot::yRight,
                          plot->axisMaxMajor(QwtPlot::yRight) + 1);
    plot->setAxisMaxMajor(QwtPlot::xBottom,
                          plot->axisMaxMajor(QwtPlot::xBottom) + 1);
}

void MainWindow::on_actionReduceAccuracy_triggered(void)
{
    // Get the current plot
    Plot* plot = this->currentPlot();

    // Set the maximum number of major scale intervals for a specified axis
    plot->setAxisMaxMajor(QwtPlot::yLeft,
                          plot->axisMaxMajor(QwtPlot::yLeft) - 1);
    plot->setAxisMaxMajor(QwtPlot::yRight,
                          plot->axisMaxMajor(QwtPlot::yRight) - 1);
    plot->setAxisMaxMajor(QwtPlot::xBottom,
                          plot->axisMaxMajor(QwtPlot::xBottom) - 1);
}

void MainWindow::on_actionShowGrid_triggered(bool visible)
{
    this->currentPlot()->setGridVisible(visible);
}

void MainWindow::on_actionShowLabelPosition_triggered(bool visible)
{
    this->currentPlot()->setLabelPositionVisible(visible);
}

void MainWindow::on_actionShowCrossLine_triggered(bool visible)
{
    this->currentPlot()->setCrossLineVisible(visible);
}

void MainWindow::on_mainTabWidget_currentChanged(int index)
{
    Q_UNUSED(index);

    this->updateMenus();
}

void MainWindow::on_actionLoadCSV_triggered(void)
{
    /* ---------------------------------------------------------------------- *
     *                         Récupérer le fichier CSV                       *
     * ---------------------------------------------------------------------- *
     * Devra changer --> sélection d'un dossier qui contient tous les         *
     * fichiers nécéssaires                                                   *
     * ---------------------------------------------------------------------- */
    QString filename = QFileDialog::getOpenFileName
            (
                this, tr("Megasquirt - Ouvrir un fichier CSV"),
                QDir::homePath(), tr("Fichiers CSV (*.csv)")
            );

    try
    {
        if (filename.isNull() || filename.isEmpty())
            throw QException(tr("Vous n'avez pas sélectionné de fichier"));

        // Load information from the CSV file
        this->parser.parse(filename, ';', QString::SkipEmptyParts);

        // Récupérer les noms des colonnes du fichier csv
        this->ui->paramMegasquirtComboBox->clear();
        this->ui->paramMegasquirtComboBox->addItems(this->parser.headersList());
    }
    catch (QException const& ex)
    {
        QMessageBox::warning(this, tr("Importation annulée"), ex.what());
    }
}

void MainWindow::setPlotCurveVisibile(QwtPlotItem* item, bool visible)
{
    item->setVisible(visible);
    QWidget* w = item->plot()->legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(visible);

    item->plot()->replot();
}

void MainWindow::on_actionDatToCSV_triggered(void)
{
    try
    {
        /* Get dat file name */
        QString datFileName = QFileDialog::getOpenFileName(
                    this, tr("Selectionné le fichier de données du Megasquirt"),
                    QDir::homePath(), tr("Fichier de données (*.dat)"));

        MSFileConverterDialog dialog(datFileName, this);
        dialog.exec();
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Conversion annulée"), ex.what());
    }
}
