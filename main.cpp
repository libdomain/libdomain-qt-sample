#include "mainwindow.h"
#include "connectiondialog.h"
#include <QApplication>
#include <memory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectionDialog connectionDialog;
    if (connectionDialog.exec() == QDialog::Rejected)
    {
        return 0;
    }

    std::unique_ptr<ConnectionSettings> settings(connectionDialog.settings());
    if(!settings)
    {
        return 0;
    }

    MainWindow window(settings.get());
    window.show();
    return a.exec();
}
