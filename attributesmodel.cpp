#include "attributesmodel.h"

#include <QDebug>
#include <QTimer>

#include <functional>

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

struct Attribute
{
    QString name;
    QStringList values;
};

class AttributesModelPrivate : public QObject
{
    Q_OBJECT

public:
    LDHandle* handle = nullptr;
    QStringList headerData { "Name", "Value" };
    QVector<Attribute> attributes;

signals:
    void ready();
};

const char *LDAP_ATTRS[] = { "*", "+", NULL };

static AttributesModelPrivate* privateData = NULL;

enum OperationReturnCode search_callback(struct ldap_connection_ctx_t *connection, ld_entry_t** entries)
{
    Q_UNUSED(connection);

    int entry_index = 0;
    while (entries[entry_index] != NULL)
    {
        ld_entry_t* entry = entries[entry_index];

        int attribute_index = 0;
        LDAPAttribute_t** attributes = ld_entry_get_attributes(entry);
        while(attributes[attribute_index] != NULL)
        {
            LDAPAttribute_t* attribute = attributes[attribute_index];

            Attribute cppAttribute;
            cppAttribute.name = attribute->name;

            int value_index = 0;
            char** values = attribute->values;
            while (values[value_index] != NULL)
            {
                cppAttribute.values.append(values[value_index]);
                value_index++;
            }

            privateData->attributes.append(cppAttribute);

            attribute_index++;
        }

        talloc_free(entry);

        entry_index++;
    }

    emit privateData->ready();

    return RETURN_CODE_SUCCESS;
}

AttributesModel::AttributesModel(LDHandle *ldhandle, QObject *parent)
    : QStandardItemModel(parent)
    , d(new AttributesModelPrivate())
{
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
}

AttributesModel::~AttributesModel()
{
}

QVariant AttributesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 0)
    {
        return QVariant();
    }
    if (section >= d->headerData.size())
    {
        return QVariant();
    }
    if (orientation != Qt::Horizontal)
    {
        return QVariant();
    }
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    return d->headerData[section];
}

#include "attributesmodel.moc"
