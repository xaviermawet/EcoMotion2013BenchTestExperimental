#ifndef __MSDATAPARAMETERDIALOG_HPP__
#define __MSDATAPARAMETERDIALOG_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

namespace Ui {
class MSDataParameterDialog;
}

class MSDataParameterDialog : public QDialog
{
    Q_OBJECT
    
    public:

        explicit MSDataParameterDialog(
            QString const& defaultTestName = QString(), QWidget* parent = NULL);
        virtual ~MSDataParameterDialog(void);

        // Getters
        double  inertia(void) const;
        QString fuelType(void) const;
        QString testName(void) const;

        // Setters
        void setInertia(double inertia);
        void setFuelType(QStringList const& fuelTypeList);
        void addFuelType(QString const& fuelType);
        void setTestName(QString const& testName);

    protected:

        Ui::MSDataParameterDialog* ui;
};

#endif /* __MSDATAPARAMETERDIALOG_HPP__ */
