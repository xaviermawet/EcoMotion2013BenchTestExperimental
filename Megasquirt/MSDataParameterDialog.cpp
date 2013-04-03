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
