#ifndef WRITEMODECREATOR_H
#define WRITEMODECREATOR_H

#include <QObject>
#include <QFile>
#include <QtConcurrent/QtConcurrent>
#include <QAtomicInt>
#include <QMutex>
#include <QWaitCondition>

class WriteModeCreator : public QObject
{
    Q_OBJECT
public:
    WriteModeCreator(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE void create(const QString &filePath, qint64 totalSize) {
        m_canceled.storeRelaxed(0);
        m_future = QtConcurrent::run([this, filePath, totalSize]() {
            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly)) {
                emit finished(false, tr("无法打开文件写入"));
                return;
            }

            const int blockSize = 1024 * 1024; // 1 MB 块
            QByteArray block(blockSize, '\0');
            qint64 written = 0;
            while (written < totalSize) {
                // 检查取消
                if (m_canceled.loadRelaxed()) {
                    file.close();
                    file.remove(); // 删除未完成的文件
                    emit finished(false, tr("已取消"));
                    return;
                }

                // 检查暂停
                if (m_paused.loadRelaxed()) {
                    QMutexLocker locker(&m_mutex);
                    m_waitCondition.wait(&m_mutex); // 等待继续
                    // 唤醒后检查是否取消
                    if (m_canceled.loadRelaxed()) {
                        file.close();
                        file.remove();
                        emit finished(false, tr("已取消"));
                        return;
                    }
                    continue;
                }

                qint64 toWrite = qMin(blockSize, totalSize - written);
                if (file.write(block.constData(), toWrite) != toWrite) {
                    file.close();
                    emit finished(false, tr("写入失败"));
                    return;
                }
                written += toWrite;
                emit progress(written, totalSize);
            }
            file.close();
            emit finished(true, tr("榨干成功（写入模式）"));
        });
    }

    Q_INVOKABLE void pause() {
        m_paused.storeRelaxed(1);
    }

    Q_INVOKABLE void resume() {
        m_paused.storeRelaxed(0);
        QMutexLocker locker(&m_mutex);
        m_waitCondition.wakeAll();
    }

    Q_INVOKABLE void cancel() {
        m_canceled.storeRelaxed(1);
        resume(); // 如果暂停中，唤醒以便取消
    }

signals:
    void finished(bool success, const QString &message);
    void progress(double currentSize, double totalSize);

private:
    QAtomicInt m_canceled{0};
    QAtomicInt m_paused{0};
    QMutex m_mutex;
    QWaitCondition m_waitCondition;
    QFuture<void> m_future;
};

#endif // WRITEMODECREATOR_H
