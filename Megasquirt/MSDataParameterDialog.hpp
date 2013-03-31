#ifndef MSDATAPARAMETERDIALOG_HPP
#define MSDATAPARAMETERDIALOG_HPP

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

        explicit MSDataParameterDialog(QWidget* parent = NULL);
        virtual ~MSDataParameterDialog(void);

    protected:

        Ui::MSDataParameterDialog* ui;
};

#endif // MSDATAPARAMETERDIALOG_HPP
