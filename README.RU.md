# Документация к программе-примеру Get-RootDSE с использованием библиотеки libdomain

## Обзор

Предоставленный пример - это программа на языке C, демонстрирующая использование библиотеки libdomain для выполнения операции поиска LDAP. 
Программа устанавливает соединение с сервером LDAP и выполняет запрос на поиск для получения записи RootDSE в указанном каталоговом сервере.

## Компиляция

Для компиляции программы вам необходимо установить библиотеку libdomain.

```bash
apt-get install libdomain-devel libconfig-devel cmake rpm-build qt5-base-devel glib2-devel
```

Клонируйте пример:

```bash
git clone https://github.com/libdomain/libdomain-qt-sample
```

Используйте следующую команду компиляции:

```bash
mkdir build && cd build && cmake .. && make -j `nproc`
```

## Использование

Программа имеет диалог подключения, позволяющий пользователю указать параметры подключения к серверу LDAP.

```bash
./libdomain-qt-sample
```

### Диалог подключения к серверу
![Диалог подключения к серверу](docs/connection_dialog.png)

### Основное окно программы
![Основное окно программы](docs/rootdse.png)

## Структура программы

Программа состоит из следующих модулей:
- MainWindow - этот класс инициализирует библиотеку libdomain. Обратите внимание что мы не запускаем
новый цикл событий, но используем цикл событий qt. Для этого ничего не нужно сделать дополнительно,
достаточно проинициализировать библиотеку.

```cpp
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
```

- AttributesModel - этот класс унаследованный от QStandardItemModel осуществляет основную работу. 
Он отправляет запрос к LDAP серверу, получая от него список арибутов записи RootDSE.
В конструкторе мы создаём поисковый запрос для libdomain и получаем список атрибутов при помощи
функции `search_callback`. Как только `search_callback` вызовет сигнал AttributesModelPrivate::ready,
мы начинаем зполнение модели данными. Предполагается что к этому моменту мы уже имеем проинициализированный
укзатель библиотеки `ldhandle` и соединение уже установлено.

```cpp
    d->handle = ldhandle;

    privateData = d.get();

    search(d->handle->connection_ctx,
           "",
           LDAP_SCOPE_BASE,
           NULL,
           const_cast<char**>(LDAP_ATTRS),
           0,
           search_callback);

    connect(d.get(), &AttributesModelPrivate::ready, [&]()
    {
        for (size_t i = 0; i < d->attributes.size(); ++i)
        {
            QStandardItem* nameItem = new QStandardItem();
            nameItem->setText(d->attributes[i].name);

            QStandardItem* valueItem = new QStandardItem();
            valueItem->setText(d->attributes[i].values.join(";"));

            this->appendRow({nameItem, valueItem});
        }
    });
```

- ConnectionDialog - содержит диалог настроек подключения. При успешном заполнении он создаёт структуру `ConnectionSettings`.
- ConnectionSettings - содержит настройки подключения такие как: сервер, имя пользователя, пароль, bind_dn, использование интерактивного подключения.


## Обработка ошибок

Обработка ошибок подключения осуществляется библиотекой `libdomain` автоматически при помощи встроенных в неё функций.
При желании можно установить свой обработчик при помомщи функции `ld_install_error_handler(handle, connection_on_error)`
передав ей callback ваш connection_on_error.

## Информация о версии

- Версия программы: 1.0.0

## Лицензия

Эта программа распространяется под лицензией GPLv2. Дополнительную информацию смотрите в сопровождающем файле LICENSE.
