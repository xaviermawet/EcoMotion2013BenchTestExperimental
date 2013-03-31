#include "MSDataParameterDialog.hpp"
#include "ui_MSDataParameterDialog.h"

MSDataParameterDialog::MSDataParameterDialog(QWidget* parent) :
    QDialog(parent), ui(new Ui::MSDataParameterDialog)
{
    // GUI Configuration
    this->ui->setupUi(this);
}

MSDataParameterDialog::~MSDataParameterDialog(void)
{
    delete this->ui;
}
