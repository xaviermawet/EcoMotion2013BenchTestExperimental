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
        double  inertia(void) const;
        QString fuelName(void) const;
        double  fuelDensity(void) const;
        QString testName(void) const;

        // Setters
        void setInertia(double inertia);
        void setTestName(QString const& testName);
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

        QMap<QString, double> _fuels; // fuel name - fuel density (g/cm³)
};

#endif /* __MSDATAPARAMETERDIALOG_HPP__ */
