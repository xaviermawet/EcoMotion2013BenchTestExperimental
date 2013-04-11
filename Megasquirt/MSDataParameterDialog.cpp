#include "MSDataParameterDialog.hpp"
#include "ui_MSDataParameterDialog.h"

MSDataParameterDialog::MSDataParameterDialog(QWidget *parent) :
    MSDataParameterDialog(QString(), parent)
{
    // Delegating constructors only available with -std=c++11 or -std=gnu++11
}

MSDataParameterDialog::MSDataParameterDialog(const QString& defaultTestName,
                                             QWidget* parent) :
    QDialog(parent), ui(new Ui::MSDataParameterDialog)
{
    qDebug() << "Construction de la fenetre de paramétrisation de l'importation";

    // GUI Configuration
    this->ui->setupUi(this);

    // Set default
    this->ui->testNameLineEdit->setText(defaultTestName);

    // Restore previous settings
    this->readSettings();
}

MSDataParameterDialog::~MSDataParameterDialog(void)
{
    qDebug() << "Destruction de la fenetre de paramétrisation de l'importation";

    delete this->ui;
}

QString MSDataParameterDialog::testName(void) const
{
    return this->ui->testNameLineEdit->text();
}

double MSDataParameterDialog::inertia(void) const
{
    return this->ui->inertieDoubleSpinBox->value();
}

double MSDataParameterDialog::rollerPerimeter(void) const
{
    return this->ui->rollerPerimeterDoubleSpinBox->value();
}

double MSDataParameterDialog::deadTime(void) const
{
    return this->ui->deadTimeDoubleSpinBox->value();
}

double MSDataParameterDialog::voltageCorrection(void) const
{
    return this->ui->voltageCorrectionDoubleSpinBox->value();
}

double MSDataParameterDialog::protoWheelPerimeter(void) const
{
    return this->ui->protoWheelPerimeterDoubleSpinBox->value();
}

double MSDataParameterDialog::injectorVolumetricFlowRate(void) const
{
    return this->ui->injectorVolumetricFlowRateDoubleSpinBox->value();
}

QString MSDataParameterDialog::fuelName(void) const
{
    return this->ui->fuelTypeComboBox->currentText();
}

double MSDataParameterDialog::fuelDensity(void) const
{
    //return this->_fuels.value(this->fuelName());
    return this->ui->fuelDensityDoubleSpinBox->value();
}
void MSDataParameterDialog::setTestName(const QString &testName)
{
    this->ui->testNameLineEdit->setText(testName);
}

void MSDataParameterDialog::setInertia(double inertia)
{
    this->ui->inertieDoubleSpinBox->setValue(inertia);
}

void MSDataParameterDialog::setRollerPerimeter(double perimeter)
{
    this->ui->rollerPerimeterDoubleSpinBox->setValue(perimeter);
}

void MSDataParameterDialog::setDeadTime(double deadTime)
{
    this->ui->deadTimeDoubleSpinBox->setValue(deadTime);
}

void MSDataParameterDialog::setVoltageCorrection(double voltageCorrection)
{
    this->ui->voltageCorrectionDoubleSpinBox->setValue(voltageCorrection);
}

void MSDataParameterDialog::setProtoWheelPerimeter(double protoWheelPerimeter)
{
    this->ui->protoWheelPerimeterDoubleSpinBox->setValue(protoWheelPerimeter);
}

void MSDataParameterDialog::setInjectorVolumetricFlowRate(
        double volumetricFlowRate)
{
    this->ui->injectorVolumetricFlowRateDoubleSpinBox->setValue(
                volumetricFlowRate);
}

/* If there is already an item with this fuel,
 * that item's density is replaced by the new value */
void MSDataParameterDialog::addFuel(const QString &name, double density)
{
    this->_fuels.insert(name, density);
}

void MSDataParameterDialog::readSettings(void)
{
    QSettings settings;

    settings.beginGroup("MSDataParameterDialog");

    this->setInertia(settings.value("inertia", 0.0).toDouble());
    this->setRollerPerimeter(settings.value("rollerPerimeter", 0.0).toDouble());
    this->setDeadTime(settings.value("deadTime", 0.0).toDouble());
    this->setVoltageCorrection(
                settings.value("voltageCorrection", 0.0).toDouble());
    this->setProtoWheelPerimeter(
                settings.value("protoWheelPerimeter", 0.0).toDouble());
    this->setInjectorVolumetricFlowRate(
                settings.value("injectorVolumetricFlowRate", 0.0).toDouble());

    // Restore fuel list
    settings.beginGroup("fuel");
    QStringList fuelNames = settings.childKeys();
    foreach (QString fuel, fuelNames)
        this->_fuels[fuel] = settings.value(fuel).toDouble();
    settings.endGroup();

    settings.endGroup();

    // Update fuel combobox
    this->ui->fuelTypeComboBox->clear();
    this->ui->fuelTypeComboBox->addItems(fuelNames);
}

