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

#include "languagechangelistener.h"

#include <QTranslator>
#include <bb/cascades/LocaleHandler>

LanguageChangeListener::LanguageChangeListener(QObject *parent)
    : QObject(parent)
{
    m_pLocaleHandler = new bb::cascades::LocaleHandler(this);
    connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));

    onSystemLanguageChanged();
}

void LanguageChangeListener::onSystemLanguageChanged()
{
    static QTranslator *qtTr = NULL;
    if (qtTr) {
        qApp->removeTranslator(qtTr);
        delete qtTr;
    }

    static QTranslator *tr = NULL;
    if (tr) {
        qApp->removeTranslator(tr);
        delete tr;
    }

    const QString lc = QLocale().name();
    // Load Qt's translation
    qtTr = new QTranslator(this);
    if (qtTr->load("qt_" + lc, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        qApp->installTranslator(qtTr);

    // Load TAOT's translation
    tr = new QTranslator(this);
    if (tr->load("fahrplan_" + lc, ":/translations"))
        qApp->installTranslator(tr);
}
