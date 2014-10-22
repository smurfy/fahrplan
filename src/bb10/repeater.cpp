/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "repeater.h"

#include <bb/cascades/DataModel>
#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/RadioGroup>

using namespace bb::cascades;

enum {
    ContainerType = 0,
    DropDownType,
    RadioGroupType,
    UnknownType
};

Repeater::Repeater(Container *parent)
    : CustomControl(parent)
    , m_delegate(NULL)
{
    setVisible(false);
}

QVariant Repeater::model() const
{
    return m_model;
}

void Repeater::setModel(const QVariant &model)
{
    if (m_model == model)
        return;

    clear();

    DataModel *dataModel = NULL;
    if (m_model.canConvert<DataModel *>()) {
        dataModel = m_model.value<DataModel *>();
        if (dataModel)
            dataModel->disconnect(this);
    }

    m_model = model;

    createDelegates();

    if (model.canConvert<DataModel *>()) {
        dataModel = model.value<DataModel *>();
        if (dataModel) {
            connect(dataModel,
                    SIGNAL(itemAdded(QVariantList)),
                    SLOT(onItemAddedRemovedOrUpdated(QVariantList)));
            connect(dataModel,
                    SIGNAL(itemRemoved(QVariantList)),
                    SLOT(onItemAddedRemovedOrUpdated(QVariantList)));
            connect(dataModel,
                    SIGNAL(itemUpdated(QVariantList)),
                    SLOT(onItemAddedRemovedOrUpdated(QVariantList)));
            connect(dataModel,
                    SIGNAL(itemsChanged(bb::cascades::DataModelChangeType::Type,
                                        QSharedPointer<bb::cascades::DataModel::IndexMapper>)),
                    SLOT(onItemsChanged(bb::cascades::DataModelChangeType::Type,
                                        QSharedPointer<bb::cascades::DataModel::IndexMapper>)));
        }
    }

    emit modelChanged();
}

QDeclarativeComponent* Repeater::delegate() const
{
    return m_delegate;
}

void Repeater::setDelegate(QDeclarativeComponent *delegate)
{
    if (m_delegate == delegate)
        return;

    clear();
    m_delegate = delegate;
    createDelegates();

    emit delegateChanged();
}

void Repeater::onItemAddedRemovedOrUpdated(const QVariantList &)
{
    clear();
    createDelegates();
}

void Repeater::onItemsChanged(DataModelChangeType::Type, QSharedPointer<DataModel::IndexMapper>)
{
    clear();
    createDelegates();
}

void Repeater::clear()
{
    switch (getParentType()) {
    case ContainerType: {
        Container *container = qobject_cast<Container *>(parent());
        foreach (UIObject *control, m_controls) {
            container->remove(qobject_cast<Control *>(control));
        }
        qDeleteAll(m_controls);
        break;
    }
    case DropDownType: {
        DropDown *dropDown = qobject_cast<DropDown *>(parent());
        dropDown->removeAll();
        break;
    }
    case RadioGroupType: {
        RadioGroup *radioGroup = qobject_cast<RadioGroup *>(parent());
        radioGroup->removeAll();
        break;
    }
    }

    m_controls.clear();
}

int Repeater::getParentType() const
{
    if (qobject_cast<Container *>(parent()))
        return ContainerType;
    else if (qobject_cast<DropDown *>(parent()))
        return DropDownType;
    else if (qobject_cast<RadioGroup *>(parent()))
        return RadioGroupType;
    else
        return UnknownType;
}

void Repeater::createDelegates()
{
    if (!m_delegate || !m_model.isValid())
        return;

    Container *container = NULL;
    DropDown *dropDown = NULL;
    RadioGroup *radioGroup = NULL;

    int pos = 0;
    switch (getParentType()) {
    case ContainerType:
        container = qobject_cast<Container *>(parent());
        pos = container->indexOf(this);
        break;
    case DropDownType:
        dropDown = qobject_cast<DropDown *>(parent());
        break;
    case RadioGroupType:
        radioGroup = qobject_cast<RadioGroup *>(parent());
        break;
    }

    QVariantList list;
    DataModel *dataModel = NULL;
    int count = 0;
    bool isDataModel = false;

    if (m_model.type() == QVariant::List) {
        list = m_model.toList();
        count = list.count();
    } else if (m_model.canConvert<DataModel *>()) {
        dataModel = m_model.value<DataModel *>();
        if (!dataModel)
            return;
        count = dataModel->childCount(QVariantList());
        isDataModel = true;
    }

    for (int i = 0; i < count; ++i) {
        QDeclarativeContext *context = new QDeclarativeContext(m_delegate->creationContext(), this);
        context->setContextProperty("index", i);

        if (m_model.type() == QVariant::List) {
            context->setContextProperty("modelData", list.at(i));
        } else if (isDataModel) {
            const QVariantMap map = dataModel->data(QVariantList() << 1).toMap();
            foreach (const QString &key, map.keys()) {
                context->setContextProperty(key, map.value(key));
            }
        }

        UIObject *obj;
        switch (getParentType()) {
        case ContainerType:
            obj = qobject_cast<Control *>(m_delegate->create(context));
            container->insert(pos + i + 1, qobject_cast<Control *>(obj));
            break;
        case DropDownType:
            obj = qobject_cast<Option *>(m_delegate->create(context));
            dropDown->add(qobject_cast<Option *>(obj));
            break;
        case RadioGroupType:
            obj = qobject_cast<Option *>(m_delegate->create(context));
            radioGroup->add(qobject_cast<Option *>(obj));
            break;
        }
        m_controls.append(obj);
    }
}
