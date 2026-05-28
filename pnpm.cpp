#include "pnpm.h"
#include "./ui_pnpm.h"

#include <QFont>

PnpmSetup::PnpmSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PnpmSetup)
{
    ui->setupUi(this);
    ui->pageLabel->setText(labelText);

    QFont font = ui->pageLabel->font();
    font.setPointSize(14);
    ui->pageLabel->setFont(font);
}

PnpmSetup::~PnpmSetup()
{
    delete ui;
}

void PnpmSetup::setPageLabel(const QString &text)
{
    ui->pageLabel->setText(text);
}