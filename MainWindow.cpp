#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    legendContextMenu(NULL), curveAssociatedToLegendItem(NULL),
    megasquirtDataPlot(NULL), MSPlotParser(), couplePowerPlot(NULL),
    coupleSpecificPowerPlot(NULL), reductionRatioPlot(NULL),
    distancePlot(NULL), benchParser()
{
    // Display Configuration
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // GUI configuration
    this->ui->setupUi(this);
    connect(this->ui->mainTabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(updateMenus()));
    connect(this->ui->benchTestTabWidget, SIGNAL(currentChanged(int)),
            this, SLOT(updateMenus()));

    // Plots configuration
    this->createPlotLegendContextMenu();
    this->createCouplePowerPlotZone();
    this->createCoupleSpecificPowerPlotZone();
    this->createReductionRatioPlotZone();
    this->createMegasquirtDataPlotZone();
    this->createDistancePlotZone();

    // Settings configuration
    QCoreApplication::setOrganizationName("EcoMotion");
    QCoreApplication::setApplicationName("BenchTest");
    this->initSettings();
    this->readSettings();
    this->updateMenus();

    // Center the window on the screen
    this->centerOnScreen();
}

MainWindow::~MainWindow(void)
{
    qDebug() << "MainWindow début destructeur";

    delete this->ui;
    delete this->legendContextMenu;

    foreach (Plot* plot, this->plots)
        delete plot;

    qDebug() << "MainWindow Fin destructeur";
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    // Save the state of the mainWindow and its widgets
    this->writeSettings();

    QMainWindow::closeEvent(event);
}

void MainWindow::centerOnScreen(void)
{
    QDesktopWidget screen;

    QRect screenGeom = screen.screenGeometry(this);

    int screenCenterX = screenGeom.center().x();
    int screenCenterY = screenGeom.center().y();

    this->move(screenCenterX - width() / 2, screenCenterY - height() / 2);
}

void MainWindow::createPlotLegendContextMenu(void)
{
    // Legend actions
    this->legendContextMenu = new QMenu(this);
    this->legendContextMenu->addAction(
                QIcon(":/Icons/erase"), tr("Effacer"),this,
                SLOT(eraseCurve()));
    this->legendContextMenu->addAction(
                QIcon(":/Icons/focusOn"), tr("Centrer sur"), this,
                SLOT(centerOnCurve()));
    this->legendContextMenu->addAction(
                QIcon(":/Icons/color"), tr("Changer la couleur"), this,
                SLOT(changeCurveColor()));
    this->legendContextMenu->addAction(
                QIcon(":/Icons/tendLine"),
                tr("Ajouter une courbe de tendance polynomiale"), this,
                SLOT(createPolynomialTrendline()));
}

