#ifndef ALLOCATEMODECREATOR_H
#define ALLOCATEMODECREATOR_H

#include <QObject>
#include <QFile>
#include <QtConcurrent/QtConcurrent>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#ifdef Q_OS_UNIX
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

class AllocateModeCreator : public QObject
{
    Q_OBJECT
public:
    AllocateModeCreator(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void create(const QString &filePath, qint64 totalSize) {
        QFuture<void> future = QtConcurrent::run([this, filePath, totalSize]() {
            bool success = false;
            QString message;

#ifdef Q_OS_WIN
            HANDLE hFile = CreateFileW((LPCWSTR)filePath.utf16(), GENERIC_WRITE, 0, nullptr,
                                       CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile == INVALID_HANDLE_VALUE) {
                emit finished(false, tr("无法创建文件"));
                return;
            }

            LARGE_INTEGER li;
            li.QuadPart = totalSize;
            if (SetFilePointerEx(hFile, li, nullptr, FILE_BEGIN) == 0) {
                CloseHandle(hFile);
                emit finished(false, tr("设置文件指针失败"));
                return;
            }
            if (SetEndOfFile(hFile) == 0) {
                CloseHandle(hFile);
                emit finished(false, tr("SetEndOfFile 失败"));
                return;
            }
            CloseHandle(hFile);
            success = true;
            message = tr("榨干成功（划区模式，Windows）");
#elif defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
            int fd = open(filePath.toUtf8().constData(), O_CREAT | O_RDWR, 0666);
            if (fd == -1) {
                emit finished(false, tr("无法创建文件"));
                return;
            }

            int result = posix_fallocate(fd, 0, totalSize);
            close(fd);
            if (result == 0) {
                success = true;
                message = tr("榨干成功（划区模式，POSIX）");
            } else {
                success = false;
                message = tr("posix_fallocate 失败，错误码: ") + QString::number(result);
            }
#else
            // 回退到 QFile::resize (可能生成稀疏文件)
            QFile file(filePath);
            if (file.resize(totalSize)) {
                success = true;
                message = tr("榨干成功（使用 resize 回退）");
            } else {
                success = false;
                message = tr("无法分配空间以榨干");
            }
#endif
            emit progress(totalSize, totalSize);
            emit finished(success, message);
        });
        (void)future; // 避免未使用的变量警告
    }

signals:
    void finished(bool success, const QString &message);
    void progress(double currentSize, double totalSize);
};

#endif // ALLOCATEMODECREATOR_H
