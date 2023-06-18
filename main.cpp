
#include "heightindicatorwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    HeightIndicatorWidget heightIndicatorWidget;

    heightIndicatorWidget.show();

    return application.exec();
}
