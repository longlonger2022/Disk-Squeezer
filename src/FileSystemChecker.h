#ifndef FILESYSTEMCHECKER_H
#define FILESYSTEMCHECKER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QStorageInfo>

class FileSystemChecker : public QObject
{
    Q_OBJECT
public:
    FileSystemChecker(QObject *parent = nullptr) : QObject(parent) {}

    // 获取父目录
    Q_INVOKABLE QString getDirectory(const QString &filePath) {
        return QFileInfo(filePath).absolutePath();
    }

    // 检查文件路径是否符合要求
    Q_INVOKABLE bool check(const QString &path) {
        QFileInfo info(path);
        return info.absoluteDir().exists() && (!info.exists() || info.isFile());
    }

    Q_INVOKABLE qint64 fileSize(const QString &filePath) {
        QFileInfo info(filePath);
        if (info.exists() && info.isFile()) {
            return info.size();
        }
        return -1;
    }

    // 获取指定路径所在磁盘的剩余可用空间（字节）
    Q_INVOKABLE qint64 getFreeSpace(const QString &path) {
        QStorageInfo storage(path);
        if (storage.isValid() && storage.isReady()) {
            return storage.bytesAvailable();
        }
        return -1;
    }
};

#endif // FILESYSTEMCHECKER_H
