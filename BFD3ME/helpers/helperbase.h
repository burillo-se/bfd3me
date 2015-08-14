/***
* BFD3 Metadata Editor
* Copyright (C) 2015 Anatoly Burakov
* 
* Contact information: burakov.anatoly@gmail.com
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef HELPERBASE_H
#define HELPERBASE_H

#include <QObject>
#include <QDomDocument>
#include <QString>

/*
 * Q_OBJECT macro needed for signals/slots support doesn't support templates,
 * so we create a dummy object with a signal, and then inherit our templates from it.
 */
class HelperBase : public QObject
{
    Q_OBJECT
protected:
    QDomDocument loadDoc(const QString &path);
    void saveDoc(const QDomDocument &doc, const QString &path);
    void restoreFile(const QString &path);
    int _progressDone;
    int _progressTodo;
public:
    HelperBase(QObject *parent = 0) : QObject(parent) {}
signals:
    void progressChanged(QString,int,int);
    void finished();
    void error(QString,QString);
};

#endif // HELPERBASE_H
