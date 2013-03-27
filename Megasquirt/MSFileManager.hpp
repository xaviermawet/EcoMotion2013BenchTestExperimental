#ifndef __MSFILEMANAGER_HPP__
#define __MSFILEMANAGER_HPP__

#if QT_VERSION >= 0x050000 //  0xMMNNPP (MM = major, NN = minor, PP = patch)
    #include <QtWidgets>
#else
    #include <QtGui>
#endif

#include "MSDataConverter.hpp"
#include "../Utils/QException.hpp"
#include "../Utils/QCSVParser.hpp"

#define DATA_SIZE	169
#define TIME_SIZE   4 // 32 bits

class MSFileManager
{
    public:

        MSFileManager(void);

        // Getters
        int count(void) const;
        int fieldIndice(QString const& field) const;
        QList<QString> fields(void) const;

        const MSDataConverter& dataConverter(int fieldIndice) const;
        const MSDataConverter& dataConverter(QString const& field) const;

        const MSDataConverter& operator[](QString const& field) const;
        const MSDataConverter& operator[](int fieldIndice) const;

        // File operations
        void datToCSV(QString const& datFile, QString const& CSVFile) const;
        void datToCSV(const QString &datFile, const QString &CSVFile,
                      QList<QString> const& fieldList) const;

    protected:

        QList<MSDataConverter> dataConverters;
};

#endif /* __MSFILEMANAGER_HPP__ */
