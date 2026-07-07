#ifndef AUTOTASKCREATOR_H
#define AUTOTASKCREATOR_H

#include <QObject>
#include <QSettings>
#include <QFile>

class AutoTaskCreator : public QObject {
    Q_OBJECT

public:
    AutoTaskCreator(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE bool hasAutoTask() {
        if (QFile::exists("auto_task_info.ini")) {
            return true;
        } else {
            return false;
        }
    }

    
    bool readAutoTaskInfo(QString &path, QString &mode) {
        QSettings settings("auto_task_info.ini", QSettings::IniFormat);
        path = settings.value("path").toString();
        if (settings.value("mode").toInt() == 0) {
            mode = "write";
        } else if (settings.value("mode").toInt() == 1) {
            mode = "command";
        } else if (settings.value("mode").toInt() == 2) {
            mode = "allocate";
        } else {
            mode = "unknown";
        }
        return !path.isEmpty() && !mode.isEmpty();
    }

    Q_INVOKABLE void createAutoTask(const QString &path, const QString &mode) {
        QSettings settings("auto_task_info.ini", QSettings::IniFormat);
        settings.setValue("path", path);
        settings.setValue("mode", mode);
    }

    Q_INVOKABLE void deleteAutoTask() {
        if (QFile::exists("auto_task_info.ini")) {
            QFile::remove("auto_task_info.ini");
        }
    }

private:
};

#endif // AUTOTASKCREATOR_H