#ifndef LD_ATTRIBUTES_MODEL_H
#define LD_ATTRIBUTES_MODEL_H

#include <QStandardItemModel>

extern "C"
{
typedef struct ldhandle LDHandle;
}

class AttributesModelPrivate;

class AttributesModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit AttributesModel(LDHandle* ldhandle, QObject* parent);
    ~AttributesModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QScopedPointer<AttributesModelPrivate> d;
};

#endif//LD_ATTRIBUTES_MODEL_H
