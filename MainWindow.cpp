#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), plot(NULL), parser()
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Plot configuration
    this->createPlotZone();

    // Display Configuration
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    this->centerOnScreen();
}

MainWindow::~MainWindow(void)
{
    qDebug() << "MainWindow début destructeur";

    delete this->ui;
    delete this->plot;

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

void MainWindow::createPlotZone(void)
{
    this->plot = new Plot("EcoMotion 2013 - Banc d'essai", this);
    this->ui->mainHorizontalLayout->addWidget(this->plot);

    // Update menu actions
    this->ui->actionShowGrid->setChecked(this->plot->isGridVisible());
    this->ui->actionShowCrossLine->setChecked(this->plot->isCrossLineVisible());

    // Connect plot signals to slots
    connect(this->plot, SIGNAL(legendChecked(QwtPlotItem*, bool)),
            this,  SLOT(setPlotCurveVisibile(QwtPlotItem*, bool)));

    // Connect signals to plot slots
    connect(this->ui->actionShowGrid, SIGNAL(triggered(bool)),
            this->plot, SLOT(setGridVisible(bool)));
    connect(this->ui->actionShowCrossLine, SIGNAL(triggered(bool)),
            this->plot, SLOT(setCrossLineVisible(bool)));
}

void MainWindow::on_actionImportData_triggered(void)
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

void MainWindow::on_actionQuit_triggered(void)
{
    // Close the main window
    qApp->quit();
}

void MainWindow::on_addCurvePushButton_clicked(void)
{
    QMessageBox::information(this, "", "afficher courbe");
}

void MainWindow::setPlotCurveVisibile(QwtPlotItem* item, bool visible)
{
    item->setVisible(visible);
    QWidget* w = item->plot()->legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(visible);

    item->plot()->replot();
}
