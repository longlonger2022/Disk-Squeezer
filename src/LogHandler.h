#ifndef LOGHANDLER_H
#define LOGHANDLER_H

#include <QObject>
#include <QtGlobal>
#include <QDateTime>

class LogHandler : public QObject
{
    Q_OBJECT
public:
    static LogHandler* instance() {
        static LogHandler instance;
        return &instance;
    }

    ~LogHandler() {
        qInstallMessageHandler(nullptr);
    }


signals:
    void logMessage(const QString &message);

private:
    LogHandler(QObject *parent = nullptr) : QObject(parent) {
        qInstallMessageHandler(messageHandler);
    }
    
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        Q_UNUSED(context)
        QString log;
        switch(type){
            case QtDebugMsg:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[DEBUG]").leftJustified(10)).arg(msg);
                break;
            case QtInfoMsg:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[INFO]").leftJustified(10)).arg(msg);
                break;
            case QtWarningMsg:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[WARNING]").leftJustified(10)).arg(msg);
                break;
            case QtCriticalMsg:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[Critical]").leftJustified(10)).arg(msg);
                break;
            case QtFatalMsg:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[FATAL]").leftJustified(10)).arg(msg);
                break;
            default:
                log = QString("%1 - %2 - %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(QString("[UNKNOWN]").leftJustified(10)).arg(msg);
                break;
        }
        if (instance()) {
            emit instance()->logMessage(log);
        }
    }
};

#endif // LOGHANDLER_H
