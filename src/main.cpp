#include "FileSystemChecker.h"
#include "WriteModeCreator.h"
#include "CommandModeCreator.h"
#include "AllocateModeCreator.h"
#include "AutoTaskCreator.h"
#include "LogHandler.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QFont>
#include <QIcon>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSettings>

static FileSystemChecker* g_fileSystemChecker = new FileSystemChecker();
static WriteModeCreator* g_writeModeCreator = new WriteModeCreator();
static CommandModeCreator* g_commandModeCreator = new CommandModeCreator();
static AllocateModeCreator* g_allocateModeCreator = new AllocateModeCreator();
static AutoTaskCreator* g_autoTaskCreator = new AutoTaskCreator();

bool squeeze(const QString &path, const QString &mode, qint64 size);

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName(QObject::tr("硬盘榨干器"));
    app.setApplicationVersion("3.0.0");


    QCommandLineParser parser;
    parser.setApplicationDescription(QObject::tr("硬盘榨干器"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption pathOption("path", QObject::tr("榨干路径 (必需)"), QObject::tr("路径"));
    parser.addOption(pathOption);
    QCommandLineOption modeOption("mode", QObject::tr("榨干模式 (必需): write (写入模式) | command (命令模式) | allocate (划区模式)"), QObject::tr("模式"));
    parser.addOption(modeOption);

    parser.process(app);

    QString path = parser.value(pathOption);
    QString mode = parser.value(modeOption);

    // 如果没有带参数
    if (path.isEmpty() && mode.isEmpty()) {
        // 检查是否静默榨干
        if (g_autoTaskCreator->readAutoTaskInfo(path, mode)) {
            // 静默榨干
            qint64 size = g_fileSystemChecker->getFreeSpace(g_fileSystemChecker->getDirectory(path));
            if (size < 0) {
                qCritical() << "cannot get free space.";
                return -1;
            }
            bool success = squeeze(path, mode, size);
            return success ? 0 : -1;
        }
        // 否则启动GUI
    }
    // 如果带参数
    else {
        QString path = parser.value(pathOption);
        QString mode = parser.value(modeOption);

        if (path.isEmpty() || mode.isEmpty() || (mode != "write" && mode != "command" && mode != "allocate")) {
            parser.showHelp(-1);
        }

        qint64 size = g_fileSystemChecker->getFreeSpace(g_fileSystemChecker->getDirectory(path));
        if (size < 0) {
            qCritical() << "cannot get free space.";
            return -1;
        }
        bool success = squeeze(path, mode, size);
        return success ? 0 : -1;
    }

    app.setWindowIcon(QIcon(":/assets/images/icon.png"));
#ifdef Q_OS_ANDROID
    QQuickStyle::setStyle("Material");
#else
    QQuickStyle::setStyle("FluentWinUI3");
#endif
    QFont defaultFont;
    defaultFont.setFamilies({
        "Maple Mono NF CN",
        "Maple Mono CN",
        "Maple Mono",
        "Microsoft YaHei UI",
        "Microsoft YaHei",
        "Noto Sans CJK SC",
        "Source Han Sans CN",
        "Segoe UI",
        "sans-serif"
    });
    app.setFont(defaultFont);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("fileSystemChecker", g_fileSystemChecker);
    engine.rootContext()->setContextProperty("writeModeCreator", g_writeModeCreator);
    engine.rootContext()->setContextProperty("commandModeCreator", g_commandModeCreator);
    engine.rootContext()->setContextProperty("allocateModeCreator", g_allocateModeCreator);
    engine.rootContext()->setContextProperty("autoTaskCreator", g_autoTaskCreator);

    // 日志处理
    LogHandler *logHandler = LogHandler::instance();
    engine.rootContext()->setContextProperty("logHandler", logHandler);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("DiskSqueezer3", "Main");

    return app.exec();
}






bool squeeze(const QString &path, const QString &mode, qint64 size) {
    // 检查父目录
    if (!g_fileSystemChecker->check(path)) {
        qCritical() << "parent directory " << g_fileSystemChecker->getDirectory(path) << " does not exist, cannot be accessed or " << path << " is not a file.";
        return false;
    }

    bool success = false;
    QString message;
    QEventLoop loop;

    if (mode == "write") {
        QObject::connect(g_writeModeCreator, &WriteModeCreator::finished, [&](bool s, const QString &msg) {
            success = s;
            message = msg;
            loop.quit();
        });
    }
    else if (mode == "command") {
        QObject::connect(g_commandModeCreator, &CommandModeCreator::finished, [&](bool s, const QString &msg) {
            success = s;
            message = msg;
            loop.quit();
        });
    }
    else if (mode == "allocate") {
        QObject::connect(g_allocateModeCreator, &AllocateModeCreator::finished, [&](bool s, const QString &msg) {
            success = s;
            message = msg;
            loop.quit();
        });
    }

    QElapsedTimer timer;
    if (mode == "write") {
        timer.start();
        QObject::connect(g_writeModeCreator, &WriteModeCreator::progress, [&](double current, double total) {
            static int lastPercent = -1;
            if (timer.elapsed() > 10) { // 每10ms更新一次进度
                int percent = static_cast<int>((current / total) * 100);
                if (percent != lastPercent) {
                    printf(QObject::tr("\r进度: %d%%").toUtf8().constData(), percent);
                    fflush(stdout);
                    lastPercent = percent;
                    timer.restart();
                }
            }
        });
    }
    else if (mode == "command") {
        QObject::connect(g_commandModeCreator, &CommandModeCreator::commandStarted, [](const QString &cmd) {
            printf("> %s\n", cmd.toUtf8().constData());
        });
        QObject::connect(g_commandModeCreator, &CommandModeCreator::commandOutput, [](const QString &out) {
            if (!out.trimmed().isEmpty()) {
                printf("%s\n", out.toUtf8().constData());
            }
        });
    }

    if (mode == "write") {
        g_writeModeCreator->create(path, size);
    } else if (mode == "command") {
        g_commandModeCreator->create(path, size);
    } else if (mode == "allocate") {
        g_allocateModeCreator->create(path, size);
    }

    loop.exec();

    return success;
}
