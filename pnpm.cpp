#include "pnpm.h"
#include "./ui_pnpm.h"

#include <QProcess>

PnpmSetup::PnpmSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PnpmSetup)
{
    ui->setupUi(this);

    connect(ui->pushButton_ls, &QPushButton::clicked,
            this, &PnpmSetup::onLs);
    connect(ui->pushButton_root, &QPushButton::clicked,
            this, &PnpmSetup::onRoot);
    connect(ui->pushButton_rm, &QPushButton::clicked,
            this, &PnpmSetup::onRm);
    connect(ui->pushButton_uninstall, &QPushButton::clicked,
            this, &PnpmSetup::onUninstall);
}

PnpmSetup::~PnpmSetup()
{
    delete ui;
}

void PnpmSetup::onLs()
{
    runPnpmCommand(ui->lineEdit_ls->text(),
                   QStringLiteral("查看包"));
}

void PnpmSetup::onRoot()
{
    runPnpmCommand(ui->lineEdit_root->text(),
                   QStringLiteral("查看目录"));
}

void PnpmSetup::onRm()
{
    runPnpmCommand(ui->lineEdit_rm->text(),
                   QStringLiteral("删除包"));
}

void PnpmSetup::onUninstall()
{
    runPnpmCommand(ui->lineEdit_uninstall->text(),
                   QStringLiteral("卸载pnpm"));
}

void PnpmSetup::runPnpmCommand(const QString &cmd, const QString &desc)
{
    ui->plainTextEdit_state->appendPlainText(
        QStringLiteral("=== %1 ===\n命令: %2").arg(desc, cmd));

    if (cmd.trimmed().isEmpty()) {
        ui->plainTextEdit_state->appendPlainText(
            QStringLiteral("错误: 命令为空\n"));
        return;
    }

    QProcess process;
    process.start("cmd", QStringList() << "/c" << cmd);
    process.waitForFinished(30000);

    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QString error = QString::fromLocal8Bit(process.readAllStandardError());

    if (!output.isEmpty()) {
        ui->plainTextEdit_state->appendPlainText(output);
    }
    if (!error.isEmpty()) {
        ui->plainTextEdit_state->appendPlainText(
            QStringLiteral("错误: %1").arg(error));
    }
    if (process.exitCode() != 0) {
        ui->plainTextEdit_state->appendPlainText(
            QStringLiteral("退出码: %1\n").arg(process.exitCode()));
    }
}