#ifndef COMMANDMODECREATOR_H
#define COMMANDMODECREATOR_H

#include <QObject>
#include <QProcess>
#include <QtConcurrent/QtConcurrent>

class CommandModeCreator : public QObject
{
    Q_OBJECT
public:
    CommandModeCreator(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void create(const QString &filePath, qint64 totalSize) {
        QFuture<void> future = QtConcurrent::run([this, filePath, totalSize]() {
            bool success = false;

#ifdef Q_OS_WIN
            // 构建命令行用于显示
            emit commandStarted(QString("fsutil file createnew \"%1\" %2").arg(filePath).arg(totalSize));

            // 使用 fsutil
            QString program = "fsutil";
            QStringList args;
            args << "file" << "createnew" << filePath << QString::number(totalSize);
            QProcess process;
            process.start(program, args);
            if (!process.waitForFinished(30000)) {
                emit commandOutput("fsutil timeout, trying PowerShell...");
            } else {
                if (process.exitCode() == 0) {
                    success = true;
                    emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed());
                } else {
                    emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed() + "\n\nfsutil failed, trying PowerShell...");
                }
            }

            // fsutil 失败使用 PowerShell 榨干
            if (!success) {
                // 重新构建命令显示
                emit commandStarted(QString("powershell -Command \"$file=[System.IO.File]::Create('%1'); $file.SetLength(%2); $file.Close()\"").arg(filePath).arg(totalSize));

                // 转义反斜杠
                QString escapedPath = filePath;
                escapedPath.replace("\\", "\\\\");
                QString psCommand = QString(
                                        "$file = [System.IO.File]::Create(\"%1\"); "
                                        "$file.SetLength(%2); "
                                        "$file.Close();"
                                        ).arg(escapedPath).arg(totalSize);

                QStringList psArgs;
                psArgs << "-NoProfile" << "-Command" << psCommand;
                QProcess ps;
                ps.start("powershell.exe", psArgs);
                if (!ps.waitForFinished(30000)) {
                    emit commandOutput("PowerShell timeout.");
                }
                else {
                    if (ps.exitCode() == 0) {
                        success = true;
                        emit commandOutput(QString::fromLocal8Bit(ps.readAllStandardOutput()).trimmed());
                    } else {
                        emit commandOutput(QString::fromLocal8Bit(ps.readAllStandardOutput()).trimmed() + "\nnowerShell failed.");
                    }
                }
            }

#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
            emit commandStarted(QString("fallocate -l %1 \"%2\"").arg(totalSize).arg(filePath));

            // Linux/macOS 使用 fallocate，失败回退到 dd
            QString program = "fallocate";
            QStringList args = {"-l", QString::number(totalSize), filePath};
            QProcess process;
            process.start(program, args);
            if (!process.waitForFinished(30000)) {
                emit commandOutput("fallocate timeout, trying dd...");
            } else {
                if (process.exitCode() == 0) {
                    success = true;
                    emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed());
                } else {
                    emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed() + "\n\nfallocate failed, trying dd...");
                }
            }

            // fallocate 失败，尝试 dd
            if (!success) {
                emit commandStarted(QString("dd if=/dev/zero of=\"%1\" bs=1M count=%2").arg(filePath).arg(totalSize / (1024*1024)));

                // fallocate 超时或失败，尝试 dd
                program = "dd";
                args = {"if=/dev/zero", "of=" + filePath, "bs=1M", "count=" + QString::number(totalSize / (1024*1024))};
                process.start(program, args);
                if (!process.waitForFinished(30000)) {
                    emit commandOutput("dd timeout.");
                } else {
                    if (process.exitCode() == 0) {
                        success = true;
                        emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed());
                    } else {
                        emit commandOutput(QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed() + "\n\ndd failed.");
                    }
                }
            }
#endif

            if (success) {
                emit finished(true, tr("榨干成功（命令模式）"));
            } else {
                emit finished(false, tr("命令模式失败"));
            }
        });
        (void)future; // 避免未使用的变量警告
    }

signals:
    void commandStarted(const QString &commandLine);
    void commandOutput(const QString &outputLine);
    void finished(bool success, const QString &message);
};

#endif // COMMANDMODECREATOR_H
