#ifndef __MSDATAPARAMETERDIALOG_HPP__
#define __MSDATAPARAMETERDIALOG_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "../Utils/QException.hpp"

namespace Ui {
class MSDataParameterDialog;
}

class MSDataParameterDialog : public QDialog
{
    Q_OBJECT
    
    public:

        explicit MSDataParameterDialog(QWidget* parent = NULL);
        explicit MSDataParameterDialog(QString const& defaultTestName,
                                       QWidget* parent = NULL);
        virtual ~MSDataParameterDialog(void);

        // Getters
        QString testName(void) const;
        double  inertia(void) const;
        double  rollerPerimeter(void) const;
        double  deadTime(void) const;
        double  voltageCorrection(void) const;
        double  protoWheelPerimeter(void) const;
        QString fuelName(void) const;
        double  fuelDensity(void) const;

        // Setters
        void setTestName(QString const& testName);
        void setInertia(double inertia);
        void setRollerPerimeter(double perimeter);
        void setDeadTime(double deadTime);
        void setVoltageCorrection(double voltageCorrection);
        void setProtoWheelPerimeter(double protoWheelPerimeter);
        void addFuel(QString const& name, double density);

    protected:

        void readSettings(void);
        void writeSettings(void) const;

    private slots:

        void on_addFuelPushButton_clicked(void);
        void on_deleteFuelPushButton_clicked(void);
        void on_fuelTypeComboBox_currentIndexChanged(QString const& fuel);
        void on_buttonBox_accepted(void);
        void on_fuelDensityDoubleSpinBox_editingFinished(void);

    protected:

        Ui::MSDataParameterDialog* ui;

        QMap<QString, double> _fuels; // fuel name - fuel density (g/l)
};

#endif /* __MSDATAPARAMETERDIALOG_HPP__ */
