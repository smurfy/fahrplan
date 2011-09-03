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

#ifndef QVALUEBUTTON
#define QVALUEBUTTON

#include <QtGui>
#include "qstylepainter.h"
#include "qstyleoption.h"
#include "qtextdocument.h"
#include "qtextlayout.h"
#include "qcolor.h"
#include "qfont.h"

#ifdef Q_WS_MAEMO_5
    #include "QMaemo5Style"
    #include <QtMaemo5>
#endif

class QValueButton : public QCommandLinkButton
{
    Q_OBJECT

public:
    QValueButton(const QString &text, const QString &description, QWidget *parent=0);
    QStandardItemModel *model();
    void setModel(QStandardItemModel *model);
    int modelIndex();
    void setModelIndex(int index);

public slots:
    void modelSelected(QString value);
    void selfClicked();

protected:
    void paintEvent(QPaintEvent *);

private:
    #ifdef Q_WS_MAEMO_5
    QMaemo5ListPickSelector *itemModelSelector;
    #endif
    QStandardItemModel *itemModel;
    int itemIndex;
    QFont titleFont() const;
    QFont descriptionFont() const;
    QRect titleRect() const;
    QRect descriptionRect() const;
    int descriptionOffset() const;
};

#endif // QVALUEBUTTON
