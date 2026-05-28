#include "route.h"
#include "./ui_route.h"

#include <QProcess>

RouteSetup::RouteSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RouteSetup)
{
    ui->setupUi(this);

    connect(ui->pushButton_route_add, &QPushButton::clicked,
            this, &RouteSetup::onRouteAdd);
    connect(ui->pushButton_route_delete, &QPushButton::clicked,
            this, &RouteSetup::onRouteDelete);
    connect(ui->pushButton_route_print, &QPushButton::clicked,
            this, &RouteSetup::onRoutePrint);
    connect(ui->pushButton_ip, &QPushButton::clicked,
            this, &RouteSetup::onIp);
}

RouteSetup::~RouteSetup()
{
    delete ui;
}

void RouteSetup::onRouteAdd()
{
    runRouteCommand(ui->lineEdit_route_add->text(),
                    QStringLiteral("添加路由"));
}

void RouteSetup::onRouteDelete()
{
    runRouteCommand(ui->lineEdit_route_delete->text(),
                    QStringLiteral("删除路由"));
}

void RouteSetup::onRoutePrint()
{
    runRouteCommand(ui->lineEdit_route_print->text(),
                    QStringLiteral("显示路由"));
}

void RouteSetup::runRouteCommand(const QString &cmd, const QString &desc)
{
    ui->plainTextEdit_stat->appendPlainText(
        QStringLiteral("=== %1 ===\n命令: %2").arg(desc, cmd));

    QStringList parts = cmd.split(' ', Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        ui->plainTextEdit_stat->appendPlainText(
            QStringLiteral("错误: 命令为空\n"));
        return;
    }

    QString program = parts.takeFirst();
    QStringList args = parts;

    QProcess process;
    process.start(program, args);
    process.waitForFinished(10000);

    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());
    QString error = QString::fromLocal8Bit(process.readAllStandardError());

    if (!output.isEmpty()) {
        ui->plainTextEdit_stat->appendPlainText(output);
    }
    if (!error.isEmpty()) {
        ui->plainTextEdit_stat->appendPlainText(
            QStringLiteral("错误: %1").arg(error));
    }
    if (process.exitCode() != 0) {
        ui->plainTextEdit_stat->appendPlainText(
            QStringLiteral("退出码: %1\n").arg(process.exitCode()));
    }
}

void RouteSetup::onIp()
{
    runRouteCommand(ui->lineEdit_ip->text(),
                    QStringLiteral("IP 信息"));
}