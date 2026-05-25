#include "uv.h"
#include "./ui_uv.h"

#include <QDir>
#include <QFileDialog>
#include <QProcess>

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

    setUserEnvVar(QStringLiteral("UV_CACHE_DIR"), cacheDir);
    setUserEnvVar(QStringLiteral("UV_PYTHON_INSTALL_DIR"), pythonDir);
}

void UVSetup::setUserEnvVar(const QString &name, const QString &value)
{
    QProcess process;
    process.start("setx", QStringList() << name << value);
    process.waitForFinished(5000);
}