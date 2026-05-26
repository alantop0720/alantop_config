#include "qconfigwidget.h"
#include "./ui_qconfigwidget.h"
#include "configpage.h"
#include "sysopt.h"
#include "uv.h"

#include <QListWidgetItem>
#include <QStackedWidget>
#include <QTabWidget>

QConfigWidget::QConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QConfigWidget)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);
    ui->splitter->setSizes({300, 700});

    initProjects();

    connect(ui->projectList, &QListWidget::currentRowChanged,
            this, &QConfigWidget::onProjectSelected);

    // 默认选中第一个项目
    if (ui->projectList->count() > 0) {
        ui->projectList->setCurrentRow(0);
    }
}

QConfigWidget::~QConfigWidget()
{
    delete ui;
}

void QConfigWidget::initProjects()
{
    QStringList projects = {
        QStringLiteral("系统优化"),
        QStringLiteral("uv")
    };

    for (const auto &name : projects) {
        auto *item = new QListWidgetItem(name, ui->projectList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);

        // 每个项目创建一套独立的 Tab 页，放入 QStackedWidget
        QTabWidget *tabs = createConfigTabs(name);
        ui->configStack->addWidget(tabs);
    }
}

QTabWidget *QConfigWidget::createConfigTabs(const QString &projectName)
{
    auto *tabs = new QTabWidget();

    struct TabInfo {
        QString title;
        QString desc;
    };

    QList<TabInfo> tabInfos = {
        { QStringLiteral("基本设置"), projectName + QStringLiteral(" - 基本设置页") },
        { QStringLiteral("编译选项"), projectName + QStringLiteral(" - 编译选项页") },
        { QStringLiteral("运行参数"), projectName + QStringLiteral(" - 运行参数页") },
        { QStringLiteral("高级配置"), projectName + QStringLiteral(" - 高级配置页") }
    };

    for (const auto &info : tabInfos) {
        // uv 项目只保留"基本设置"，使用独立的 UVSetup 页面（uv.ui）
        if (projectName == QStringLiteral("uv")) {
            auto *page = new UVSetup(info.desc);
            tabs->addTab(page, info.title);
            break;
        }
        // 系统优化项目只保留"基本设置"，使用独立的 SysOpt 页面（sysopt.ui）
        if (projectName == QStringLiteral("系统优化")) {
            auto *page = new SysOpt(info.desc);
            tabs->addTab(page, info.title);
            break;
        }
        auto *page = new ConfigPage(info.desc);
        tabs->addTab(page, info.title);
    }

    return tabs;
}

void QConfigWidget::onProjectSelected(int row)
{
    if (row < 0 || row >= ui->configStack->count())
        return;

    // 切换到对应项目的 Tab 页集
    ui->configStack->setCurrentIndex(row);
}