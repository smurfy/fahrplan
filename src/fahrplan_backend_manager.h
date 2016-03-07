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

#ifndef FAHRPLAN_BACKEND_MANAGER_H
#define FAHRPLAN_BACKEND_MANAGER_H

#include "fahrplan_parser_thread.h"
#include "models/backends.h"

class FahrplanBackendManager : public QObject
{
    Q_OBJECT

    public:
        explicit FahrplanBackendManager(int defaultParser, QObject *parent = 0);
        QStringList getParserList();
        void setParser(int index);
        FahrplanParserThread *getParser();

    signals:
        void parserChanged(QString name, int index);

    private:
        FahrplanParserThread *m_parser;
        int currentParserIndex;
};

#endif // FAHRPLAN_BACKEND_MANAGER_H
