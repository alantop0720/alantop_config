#include "configpage.h"
#include "./ui_configpage.h"

ConfigPage::ConfigPage(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigPage)
{
    ui->setupUi(this);
    ui->pageLabel->setText(labelText);

    QFont font = ui->pageLabel->font();
    font.setPointSize(14);
    ui->pageLabel->setFont(font);
}

ConfigPage::~ConfigPage()
{
    delete ui;
}

void ConfigPage::setPageLabel(const QString &text)
{
    ui->pageLabel->setText(text);
}