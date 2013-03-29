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

void MainWindow::checkFolderContent(const QDir &MSDir) const
{
    if (!MSDir.exists())
        throw QException(tr("Le dossier n'existe pas"));

    // Check if the folder content all the files
    QStringList files;
    files << MEGASQUIRT_DAT_FILENAME << INERTIE_FILENAME << PROTOWHEEL_FILENAME;

    foreach (QString filename, files)
    {
        if(!MSDir.exists(filename))
            throw QException(tr("Fichier ") + filename + tr(" inexistant"));
    }
}

void MainWindow::createCoupleAndPowerCurves(
        QString const& megasquirtCSVFilename)
{
    qDebug() << "Début de la création des courbes ...";

    QCSVParser MSFile(megasquirtCSVFilename, ';', QString::KeepEmptyParts);

    qDebug() << "nombre de colonnes = " << MSFile.columnCount();

    foreach (QString header, MSFile.headersList()) {
        qDebug() << header;
    }

    qDebug() << "avant recupération de données";
    QCSVColumn times = MSFile["times"];
    qDebug() << "times récupérés !!!!";
    QCSVColumn rpm   = MSFile["rpm"];
    qDebug() << "RPM récupérés !!!!";

    QVector<QPointF> couplePoints;

    double w1, w2;
    double accAngulaire;
    double couple;
    const double PI = 3.141592653589793;
    const double Jdelta = 0.22; // FIXME : demander à l'utilisateur de rentrer la valeur

    int lastDataIndice = 0;
    w2 = (PI * rpm.at(0).toDouble()) / 30;

    for (int i(1); i < times.count(); ++i)
    {
        // filtrage des données continues ayant le meme nombre de RPM
        if (rpm.at(i - 1) == rpm.at(i))
        {
            qDebug() << "ligne " << i - 1 << " et " << i << " : RPM identique "
                     << rpm.at(i);
            continue;
        }

        w1 = w2;

        qDebug() << "w1 = " << w1;

    /* ---------------------------------------------------------------------- *
     *                           ωx = (Π * Nx) / 30                           *
     * ---------------------------------------------------------------------- *
     * ωx = Vitesse angulaire à l'instant x (rad/s)                           *
     * Π  = Pi, constante qui vaut 3.141592653589793...                       *
     * N  = tours par minutes à l'instant x == RPM (rad/s)                           *
     * ---------------------------------------------------------------------- */

        w2 = (PI * rpm.at(i).toDouble()) / 30;

        qDebug() << "w2 = " << w2;

    /* ---------------------------------------------------------------------- *
     *                        α = (ω2 - ω1) / (t2 - t1)                       *
     * ---------------------------------------------------------------------- *
     * α  = Accélération angulaire (rad/s²)                                   *
     * ωx = Vitesse angulaire à l'instant x (rad/s)                           *
     * tx = temps x (s)                                                       *
     * ---------------------------------------------------------------------- */

        accAngulaire =                    (w2 - w1)
                                              /
     ((times.at(i).toDouble() - times.at(lastDataIndice).toDouble()) / 1000000);
        // times values are microseconds

        qDebug() << "t2 = " << times.at(i).toDouble() << " microsecondes";
        qDebug() << "t1 = " << times.at(lastDataIndice).toDouble() << " microsecondes";
        qDebug() << "Δt (t2 - t1) = " << ((times.at(i).toDouble() - times.at(lastDataIndice).toDouble()) / 1000000);
        qDebug() << "α = " << accAngulaire;

    /* ---------------------------------------------------------------------- *
     *                              C = JΔ * α                                *
     * ---------------------------------------------------------------------- *
     * C  = Couple (N.m)                                                      *
     * JΔ = moment d'inertie (kg.m²)                                          *
     * α  = Accélération angulaire (rad/s²)                                   *
     * ---------------------------------------------------------------------- */

        couple = Jdelta * accAngulaire;

        // Ajout du point à la liste des points de la courbe du couple
        couplePoints.append(QPointF(rpm.at(i).toDouble(), couple));
        lastDataIndice = i;

        qDebug() << "Coordonnée (rpm, couple) = " << rpm.at(i).toDouble() << couple;
    }

    // Crée la courbe du couple
    QwtPointSeriesData* coupleSerieData = new QwtPointSeriesData(couplePoints);
    QwtPlotCurve* coupleCurve = new QwtPlotCurve(tr("Couple"));
    coupleCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
    coupleCurve->setItemAttribute(QwtPlotItem::Legend);
    coupleCurve->setLegendAttribute(QwtPlotCurve::LegendShowLine);
    coupleCurve->setPen(QPen(Qt::darkRed, 1));
    coupleCurve->setData(coupleSerieData);
    coupleCurve->attach(this->CPPlot);

    this->setPlotCurveVisibile(coupleCurve, true);

    qDebug() << "Fin de la création des courbes ...";
}

void MainWindow::on_actionImportData_triggered(void)
{
    QDir MSDir = QFileDialog::getExistingDirectory(
                this, tr("Sélectionnez le dossier contenant "
                "les données du Megasquirt"), QDir::homePath());
    try
    {
        // Check the folder content
        this->checkFolderContent(MSDir);

        // Remove oldest csv file if exists
        QString csvFilename = MSDir.filePath(MEGASQUIRT_CSV_FILENAME);
        QFile csvFile(csvFilename);
        if (csvFile.exists())
            csvFile.remove();

        // Generate csv file by extracting needed data from dat file
        QStringList megasquirtParameters;
        megasquirtParameters << "pulseWidth1" << "rpm" << "batteryVoltage";
        MSManager megasquirtManager;
        megasquirtManager.datToCSV(MSDir.filePath(MEGASQUIRT_DAT_FILENAME),
                                   csvFilename,
                                   megasquirtParameters);

        // Create couple and power curves
        this->createCoupleAndPowerCurves(
                    MSDir.filePath(MEGASQUIRT_CSV_FILENAME));
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Importation annulée"), ex.what());
    }
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
