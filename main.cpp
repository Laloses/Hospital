#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QTranslator qtTranslator;
    if (qtTranslator.load(QLocale::system(),
                    "qt", "_",
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            a.installTranslator(&qtTranslator);
        }

        QTranslator qtBaseTranslator;
        if (qtBaseTranslator.load("qtbase_" + QLocale::system().name(),
                    QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
            a.installTranslator(&qtBaseTranslator);
        }

    return a.exec();
}
