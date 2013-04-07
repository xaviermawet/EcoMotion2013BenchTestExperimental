#include "MSDataParameterDialog.hpp"
#include "ui_MSDataParameterDialog.h"

MSDataParameterDialog::MSDataParameterDialog(const QString& defaultTestName,
                                             QWidget* parent) :
    QDialog(parent), ui(new Ui::MSDataParameterDialog)
{
    // GUI Configuration
    this->ui->setupUi(this);

    // Set default
    this->ui->testNameLineEdit->setText(defaultTestName);
}

MSDataParameterDialog::~MSDataParameterDialog(void)
{
    delete this->ui;
}

double MSDataParameterDialog::inertia(void) const
{
    return this->ui->inertieDoubleSpinBox->value();
}

QString MSDataParameterDialog::fuelType(void) const
{
    return this->ui->fuelTypeLabelComboBox->currentText();
}

QString MSDataParameterDialog::testName(void) const
{
    return this->ui->testNameLineEdit->text();
}

void MSDataParameterDialog::setInertia(double inertia)
{
    this->ui->inertieDoubleSpinBox->setValue(inertia);
}

void MSDataParameterDialog::setFuelType(const QStringList &fuelTypeList)
{
    this->ui->fuelTypeLabelComboBox->clear();
    this->ui->fuelTypeLabelComboBox->addItems(fuelTypeList);
}

void MSDataParameterDialog::addFuelType(const QString &fuelType)
{
    this->ui->fuelTypeLabelComboBox->addItem(fuelType);
}

void MSDataParameterDialog::setTestName(const QString &testName)
{
    this->ui->testNameLineEdit->setText(testName);
}
