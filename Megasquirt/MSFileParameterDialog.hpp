#ifndef __MSFILEPARAMETERDIALOG_HPP__
#define __MSFILEPARAMETERDIALOG_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#define KEY_INERTIE        "Inertie"
#define KEY_PROTOWHEEL     "ProtoWheel"
#define KEY_MEGASQUIRT_DAT "MegasquirtDAT"
#define KEY_MEGASQUIRT_CSV "MegasquirtCSV"

namespace Ui {
class MSFileParameterDialog;
}

class MSFileParameterDialog : public QDialog
{
    Q_OBJECT
    
    public:

        explicit MSFileParameterDialog(QWidget* parent = NULL);
        virtual ~MSFileParameterDialog(void);

    protected:

        void  readSettings(void);
        void  writeSettings(void) const;

    private slots:

        void on_pushButton_clicked(void);

    protected:

        Ui::MSFileParameterDialog* ui;
};

#endif /* __MSFILEPARAMETERDIALOG_HPP__ */
