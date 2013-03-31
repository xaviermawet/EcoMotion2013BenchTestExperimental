#include "MSFileParameterDialog.hpp"
#include "ui_MSFileParameterDialog.h"

MSFileParameterDialog::MSFileParameterDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::MSFileParameterDialog)
{
    // Gui Configuration
    this->ui->setupUi(this);

    // Load file information from Settings
    this->readSettings();
}

MSFileParameterDialog::~MSFileParameterDialog(void)
{
    delete this->ui;
}

void MSFileParameterDialog::readSettings(void)
{
    QSettings settings;

    settings.beginGroup("files");

    this->ui->megasquirtDatLineEdit->setText(
                settings.value(KEY_MEGASQUIRT_DAT).toString());
    this->ui->megasquirtCSVLineEdit->setText(
                settings.value(KEY_MEGASQUIRT_CSV).toString());
    this->ui->protoWheelLineEdit->setText(
                settings.value(KEY_PROTOWHEEL).toString());
    this->ui->inertieLineEdit->setText(
                settings.value(KEY_INERTIE).toString());

    settings.endGroup();
}

void MSFileParameterDialog::writeSettings(void) const
{
    QSettings settings;

    settings.beginGroup("files");

    settings.setValue(KEY_MEGASQUIRT_DAT,
                      this->ui->megasquirtDatLineEdit->text());
    settings.setValue(KEY_MEGASQUIRT_CSV,
                      this->ui->megasquirtCSVLineEdit->text());
    settings.setValue(KEY_PROTOWHEEL,
                      this->ui->protoWheelLineEdit->text());
    settings.setValue(KEY_INERTIE,
                      this->ui->inertieLineEdit->text());

    settings.endGroup();
}

void MSFileParameterDialog::on_pushButton_clicked(void)
{
    // Save the settings (file names)
    this->writeSettings();

    // Close the dialog
    this->accept();
}
