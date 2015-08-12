#include "bfd3me.h"
#include "ui_bfd3me.h"

#include <QMessageBox>

#include "util.h"

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
