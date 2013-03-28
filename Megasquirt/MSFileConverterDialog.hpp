#ifndef __MSFILECONVERTERDIALOG_HPP__
#define __MSFILECONVERTERDIALOG_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "MSManager.hpp"

namespace Ui {
class MSFileConverterDialog;
}

class MSFileConverterDialog : public QDialog
{
    Q_OBJECT
    
    public:

        explicit MSFileConverterDialog(QString const& datFilename,
                                       QWidget* parent = NULL);
        virtual ~MSFileConverterDialog(void);

    private slots:

            void on_convertPushButton_clicked(void);

    protected:

        Ui::MSFileConverterDialog* ui;

        QString   datFilename;
        MSManager megasquirtManager;

};

#endif /* __MSFILECONVERTERDIALOG_HPP__ */
