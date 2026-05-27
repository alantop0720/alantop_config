#include "uv.h"
#include "./ui_uv.h"

#include <QDir>
#include <QFileDialog>
#include <QProcess>
#include <QSettings>
#include <QStandardPaths>

UVSetup::UVSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UVSetup)
{
    ui->setupUi(this);
    ui->pageLabel->setText(labelText);

    QFont font = ui->pageLabel->font();
    font.setPointSize(14);
    ui->pageLabel->setFont(font);

    initUV();

    connect(ui->pushButton_cache_dir, &QPushButton::clicked,
            this, &UVSetup::onBrowseCacheDir);
    connect(ui->pushButton_python_dir, &QPushButton::clicked,
            this, &UVSetup::onBrowsePythonDir);
    connect(ui->pushButton_set, &QPushButton::clicked,
            this, &UVSetup::onSetEnvVars);
    connect(ui->pushButton_read, &QPushButton::clicked,
            this, &UVSetup::onReadEnvVars);
    connect(ui->pushButton_select_dir, &QPushButton::clicked,
            this, &UVSetup::onSelectDirAndInit);
}

UVSetup::~UVSetup()
{
    delete ui;
}

void UVSetup::setPageLabel(const QString &text)
{
    ui->pageLabel->setText(text);
}

void UVSetup::initUV()
{
    ui->lineEdit_CACHE_DIR->setText(runUVCommand("cache dir"));
    ui->lineEdit_PTHON_DIR->setText(runUVCommand("python dir"));
    ui->lineEdit_tool_dir->setText(runUVCommand("tool dir --bin"));



}

QString UVSetup::runUVCommand(const QString &args)
{
    QProcess process;
    process.start("uv", QStringList() << args.split(' '));
    process.waitForFinished(5000);

    if (process.exitCode() == 0) {
        return QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    }
    return QString::fromUtf8(process.readAllStandardError()).trimmed();
}

void UVSetup::onBrowseCacheDir()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, QStringLiteral("选择 UV 缓存目录"),
        ui->lineEdit_NEW_CACHE_DIR->text());
    if (!dir.isEmpty()) {
        ui->lineEdit_NEW_CACHE_DIR->setText(dir);
    }
}

void UVSetup::onBrowsePythonDir()
{
    QString dir = QFileDialog::getExistingDirectory(
        this, QStringLiteral("选择 UV Python 安装目录"),
        ui->lineEdit_PYTHON_DIR->text());
    if (!dir.isEmpty()) {
        ui->lineEdit_PYTHON_DIR->setText(dir);
    }
}

void UVSetup::onSetEnvVars()
{
    QString cacheDir = ui->lineEdit_NEW_CACHE_DIR->text();
    QString pythonDir = ui->lineEdit_PYTHON_DIR->text();

    // 检测目录是否存在，不存在则创建
    QDir dir;
    if (!dir.exists(cacheDir)) {
        dir.mkpath(cacheDir);
    }
    if (!dir.exists(pythonDir)) {
        dir.mkpath(pythonDir);
    }

    QStringList failedVars;
    auto checkAndSet = [&](const QString &name, const QString &value) {
        if (!setUserEnvVar(name, value)) {
            failedVars.append(name);
        }
    };

    checkAndSet(QStringLiteral("UV_CACHE_DIR"), cacheDir);
    checkAndSet(QStringLiteral("UV_PYTHON_INSTALL_DIR"), pythonDir);
    checkAndSet(QStringLiteral("UV_DEFAULT_INDEX"),
                ui->lineEdit_default_index->text());
    checkAndSet(QStringLiteral("UV_PYTHON_INSTALL_MIRROR"),
                ui->lineEdit_install_mirror->text());

    if (failedVars.isEmpty()) {
        ui->statusLabel->setStyleSheet(
            QStringLiteral("color: green;"));
        ui->statusLabel->setText(QStringLiteral("  操作成功"));
    } else {
        ui->statusLabel->setStyleSheet(
            QStringLiteral("color: red;"));
        ui->statusLabel->setText(
            QStringLiteral("  设置失败: %1").arg(failedVars.join(QStringLiteral(", "))));
    }
}

bool UVSetup::setUserEnvVar(const QString &name, const QString &value)
{
    QProcess process;
    process.start("setx", QStringList() << name << value);
    process.waitForFinished(5000);
    return process.exitCode() == 0;
}

void UVSetup::onReadEnvVars()
{
    ui->lineEdit_default_index->setText(
        getUserEnvVar(QStringLiteral("UV_DEFAULT_INDEX")));
    ui->lineEdit_install_mirror->setText(
        getUserEnvVar(QStringLiteral("UV_PYTHON_INSTALL_MIRROR")));
}

QString UVSetup::getUserEnvVar(const QString &name)
{
    QSettings envSettings(
        QStringLiteral("HKEY_CURRENT_USER\\Environment"),
        QSettings::NativeFormat);
    return envSettings.value(name).toString();
}

void UVSetup::onSelectDirAndInit()
{
    const QString desktop = QStandardPaths::writableLocation(
        QStandardPaths::DesktopLocation);

    QString dir = QFileDialog::getExistingDirectory(
        this, QStringLiteral("选择 uv 项目目录"), desktop);
    if (dir.isEmpty()) {
        return;
    }

    ui->lineEdit_project_dir->setText(dir);

    QString cmd = ui->lineEdit_uv_init->text().trimmed();
    if (cmd.isEmpty()) {
        ui->statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        ui->statusLabel->setText(QStringLiteral("  初始化命令为空"));
        return;
    }

    // 分割命令为程序名和参数
    QStringList parts = cmd.split(' ', Qt::SkipEmptyParts);
    QString program = parts.takeFirst();
    QStringList args = parts;

    QProcess process;
    process.setWorkingDirectory(dir);
    process.start(program, args);
    process.waitForFinished(60000);

    if (process.exitCode() == 0) {
        ui->statusLabel->setStyleSheet(QStringLiteral("color: green;"));
        ui->statusLabel->setText(QStringLiteral("  uv 项目初始化成功"));
    } else {
        QString err = QString::fromUtf8(process.readAllStandardError()).trimmed();
        ui->statusLabel->setStyleSheet(QStringLiteral("color: red;"));
        ui->statusLabel->setText(
            QStringLiteral("  初始化失败: %1").arg(err));
    }
}