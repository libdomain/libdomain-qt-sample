#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "attributesmodel.h"

#include <ldap.h>
#include <talloc.h>

extern "C"
{
#include <libdomain/common.h>
#include <libdomain/domain.h>
#include <libdomain/domain_p.h>
#include <libdomain/directory.h>
#include <libdomain/entry.h>
#include <libdomain/connection_state_machine.h>
}

class CallbackHelper : public QObject
{
    Q_OBJECT
public:
    explicit CallbackHelper(QObject* parent)
        : QObject(parent)
    {}

signals:
    void ready();
};

CallbackHelper* helper = NULL;

void connection_on_update(verto_ctx *ctx, verto_ev *ev)
{
    Q_UNUSED(ctx);

    ldap_connection_ctx_t* connection = static_cast<ldap_connection_ctx_t*>(verto_get_private(ev));

    if (connection->state_machine->state == LDAP_CONNECTION_STATE_RUN)
    {
        verto_del(ev);

        if (helper)
        {
            helper->ready();
        }
    }
}

MainWindow::MainWindow(ConnectionSettings *settings, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    helper = new CallbackHelper(this);

    TALLOC_CTX* talloc_ctx = talloc_new(NULL);

    const int update_interval = 10;

    ld_config_t *config = NULL;
    config = ld_create_config(talloc_ctx, settings->hostname, 0, LDAP_VERSION3, settings->bind_dn,
                              settings->username, settings->password, !settings->useSasl, false, settings->useSasl, false,
                              update_interval, talloc_strdup(talloc_ctx, ""), talloc_strdup(talloc_ctx, ""), talloc_strdup(talloc_ctx, ""));

    LDHandle *handle = NULL;
    ld_init(&handle, config);

    ld_install_default_handlers(handle);
    ld_install_handler(handle, connection_on_update, update_interval);

    ui->setupUi(this);

    connect(helper, &CallbackHelper::ready, [&, handle]()
    {
        ui->tableView->setModel(new AttributesModel(handle, this));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

#include "mainwindow.moc"
