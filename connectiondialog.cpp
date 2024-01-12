#include "connectiondialog.h"
#include "./ui_connectiondialog.h"

#include <QToolTip>

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConnectionDialog())
{
    ui->setupUi(this);

    QRegExp rx("(^(ldap|ldaps):\\/\\/(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)"
               "*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$)|"
               "(^(ldap|ldaps):\\/\\/(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}"
               "([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$)");

    QValidator *validator = new QRegExpValidator(rx, this);

    ui->hostnameLineEdit->setValidator(validator);

    connect(ui->hostnameLineEdit, &QLineEdit::textEdited, [&] ()
    {
        if(ui->hostnameLineEdit->hasAcceptableInput() == false)
        {
            QToolTip::showText(ui->hostnameLineEdit->mapToGlobal(QPoint()), tr("Invalid Input - use ldap://hostname format."));
        }
        else
        {
            QToolTip::hideText();
        }
    });
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

