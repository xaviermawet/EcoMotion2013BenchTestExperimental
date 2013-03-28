#include "MSFileConverterDialog.hpp"
#include "ui_MSFileConverterDialog.h"

MSFileConverterDialog::MSFileConverterDialog(const QString& datFilename,
                                             QWidget* parent) :
    QDialog(parent), ui(new Ui::MSFileConverterDialog),
    datFilename(datFilename), megasquirtManager()
{
    // Display Configuration
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    // GUI Configuration
    this->ui->setupUi(this);

    if (this->datFilename.isEmpty())
        throw QException(tr("Aucun fichier de données sélectionné"));

    QFileInfo datFile(this->datFilename);
    if (datFile.suffix().toUpper() != "DAT")
        throw QException(tr("Fichier de données non valide"));

    // fill the list with all megasquirt parameters
    this->ui->listParamMegasquirt->addItems(
                QStringList(this->megasquirtManager.fields()));
}

MSFileConverterDialog::~MSFileConverterDialog(void)
{
    delete this->ui;
}

void MSFileConverterDialog::on_convertPushButton_clicked(void)
{
    QList<QListWidgetItem*> itemsSelected =
            this->ui->listParamMegasquirt->selectedItems();

    try
    {
        /* Get the name of all parameters to convert */
        QList<QString> listParam;
        foreach (QListWidgetItem* item, itemsSelected)
            listParam << item->text();

        /* Get CSV file name */
        QString csvFileName = QFileDialog::getSaveFileName(
                    this, tr("Choisir ou enregistrer les données converties"),
                    QDir::homePath(), tr("Fichier CSV (*.csv)"));

        if (csvFileName.isEmpty())
            throw QException("Aucun fichier de sauvegade renseigné");

        /* delete CSV if already exists */
        QFile csvFile(csvFileName);
        if (csvFile.exists())
            csvFile.remove();

        megasquirtManager.datToCSV(this->datFilename, csvFileName, listParam);

        QMessageBox::information(this, tr("Conversion terminée"),
                                 tr("Conversion terminée avec succès"));
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Conversion annulée"),
                             ex.what());
    }
}
