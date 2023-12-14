#ifndef LD_CONNECTION_DIALOG_H
#define LD_CONNECTION_DIALOG_H

#include "connectionsettings.h"

#include <QDialog>

namespace Ui
{
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = nullptr);
    ~ConnectionDialog();

    ConnectionSettings* settings();

private:
    Ui::ConnectionDialog* ui;
};

#endif//LD_CONNECTION_DIALOG_H
