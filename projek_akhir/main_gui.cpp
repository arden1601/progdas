#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Car Rental Management System");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("CarRental");

    // Set application style
    app.setStyle("Fusion");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}