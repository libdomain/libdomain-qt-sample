#include "connectiondialog.h"
#include "./ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog())
{
    ui->setupUi(this);
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

ConnectionSettings *ConnectionDialog::settings()
{
    ConnectionSettings* result = new ConnectionSettings(ui->hostnameLineEdit->text(),
                                                        ui->portSpinBox->text(),
                                                        ui->usernameLineEdit->text(),
                                                        ui->passwordLineEdit->text(),
                                                        ui->interactiveBindCheckBox->isChecked());
    return result;
}