void MainWindow::createMegasquirtDataPlotZone(void)
{
    this->megasquirtDataPlot = new Plot("Données du Megasquirt", this);
    this->megasquirtDataPlot->setAxisTitle(Plot::xBottom, tr("Temps (s)"));
    this->ui->megasquirtDataSplitter->addWidget(this->megasquirtDataPlot);

    // Connect plot signals to slots
    connect(this->megasquirtDataPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
    connect(this->megasquirtDataPlot,
            SIGNAL(legendRightClicked(const QwtPlotItem*,QPoint)),
            this, SLOT(showLegendContextMenu(const QwtPlotItem*,QPoint)));

    // Settings management configuration
    this->megasquirtDataPlot->setObjectName("MegasquirtDataPlot");
    this->plots.append(this->megasquirtDataPlot);
}

void MainWindow::createCouplePowerPlotZone(void)
{
    this->couplePowerPlot = new DoubleYAxisPlot(
                tr("Couple - Puissance"), 0.01, this);
    this->couplePowerPlot->setAxisTitle(Plot::yLeft, tr("Couple (N.m)"));;
    this->couplePowerPlot->setAxisTitle(Plot::yRight, tr("Puissance (W)"));
    this->couplePowerPlot->setAxisTitle(
                Plot::xBottom, tr("Tours par minute (tr/min)"));

    // Add plot into a main window's layout
    this->ui->coupleAndPowerHLayout->addWidget(this->couplePowerPlot);

    // Connect plot signals to slots
    connect(this->couplePowerPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
    connect(this->couplePowerPlot,
            SIGNAL(legendRightClicked(const QwtPlotItem*,QPoint)),
            this, SLOT(showLegendContextMenu(const QwtPlotItem*,QPoint)));

    // Settings management configuration
    this->couplePowerPlot->setObjectName("CouplePowerPlot");
    this->plots.append(this->couplePowerPlot);
}

void MainWindow::createCoupleSpecificPowerPlotZone(void)
{
    this->coupleSpecificPowerPlot = new DoubleYAxisPlot(
                "Couple - Puissance spécifique", 2, this);
    this->coupleSpecificPowerPlot->setAxisTitle(
                Plot::yLeft,tr("Couple (N.m)"));;
    this->coupleSpecificPowerPlot->setAxisTitle(
                Plot::yRight, tr("Puissance Spécifique (l/kwh)"));
    this->coupleSpecificPowerPlot->setAxisTitle(
                Plot::xBottom, tr("Tours par minute (tr/min)"));

    // Add plot into a main window's layout
    this->ui->coupleAndSpecificPowerHLayout->addWidget(
                this->coupleSpecificPowerPlot);

    // Connect plot signals to slots
    connect(this->coupleSpecificPowerPlot,
            SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
    connect(this->coupleSpecificPowerPlot,
            SIGNAL(legendRightClicked(const QwtPlotItem*,QPoint)),
            this, SLOT(showLegendContextMenu(const QwtPlotItem*,QPoint)));

    // Settings management configuration
    this->coupleSpecificPowerPlot->setObjectName("CoupleSpecificPowerPlot");
    this->plots.append(this->coupleSpecificPowerPlot);
}

void MainWindow::createReductionRatioPlotZone(void)
{
    this->reductionRatioPlot = new DoubleXAxisPlot(
                tr("Rapport de réduction"), 2, this);
    this->reductionRatioPlot->setAxisTitle(
                Plot::yLeft, tr("i (rapport de réduction)"));
    this->reductionRatioPlot->setAxisTitle(
                Plot::xBottom, tr("Tours par minute du moteur (tr/min)"));
    this->reductionRatioPlot->setAxisTitle(
                Plot::xTop, tr("Tours par minute du rouleau (tr/min)"));

    this->ui->reductionRatioHLayout->addWidget(this->reductionRatioPlot);

    // Connect plot signals to slots
    connect(this->reductionRatioPlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
    connect(this->reductionRatioPlot,
            SIGNAL(legendRightClicked(const QwtPlotItem*,QPoint)),
            this, SLOT(showLegendContextMenu(const QwtPlotItem*,QPoint)));

    // Settings management configuration
    this->reductionRatioPlot->setObjectName("ReductionRatioPlot");
    this->plots.append(this->reductionRatioPlot);
}

void MainWindow::createDistancePlotZone(void)
{
    this->distancePlot = new Plot(tr("Distances"), this);
    this->distancePlot->setAxisTitle(Plot::yLeft, tr("Distance (m)"));
    this->distancePlot->setAxisTitle(Plot::xBottom, tr("Temps (s)"));
    this->ui->wheelSlippageHLayout->addWidget(this->distancePlot);

    // Connect plot signals to slots
    connect(this->distancePlot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));
    connect(this->distancePlot,
            SIGNAL(legendRightClicked(const QwtPlotItem*,QPoint)),
            this, SLOT(showLegendContextMenu(const QwtPlotItem*,QPoint)));

    // Settings management configuration
    this->distancePlot->setObjectName("DistancePlot");
    this->plots.append(this->distancePlot);
}

Plot* MainWindow::currentPlot(void) const
{
    switch (this->ui->mainTabWidget->currentIndex())
    {
    case TAB_BENCH_TEST:
        switch (this->ui->benchTestTabWidget->currentIndex())
        {
            case TAB_COUPLE_AND_POWER:
                return this->couplePowerPlot;
            case TAB_COUPLE_AND_SPECIFIC_POWER:
                return this->coupleSpecificPowerPlot;
            case TAB_REDUCTION_RATIO:
                return this->reductionRatioPlot;
            case TAB_DISTANCE:
                return this->distancePlot;
            default:
                return NULL;
        }
    case TAB_MEGASQUIRT_DATA:
        return this->megasquirtDataPlot;
    default:
        return NULL;
    }
}

void MainWindow::updateMenus(void)
{
    // Get the current plot
    Plot* plot = this->currentPlot();
    if (!plot) return;

    // Update menu edit actions
    this->ui->actionShowGrid->setChecked(plot->isGridVisible());
    this->ui->actionShowCrossLine->setChecked(plot->isCrossLineVisible());
    this->ui->actionShowLabelPosition->setChecked(
                plot->isLabelPositionVisible());
    this->ui->actionShowLabelPosition->setEnabled(!plot->isCrossLineVisible());

    // Update menu file actions
    int currentMainTabWidgetIndex = this->ui->mainTabWidget->currentIndex();

    this->ui->actionImportData->setVisible(
                currentMainTabWidgetIndex == TAB_BENCH_TEST);
    this->ui->actionDatToCSV->setVisible(
                currentMainTabWidgetIndex == TAB_MEGASQUIRT_DATA);
    this->ui->actionLoadCSV->setVisible(
                currentMainTabWidgetIndex == TAB_MEGASQUIRT_DATA);

    // Update menu configure actions
    this->ui->menuConfigure->menuAction()->setVisible(
                currentMainTabWidgetIndex == TAB_BENCH_TEST);
}

void MainWindow::readSettings(void)
{
    QSettings settings;

    // Restore plots settings
    foreach (Plot* plot, this->plots)
    {
        settings.beginGroup(plot->objectName());
        plot->setGridVisible(
                    settings.value("isGridVisible", true).toBool());
        plot->setCrossLineVisible(
                    settings.value("isCrossLineVisible", false).toBool());
        plot->setLabelPositionVisible(
                    settings.value("isLabelPositionVisible", true).toBool());
        settings.endGroup();
    }

    // Restore MainWindow settings
    settings.beginGroup("MainWindow");

    /* Contourne le bug non résolu par Qt de la restauration de la géométrie
     * d'une fenetre maximisée alors qu'elle est déjà maximisée */
    if (settings.value("isMaximized", true).toBool())
        this->showMaximized();
    else
        this->restoreGeometry(settings.value("geometry").toByteArray());

    this->ui->mainTabWidget->setCurrentIndex(
                settings.value("mainTabWidgetCurrentIndex", 0).toInt());
    this->ui->benchTestTabWidget->setCurrentIndex(
                settings.value("benchTestTabWidgetCurrentIndex", 0).toInt());
    this->ui->megasquirtDataSplitter->restoreState(
                settings.value("megasquirtDataSplitter").toByteArray());

    settings.endGroup();
}

void MainWindow::writeSettings(void) const
{
    QSettings settings;

    // Save plots settings
    foreach (Plot* plot, this->plots)
    {
        settings.beginGroup(plot->objectName());
        settings.setValue("isGridVisible",
                          plot->isGridVisible());
        settings.setValue("isCrossLineVisible",
                          plot->isCrossLineVisible());
        settings.setValue("isLabelPositionVisible",
                          plot->isLabelPositionVisible());
        settings.endGroup();
    }

    // Restore MainWindow settings
    settings.beginGroup("MainWindow");
    settings.setValue("isMaximized", this->isMaximized());
    settings.setValue("geometry", this->saveGeometry());
    settings.setValue("mainTabWidgetCurrentIndex",
                      this->ui->mainTabWidget->currentIndex());
    settings.setValue("benchTestTabWidgetCurrentIndex",
                      this->ui->benchTestTabWidget->currentIndex());
    settings.setValue("megasquirtDataSplitter",
                      this->ui->megasquirtDataSplitter->saveState());
    settings.endGroup();
}

void MainWindow::initSettings(void) const
{
    QSettings settings;

    settings.beginGroup("files");

    if(!settings.contains(KEY_INERTIE))
        settings.setValue(KEY_INERTIE, "Inertie.csv");

    if(!settings.contains(KEY_PROTOWHEEL))
        settings.setValue(KEY_PROTOWHEEL, "ProtoWheel.csv");

    if(!settings.contains(KEY_MEGASQUIRT_DAT))
        settings.setValue(KEY_MEGASQUIRT_DAT, "Megasquirt.dat");

    if(!settings.contains(KEY_MEGASQUIRT_CSV))
        settings.setValue(KEY_MEGASQUIRT_CSV, "Megasquirt.csv");

    settings.endGroup();
}

QDir MainWindow::getMegasquirtDataFolder(void)
{
    // Get dir path
    QString dirPath = QFileDialog::getExistingDirectory(
                      this, tr("Sélectionnez le dossier contenant "
                      "les données du Megasquirt"), QDir::homePath());

    if (dirPath.isEmpty()) // User canceled
        throw QException(tr("Aucun dossier sélectionné"));

    // Get all file names from settings
    QSettings settings;
    settings.beginGroup("files");

    QStringList fileNames;
    fileNames << settings.value(KEY_INERTIE).toString()
              << settings.value(KEY_PROTOWHEEL).toString()
              << settings.value(KEY_MEGASQUIRT_DAT).toString();

    settings.endGroup();

    // Check if the folder content all the files
    QDir MSDir(dirPath);
    foreach (QString filename, fileNames)
        if(!MSDir.exists(filename))
            throw QException(tr("Fichier ") + filename + tr(" inexistant"));

    return MSDir; // Is a valid folder
}

void MainWindow::getTimesFromCSV(QVector<double>& timeValues,
                                 QString const& csvFilePath) const
{
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        throw QException(QObject::tr("Impossible d'ouvrir le fichier ")
                         + csvFilePath);

    // Read the file line-by-line and covert time value into seconds
    while(!file.atEnd())
        timeValues.append(QString(file.readLine()).toDouble() / 1000000);
    file.close();

    if (timeValues.count() < 3)
        throw QException(QObject::tr("Le fichier ") + csvFilePath +
                         QObject::tr(" ne contient pas assez de valeurs"));

    // Remove wrong values (first ones)
    for (int i(0); i < 3; ++i)
        timeValues.pop_front();

    // We need at least 3 values of time
    if (timeValues.count() < 3)
        throw QException(QObject::tr("Le fichier ") + csvFilePath +
                         QObject::tr(" ne contient pas assez de valeurs"));
}

void MainWindow::createCoupleAndPowerCurves(QVector<double> const& inertieTimes,
                                            MSDataParameterDialog const& param)
{
    // Constants
    const double PI       = 3.14159265358979323846;
    const double Jdelta   = param.inertia();
    const double deadTime = param.deadTime();
    const double voltCorr = param.voltageCorrection();
    const double injectorFlowRate = param.injectorVolumetricFlowRate() / 60000;

    double angularSpeed_a, angularSpeed_b;
    double ta_average, tb_average;
    double angularAcceleration;
    double couple, power, specificPower;

    int indiceMS(0);

    QVector<QPointF> powerPoints;
    QVector<QPointF> couplePoints;
    QVector<QPointF> specificPowerPoints;
    QVector<QPointF> reductionRatioPoints1;
    QVector<QPointF> reductionRatioPoints2;

    /* ---------------------------------------------------------------------- *
     *                           ωa = 2π / (t2 - t1)                          *
     * ---------------------------------------------------------------------- *
     * ωa = Première vitesse angulaire (rad/s)                                *
     * Π  = Pi, constante qui vaut 3.141592653589793...                       *
     * tx = temps à l'instant x (s) ave t2 > t1                               *
     * ---------------------------------------------------------------------- */

    angularSpeed_b = (2 * PI) / (inertieTimes.at(1) - inertieTimes.at(0));

    for (int i(2); i < inertieTimes.count(); ++i)
    {
        angularSpeed_a = angularSpeed_b;

    /* ---------------------------------------------------------------------- *
     *                           ωb = 2π / (t3 - t2)                          *
     * ---------------------------------------------------------------------- *
     * ωb = Deuxième vitesse angulaire (rad/s)                                *
     * Π  = Pi, constante qui vaut 3.141592653589793...                       *
     * tx = temps à l'instant x (s) ave t2 > t1                               *
     * ---------------------------------------------------------------------- */

        angularSpeed_b = (2 * PI) / (inertieTimes.at(i) - inertieTimes.at(i-1));

    /* ---------------------------------------------------------------------- *
     *                        ta_moyen = (t1 + t2) / 2                        *
     *                        tb_moyen = (t2 + t3) / 2                        *
     *                   α = (ωb - ωa) / (tb_moyen - ta_moyen)                *
     * ---------------------------------------------------------------------- *
     * α  = Accélération angulaire (rad/s²)                                   *
     * ωx = Vitesse angulaire à l'instant x (rad/s)                           *
     * tx = temps x (s)                                                       *
     * ---------------------------------------------------------------------- */

        //accAngulaire = (wb - wa) / tb;

        ta_average = (inertieTimes.at(i - 2) + inertieTimes.at(i - 1)) / 2;
        tb_average = (inertieTimes.at(i - 1) + inertieTimes.at(i)) / 2;

        angularAcceleration = (angularSpeed_b - angularSpeed_a)
                                              /
                                  (tb_average - ta_average);

    /* ---------------------------------------------------------------------- *
     *                              C = JΔ * α                                *
     * ---------------------------------------------------------------------- *
     * C  = Couple (N.m)                                                      *
     * JΔ = moment d'inertie (kg.m²)                                          *
     * α  = Accélération angulaire (rad/s²)                                   *
     * ---------------------------------------------------------------------- */

        couple = Jdelta * angularAcceleration;

    /* ---------------------------------------------------------------------- *
     *                               P = C * ωb                               *
     * ---------------------------------------------------------------------- *
     * P  = Puissance (Watts)                                                 *
     * ωx = Vitesse angulaire à l'instant x (rad/s)                           *
     * ---------------------------------------------------------------------- */

        power = couple * angularSpeed_b;

    /* ---------------------------------------------------------------------- *
     *                   Calcul de la Puissance spécifique                    *
     * ---------------------------------------------------------------------- */

        /* Récupère l'indice de la ligne de données MS correspondant au temps
         * juste inférieur à t3 */
        while(indiceMS + 1 < this->benchParser.rowCount()
              && this->benchParser.row(indiceMS + 1).at(0).toDouble() / 1000000
                 < inertieTimes.at(i))
            ++indiceMS;

        qDebug() << "indice Megasquirt = " << indiceMS;
        qDebug() << "temps inertie (s)          = " << inertieTimes.at(i);
        qDebug() << "temps MS correspondant (s) = " << this->benchParser.row(indiceMS).at(0).toDouble() / 1000000;

    /* ---------------------------------------------------------------------- *
     * Temps d'injection = PW1 - (deadTime + (voltCorr *(13,2 - battVolt)))   *
     * ---------------------------------------------------------------------- */

        double pw       = this->benchParser.row(indiceMS).at(1).toDouble(); // ms
        double battVolt = this->benchParser.row(indiceMS).at(3).toDouble(); // v
        double tempsInjection = pw - (deadTime + (voltCorr *(13.2 - battVolt)));  // ms

        qDebug() << "pw (ms)      = " << pw;
        qDebug() << "battVolt (v) = " << battVolt;
        qDebug() << "temps d'injection (ms) = " << tempsInjection;

    /* ---------------------------------------------------------------------- *
     *       Puissance spécifique = (3600000 * quantité injectée)             *
     *                                         /                              *
     *                            (4 * π * couple * 1000)                     *
     * ---------------------------------------------------------------------- */
        double quantiteInjectee = injectorFlowRate * tempsInjection;
        qDebug() << "Quantité injectée = " << quantiteInjectee;

        specificPower = (3600000 * quantiteInjectee) / (4 * PI * couple * 1000);
        qDebug() << "puissance specifique = " << specificPower;

    /* ---------------------------------------------------------------------- *
     *               ωx = (π * Nx) / 30  <=> Nx = (30 * ωx) / π               *
     * ---------------------------------------------------------------------- *
     * ω = Vitesse angulaire (rad/s)                                          *
     * Π = Pi, constante qui vaut 3.141592653589793...                        *
     * N = tours par minute == RPM (tours/minute)                             *
     * ---------------------------------------------------------------------- */

        double rpm_b = (30 * angularSpeed_b) / PI;

        // create curves coordinates
        powerPoints.append(QPointF(rpm_b, power));
        couplePoints.append(QPointF(rpm_b, couple));
        specificPowerPoints.append(QPointF(rpm_b, specificPower));

        reductionRatioPoints1.append(
                   QPointF(rpm_b,
                           benchParser.row(indiceMS).at(2).toDouble() / rpm_b));
        reductionRatioPoints2.append(
                   QPointF(benchParser.row(indiceMS).at(2).toDouble(),
                           benchParser.row(indiceMS).at(2).toDouble() / rpm_b));

    }

    // Create power curve
    QwtPointSeriesData* powerSerieData = new QwtPointSeriesData(powerPoints);
    PlotCurve* powerCurve = new PlotCurve(
                tr("Puissance ") + param.testName(), QPen(Qt::darkBlue));
    powerCurve->setData(powerSerieData);
    powerCurve->setAxes(Plot::xTop, Plot::yRight);
    powerCurve->attach(this->couplePowerPlot);
    this->setPlotCurveVisibile(powerCurve, true);

    // Create specificPower curve
    QwtPointSeriesData* specificPowerSerieData =
            new QwtPointSeriesData(specificPowerPoints);
    PlotCurve* specificPowerCurve = new PlotCurve(
              tr("Puissance spécifique ") + param.testName(),QPen(Qt::darkBlue));
    specificPowerCurve->setData(specificPowerSerieData);
    specificPowerCurve->setAxes(Plot::xTop, Plot::yRight);
    specificPowerCurve->attach(this->coupleSpecificPowerPlot);
    this->setPlotCurveVisibile(specificPowerCurve, true);

    // Create couple curve for couplePowerPlot
    QwtPointSeriesData* coupleSerieData1 = new QwtPointSeriesData(couplePoints);
    PlotCurve* coupleCurve1 = new PlotCurve(
                tr("Couple ") + param.testName(), QPen(Qt::darkRed));
    coupleCurve1->setData(coupleSerieData1);
    coupleCurve1->attach(this->couplePowerPlot);
    this->setPlotCurveVisibile(coupleCurve1, true);
    this->couplePowerPlot->zoom(powerCurve);

    // Create couple curve for coupleSpecificPowerPlot
    QwtPointSeriesData* coupleSerieData2 = new QwtPointSeriesData(couplePoints);
    PlotCurve* coupleCurve2 = new PlotCurve(
                tr("Couple ") + param.testName(), QPen(Qt::darkRed));
    coupleCurve2->setData(coupleSerieData2);
    coupleCurve2->attach(this->coupleSpecificPowerPlot);
    this->setPlotCurveVisibile(coupleCurve2, true);
    this->coupleSpecificPowerPlot->zoom(coupleCurve2);

    // Create reduction ratio curve for reductionRatioPlot
    QwtPointSeriesData* reductionRatioSerieData1 = new QwtPointSeriesData(reductionRatioPoints1);
    PlotCurve* reductionRatioCurve1 = new PlotCurve(
                tr("i rouleau ") + param.testName(), QPen(Qt::darkBlue));
    reductionRatioCurve1->setData(reductionRatioSerieData1);
    reductionRatioCurve1->setAxes(Plot::xTop, Plot::yLeft);
    reductionRatioCurve1->attach(this->reductionRatioPlot);
    this->setPlotCurveVisibile(reductionRatioCurve1, true);
    this->reductionRatioPlot->zoom(reductionRatioCurve1);

    // Create reduction ratio curve for reductionRatioPlot
    QwtPointSeriesData* reductionRatioSerieData2 = new QwtPointSeriesData(reductionRatioPoints2);
    PlotCurve* reductionRatioCurve2 = new PlotCurve(
                tr("i moteur ") + param.testName(), QPen(Qt::darkRed));
    reductionRatioCurve2->setData(reductionRatioSerieData2);
    reductionRatioCurve2->attach(this->reductionRatioPlot);
    this->setPlotCurveVisibile(reductionRatioCurve2, true);
    this->reductionRatioPlot->zoom(reductionRatioCurve2);
}

void MainWindow::on_actionImportData_triggered(void)
{
    try
    {
        // Check if the folder contents all necessary data files
        QDir MSDir = getMegasquirtDataFolder();

        // Set import parameters
        MSDataParameterDialog importParamDial(MSDir.dirName(), this);
        if (importParamDial.exec() == QDialog::Rejected)
            return;

        // Get all file paths
        QSettings settings;
        settings.beginGroup("files");

        QString msCSVFilePath      = MSDir.filePath(
                    settings.value(KEY_MEGASQUIRT_CSV).toString());
        QString msDatFilePath      = MSDir.filePath(
                    settings.value(KEY_MEGASQUIRT_DAT).toString());
        QString inertieFilePath    = MSDir.filePath(
                    settings.value(KEY_INERTIE).toString());
        QString protoWheelFilePath = MSDir.filePath(
                    settings.value(KEY_PROTOWHEEL).toString());

        settings.endGroup();

    /* ---------------------------------------------------------------------- *
     *                           Get inertie times                            *
     * ---------------------------------------------------------------------- */
        QVector<double> inertieTimes;
        this->getTimesFromCSV(inertieTimes, inertieFilePath);

    /* ---------------------------------------------------------------------- *
     *                         Get proto wheel times                          *
     * ---------------------------------------------------------------------- */
//        QVector<double> protoWheelTimes;
//        this->getTimesFromCSV(protoWheelTimes, protoWheelFilePath);

//        qDebug() << "Nombre de données proto wheel = " << protoWheelTimes.count();

    /* ---------------------------------------------------------------------- *
     *       Generate csv file by extracting needed data from dat file        *
     *                    and read data from csv file                         *
     * ---------------------------------------------------------------------- */
        QStringList megasquirtParameters;
        megasquirtParameters << "pulseWidth1" << "rpm" << "batteryVoltage";

        MSManager megasquirtManager;
        megasquirtManager.datToCSV(msDatFilePath, msCSVFilePath,
                                   megasquirtParameters);

        this->benchParser.parse(msCSVFilePath, ';', QString::SkipEmptyParts);

    /* ---------------------------------------------------------------------- *
     *                             Create curves                              *
     * ---------------------------------------------------------------------- */
        this->createCoupleAndPowerCurves(inertieTimes, importParamDial);
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Importation annulée"), ex.what());
    }

    // Free the megasquirt data from memory
    this->benchParser.reset();
}

void MainWindow::on_actionQuit_triggered(void)
{
    // Save the state of the mainWindow and its widgets
    this->writeSettings();

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
        QCSVRow secondes = this->MSPlotParser["times"];

        // Récupération de la colonne de données a représenter par une courbe
        QCSVRow data     = this->MSPlotParser[curveName];

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
        PlotCurve* curve = new PlotCurve(curveName, QPen(Qt::darkRed, 1));
        curve->setData(serieData);
        curve->attach(this->megasquirtDataPlot);

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
    this->updateMenus(); // Because two menu actions must been (un)checked
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

    if (filename.isEmpty())
        return;

    try
    {
        // Load information from the CSV file
        this->MSPlotParser.parse(filename, ';', QString::SkipEmptyParts);

        // Récupérer les noms des colonnes du fichier csv
        this->ui->paramMegasquirtComboBox->clear();
        this->ui->paramMegasquirtComboBox->addItems(this->MSPlotParser.headersList());
    }
    catch (QException const& ex)
    {
        QMessageBox::warning(this, tr("Importation annulée"), ex.what());
    }
}

void MainWindow::on_actionDatToCSV_triggered(void)
{
    try
    {
        /* Get dat file name */
        QString datFileName = QFileDialog::getOpenFileName(
                    this, tr("Selectionné le fichier de données du Megasquirt"),
                    QDir::homePath(), tr("Fichier de données (*.dat)"));

        if (datFileName.isEmpty()) // User canceled
            return;

        MSFileConverterDialog dialog(datFileName, this);
        dialog.exec();
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Conversion annulée"), ex.what());
    }
}

void MainWindow::on_actionConfigureMegasquirtFileName_triggered(void)
{
    MSFileParameterDialog dial(this);
    dial.exec();
}

void MainWindow::on_actionExportToPDF_triggered(void)
{
    QString pdfFile = QFileDialog::getSaveFileName(
                this, tr("Sauvegarder le graphique"), QDir::homePath(),
                tr("Portable Document Format (*.pdf)"));

    if (pdfFile.isNull() || pdfFile.isEmpty()) // User canceled
        return;

    QwtPlotRenderer renderer;
    renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground);
    renderer.renderDocument(this->currentPlot(), pdfFile,
                            this->currentPlot()->size());
}

void MainWindow::eraseCurve(void)
{
    // if no curve associated to the legend item. This shouldn't happen!
    if (this->curveAssociatedToLegendItem == NULL)
        return;

    // Delete the curve associated to the legend item
    this->curveAssociatedToLegendItem->detach();
    delete this->curveAssociatedToLegendItem;
    this->curveAssociatedToLegendItem = NULL;

    // update the plot
    this->currentPlot()->replot();
}

void MainWindow::centerOnCurve(void)
{
    // if no curve associated to the legend item. This shouldn't happen!
    if (this->curveAssociatedToLegendItem == NULL)
        return;

    this->currentPlot()->zoom(this->curveAssociatedToLegendItem);
}

void MainWindow::changeCurveColor(void)
{
    // if no curve associated to the legend item. This shouldn't happen!
    if (this->curveAssociatedToLegendItem == NULL)
        return;

    // Select a new color
    QColor newColor = QColorDialog::getColor(
                this->curveAssociatedToLegendItem->pen().color(), this,
                tr("Choisir une nouvelle couleur pour la courbe"));

    // If the user cancels the dialog, an invalid color is returned
    if (newColor.isValid())
        this->curveAssociatedToLegendItem->setPen(QPen(newColor));
}

void MainWindow::createPolynomialTrendline(void)
{
    // if no curve associated to the legend item. This shouldn't happen!
    if (this->curveAssociatedToLegendItem == NULL)
        return;

    bool ok(false);
    int degree = QInputDialog::getInt(
                this, tr("Courbe de tendance polynomiale"),
                tr("Ordre de complexité ?"), 2, 2, 100, 1, &ok);

    if (!ok) // User canceled
        return;

    // Récupération de la série des points de la courbe
    QwtPointSeriesData* curveSeriesData =
            (QwtPointSeriesData*)this->curveAssociatedToLegendItem->data();
    if (!curveSeriesData)
        return;

    // Récupération de la liste des points de la courbe
    QVector<QPointF> curvePoints(curveSeriesData->samples());

    // Calcul de tous les coefficients de l'équation
    QVector<double> coefficients(polynomialfit(curvePoints, degree));

    // Création de la liste des points de la courbe de tendance
    for(int i(0); i < curvePoints.count(); ++i)
    {
        // Calcul de la nouvelle valeur de y
        double y(0);
        for(int j(0); j < coefficients.count(); ++j)
            y += coefficients.at(j) * qPow(curvePoints.at(i).x(), j);

        curvePoints[i].setY(y);
    }

    // Création de la courbe
    QwtPointSeriesData* trendlineSeriesData = new QwtPointSeriesData(curvePoints);
    PlotCurve* trendlineCurve = new PlotCurve(
                this->curveAssociatedToLegendItem->title().text() +
                tr(" Poly(") + QString::number(degree) + ")",
                this->curveAssociatedToLegendItem->pen());
    trendlineCurve->setAxes(this->curveAssociatedToLegendItem->xAxis(),
                            this->curveAssociatedToLegendItem->yAxis());
    trendlineCurve->setData(trendlineSeriesData);
    trendlineCurve->attach(this->curveAssociatedToLegendItem->plot());
    this->setPlotCurveVisibile(trendlineCurve, true);
}

void MainWindow::setPlotCurveVisibile(QwtPlotItem* item, bool visible)
{
    item->setVisible(visible);
    QWidget* w = item->plot()->legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(visible);

    item->plot()->replot();
}

void MainWindow::showLegendContextMenu(const QwtPlotItem* item,
                                       const QPoint& pos)
{
    // Save the plot curve associated to the legend item
    this->curveAssociatedToLegendItem = (PlotCurve*) item;
    if(!this->curveAssociatedToLegendItem)
        return;

    // Display custom contextual menu
    this->legendContextMenu->exec(pos);
}