void MSDataParameterDialog::writeSettings(void) const
{
    qDebug() << "Sauvegarde des paramètres d'importation des données Megasquirt";

    QSettings settings;
    settings.beginGroup("MSDataParameterDialog");

    settings.setValue("inertia", this->inertia());
    settings.setValue("rollerPerimeter", this->rollerPerimeter());
    settings.setValue("deadTime", this->deadTime());
    settings.setValue("voltageCorrection", this->voltageCorrection());
    settings.setValue("protoWheelPerimeter", this->protoWheelPerimeter());
    settings.setValue("injectorVolumetricFlowRate",
                      this->injectorVolumetricFlowRate());

        // Save fuel list
        settings.beginGroup("fuel");
        settings.remove(""); //  remove all entries in the current group
        QMap<QString, double>::const_iterator i = this->_fuels.constBegin();
        while(i != this->_fuels.constEnd())
        {
            settings.setValue(i.key(), i.value());
            ++i;
        }
        settings.endGroup();

    settings.endGroup();
}

void MSDataParameterDialog::on_addFuelPushButton_clicked(void)
{
    bool ok(false);
    QString fuelName;
    double fuelDensity;

    /* ---------------------------------------------------------------------- *
     *                             Get fuel name                              *
     * ---------------------------------------------------------------------- */

    // If the fuel has no name or if already exists
    while(!ok)
    {
        fuelName = QInputDialog::getText(
                    this, tr("Ajout d'un type de carburant"),
                    tr("Nouveau type de carburant ?"), QLineEdit::Normal,
                    QString(), &ok);

        if (!ok) // User canceled
            return;

        if (fuelName.isEmpty())
        {
            QMessageBox::warning(this, tr("Carburant non valide"),
                                 tr("Vous devez entrer un nom de carburant"));
            ok = false;
        }
        else if (this->_fuels.contains(fuelName))
        {
            QMessageBox::critical(this, tr("Carburant non valide"),
                                  tr("Ce carburant existe déjà"));
            ok = false;
        }
    }

    /* ---------------------------------------------------------------------- *
     *                            Get fuel density                            *
     * ---------------------------------------------------------------------- */
    fuelDensity = QInputDialog::getDouble
                  (
                    this, tr("Ajout d'un carburant"), tr("Masse volumique de ")
                    + fuelName + " (g/l)",
                    this->ui->fuelDensityDoubleSpinBox->minimum(),
                    this->ui->fuelDensityDoubleSpinBox->minimum(),
                    this->ui->fuelDensityDoubleSpinBox->maximum(),
                    this->ui->fuelDensityDoubleSpinBox->decimals(), &ok
                   );

    if (!ok) // User canceled
        return;

    // Add fuel
    this->_fuels.insert(fuelName, fuelDensity);
    this->ui->fuelTypeComboBox->addItem(fuelName);
}

void MSDataParameterDialog::on_deleteFuelPushButton_clicked(void)
{
    // delete fuel from QMap
    this->_fuels.remove(this->fuelName());

    // delete fuel from combobox
    this->ui->fuelTypeComboBox->removeItem(
                this->ui->fuelTypeComboBox->currentIndex());
}

void MSDataParameterDialog::on_fuelTypeComboBox_currentIndexChanged(
        QString const& fuel)
{
    this->ui->fuelDensityDoubleSpinBox->setValue(this->_fuels.value(fuel));
}

void MSDataParameterDialog::on_buttonBox_accepted(void)
{
    try
    {
        if (this->inertia() <= 0.0)
            throw QException(tr("L'inertie doit etre supérieure à 0"));

        if (this->rollerPerimeter() <= 0.0)
            throw QException(tr("Le périmètre du rouleau doit etre supérieur à 0"));

        if (this->deadTime() <= 0.0)
            throw QException(tr("Le deadTime doit etre supérieur à 0"));

        if (this->voltageCorrection() <= 0.0)
            throw QException(tr("Le voltage correction doit etre supérieur à 0"));

        if (this->protoWheelPerimeter() <= 0.0)
            throw QException(tr("Le périmètre de la roue du prototype doit etre supérieure à 0"));

        if (this->injectorVolumetricFlowRate() <= 0)
            throw QException(tr("Le débit de l'injecteur doit etre supérieur à 0"));

        if (this->fuelName().isEmpty())
            throw QException(tr("Vous devez choisir un type de carburant"));

        // Save the state of the mainWindow and its widgets
        this->writeSettings();

        // Close the dialog
        QDialog::accept();
    }
    catch(QException const& ex)
    {
        QMessageBox::warning(this, tr("Paramètres non valides"), ex.what());
    }
}

void MSDataParameterDialog::on_fuelDensityDoubleSpinBox_editingFinished(void)
{
    // Save the new density value
    this->_fuels[this->fuelName()] = this->fuelDensity();
}
