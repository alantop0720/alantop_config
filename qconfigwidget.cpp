#include "qconfigwidget.h"
#include "./ui_qconfigwidget.h"

#include <QListWidgetItem>

QConfigWidget::QConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QConfigWidget)
{
    ui->setupUi(this);

    // 初始化左右分栏比例 3:7
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);

    initProjects();
    initConfigTabs();

    connect(ui->projectList, &QListWidget::currentRowChanged,
            this, &QConfigWidget::onProjectSelected);
}

QConfigWidget::~QConfigWidget()
{
    delete ui;
}

void QConfigWidget::initProjects()
{
    QStringList projects = {
        QStringLiteral("项目 Alpha"),
        QStringLiteral("项目 Beta"),
        QStringLiteral("项目 Gamma"),
        QStringLiteral("项目 Delta")
    };

    for (const auto &name : projects) {
        auto *item = new QListWidgetItem(name, ui->projectList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
}

void QConfigWidget::initConfigTabs()
{
    // 创建几个示例配置 Tab 页
    ui->configTabs->addTab(new QWidget(), QStringLiteral("基本设置"));
    ui->configTabs->addTab(new QWidget(), QStringLiteral("编译选项"));
    ui->configTabs->addTab(new QWidget(), QStringLiteral("运行参数"));
    ui->configTabs->addTab(new QWidget(), QStringLiteral("高级配置"));
}

void QConfigWidget::onProjectSelected(int row)
{
    if (row < 0)
        return;

    auto *item = ui->projectList->item(row);
    if (item) {
        // 选中项目时自动切到第一个配置 Tab
        ui->configTabs->setCurrentIndex(0);
    }
}