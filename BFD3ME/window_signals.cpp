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

#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QMessageBox>
#include <QtDebug>

#include "utils/util.h"

/*
 * Source file with custom window signals
 */

void BFD3ME::progressChanged(QString progressStr, int progressDone, int progressTodo) {
    ui->statusBar->showMessage(QString("%0: %1/%2").arg(progressStr)
                               .arg(progressDone).arg(progressTodo));
}

void BFD3ME::error(QString path, QString errorStr) {
    errors << QString("%0 (%1)").arg(path).arg(errorStr);
}

void BFD3ME::finished() {
    if (errors.count() != 0) {
        QString errStr = "Errors have occured:";
        foreach (const QString &str, errors) {
            errStr += QString("\n%0").arg(str);
        }
        QMessageBox::warning(this, "Errors occured", errStr);
    }
    loadThread.quit();
}

void BFD3ME::loadBtn_animation(int value) {
    int base_red = palette.text().color().red();
    int base_green = palette.text().color().green();
    int base_blue = palette.text().color().blue();
    int cur_frame = value % 100;
    if (cur_frame > 50) {
        cur_frame = 100 - (cur_frame);
    }
    int red_range = 255 - base_red;
    int cur_red = base_red + (int) ((cur_frame / 50.0) * red_range);
    ui->loadBtn->setStyleSheet(QString("font-weight: bold; color: rgb(%0,%1,%2)").arg(cur_red).arg(base_green).arg(base_blue));
}

void BFD3ME::loadBtn_animation_finished() {
    ui->loadBtn->setStyleSheet("");
}
