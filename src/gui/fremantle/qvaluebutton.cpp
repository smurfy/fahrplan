/*******************************************************************************

    This file is a part of Fahrplan for maemo 2009-2010

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/

#include "qvaluebutton.h"

QValueButton::QValueButton(const QString &text, const QString &description, QWidget *parent)
    : QCommandLinkButton(text, description, parent)
{
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(selfClicked()));
    itemModelSelector = new QMaemo5ListPickSelector;
    connect(itemModelSelector, SIGNAL(selected(QString)), this, SLOT(modelSelected(QString)));
    itemModel = NULL;
}

void QValueButton::selfClicked()
{
    if (itemModel) {
        QWidget *w = itemModelSelector->widget(this);
        w->show();
    }
}

void QValueButton::modelSelected(QString value)
{
    for (int i = 0; i < itemModel->rowCount(); i++) {
        if (itemModel->item(i)->text() == value) {
            this->setModelIndex(i);
            break;
        }
    }
    this->setDescription(value);
}

QStandardItemModel * QValueButton::model()
{
    return itemModel;
}

void QValueButton::setModel(QStandardItemModel * model)
{
    itemModel = model;
    itemModelSelector->setModel(itemModel);
    itemModelSelector->setCurrentIndex(itemIndex);
}

int QValueButton::modelIndex()
{
    return itemIndex;
}

void QValueButton::setModelIndex(int index)
{
    itemIndex = index;
    itemModelSelector->setCurrentIndex(itemIndex);
}

QRect QValueButton::titleRect() const
{
    return rect().adjusted(0, 10, -4, -4);
}

QRect QValueButton::descriptionRect() const
{
    return rect().adjusted(0, descriptionOffset(), -4, -4);
}

int QValueButton::descriptionOffset() const
{
    QFontMetrics fm(titleFont());
    return 10 + fm.height();
}

QFont QValueButton::titleFont() const
{
    QFont font = QMaemo5Style::standardFont("SystemFont");
    return font;
}

QFont QValueButton::descriptionFont() const
{
    QFont font = QMaemo5Style::standardFont("SmallSystemFont");
    return font;
}

void QValueButton::paintEvent(QPaintEvent *)
{
    QStylePainter p(this);
    p.save();

    QStyleOptionButton option;
    initStyleOption(&option);

    //Enable command link appearence on Vista
    option.features |= QStyleOptionButton::CommandLinkButton;
    option.text = QString();
    option.icon = QIcon(); //we draw this ourselves

    int vOffset = isDown() ? style()->pixelMetric(QStyle::PM_ButtonShiftVertical) : 0;
    int hOffset = isDown() ? style()->pixelMetric(QStyle::PM_ButtonShiftHorizontal) : 0;

    QFontMetrics textFm = QFontMetrics(titleFont());
    QFontMetrics descriptionFm = QFontMetrics(descriptionFont());

    int textWidth = textFm.size(Qt::TextSingleLine, text()).width();
    int descriptionWidth = descriptionFm.size(Qt::TextSingleLine, description()).width();
    int textHeight = textFm.size(Qt::TextSingleLine, text()).height();
    int descriptionHeight =descriptionFm.size(Qt::TextSingleLine, description()).height();

    QRect ir = option.rect;

    int titleHOffset = ir.left() + (ir.width() - (qMax(textWidth, descriptionWidth))) / 2;
    int titleVOffset = ir.top() + (ir.height() / 2) - (((textHeight) / 2) + descriptionHeight);
    int deltaText = (textWidth - descriptionWidth) / 2;

    p.drawControl(QStyle::CE_PushButton, option);
    int textflags = Qt::TextShowMnemonic;
    if (!style()->styleHint(QStyle::SH_UnderlineShortcut, &option, this))
        textflags |= Qt::TextHideMnemonic;

    p.setFont(titleFont());
    p.drawItemText(titleRect().translated(hOffset + titleHOffset + (deltaText > 0 ? 0 : -deltaText), vOffset + titleVOffset),
                    textflags, option.palette, isEnabled(), text(), QPalette::ButtonText);

    //Draw description
    QColor color = QMaemo5Style::standardColor("ActiveTextColor");
    QPalette pal = option.palette;
    pal.setBrush(QPalette::ButtonText, color);

    textflags |= Qt::TextWordWrap | Qt::ElideRight;
    p.setFont(descriptionFont());
    p.drawItemText(descriptionRect().translated(hOffset + (titleHOffset + (deltaText < 0 ? 0 : deltaText)), vOffset + titleVOffset), textflags,
                    pal, isEnabled(), description(), QPalette::ButtonText);
    p.restore();
}
