#include "sysopt.h"
#include "./ui_sysopt.h"

SysOpt::SysOpt(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SysOpt)
{
    ui->setupUi(this);
    ui->pageLabel->setText(labelText);

    QFont font = ui->pageLabel->font();
    font.setPointSize(14);
    ui->pageLabel->setFont(font);
}

SysOpt::~SysOpt()
{
    delete ui;
}

void SysOpt::setPageLabel(const QString &text)
{
    ui->pageLabel->setText(text);
}