#include "sysopt.h"
#include "./ui_sysopt.h"

#include <QDir>
#include <QFileInfo>

SysOpt::SysOpt(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SysOpt)
{
    ui->setupUi(this);

    connect(ui->pushButton_del_temp, &QPushButton::clicked,
            this, &SysOpt::onDelTemp);
}

SysOpt::~SysOpt()
{
    delete ui;
}

void SysOpt::onDelTemp()
{
    const QString tempPath = QStringLiteral("C:/Windows/Temp");

    ui->plainTextEdit_result->clear();
    ui->plainTextEdit_result->appendPlainText(QStringLiteral("开始清理临时目录...\n"));

    QDir dir(tempPath);
    if (!dir.exists()) {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("目录不存在: %1").arg(tempPath));
        return;
    }

    // 计算清理前的目录大小
    qint64 totalSize = 0;
    QFileInfoList beforeList = dir.entryInfoList(
        QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    for (const auto &info : beforeList) {
        if (info.isFile()) {
            totalSize += info.size();
        }
    }

    // 执行清理
    QStringList failedFiles;
    qint64 deletedSize = clearDirectory(tempPath, failedFiles);

    // 输出结果
    ui->plainTextEdit_result->appendPlainText(
        QStringLiteral("清理路径: %1").arg(tempPath));
    ui->plainTextEdit_result->appendPlainText(
        QStringLiteral("--------------------------------------------------"));

    if (deletedSize >= 1024 * 1024 * 1024) {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("已释放空间: %1 GB")
                .arg(static_cast<double>(deletedSize) / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2));
    } else if (deletedSize >= 1024 * 1024) {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("已释放空间: %1 MB")
                .arg(static_cast<double>(deletedSize) / (1024.0 * 1024.0), 0, 'f', 2));
    } else if (deletedSize >= 1024) {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("已释放空间: %1 KB")
                .arg(static_cast<double>(deletedSize) / 1024.0, 0, 'f', 2));
    } else {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("已释放空间: %1 字节").arg(deletedSize));
    }

    if (failedFiles.isEmpty()) {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("\n全部清理成功，无失败文件。"));
    } else {
        ui->plainTextEdit_result->appendPlainText(
            QStringLiteral("\n以下文件/目录未能删除 (%1 项):").arg(failedFiles.size()));
        for (const auto &f : failedFiles) {
            ui->plainTextEdit_result->appendPlainText(QStringLiteral("  - %1").arg(f));
        }
    }
}

qint64 SysOpt::clearDirectory(const QString &path, QStringList &failedFiles)
{
    qint64 freedSize = 0;
    QDir dir(path);
    if (!dir.exists()) {
        return 0;
    }

    QFileInfoList entries = dir.entryInfoList(
        QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

    for (const auto &info : entries) {
        if (info.isDir()) {
            // 递归清理子目录
            freedSize += clearDirectory(info.absoluteFilePath(), failedFiles);
            // 删除空目录
            QDir subDir(info.absoluteFilePath());
            if (!subDir.removeRecursively()) {
                failedFiles.append(info.absoluteFilePath());
            }
        } else {
            qint64 fileSize = info.size();
            QFile file(info.absoluteFilePath());
            if (file.remove()) {
                freedSize += fileSize;
            } else {
                failedFiles.append(info.absoluteFilePath());
            }
        }
    }

    // 删除根目录下的文件（再次检查，处理 setPermissions 等）
    QFileInfoList topFiles = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
    for (const auto &info : topFiles) {
        qint64 fileSize = info.size();
        QFile file(info.absoluteFilePath());
        // 尝试先改权限再删
        file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        if (!file.remove()) {
            if (!failedFiles.contains(info.absoluteFilePath())) {
                failedFiles.append(info.absoluteFilePath());
            }
        } else {
            freedSize += fileSize;
        }
    }

    return freedSize;
}