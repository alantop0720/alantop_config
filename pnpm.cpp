#include "pnpm.h"
#include "./ui_pnpm.h"

PnpmSetup::PnpmSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PnpmSetup)
{
    ui->setupUi(this);
}

PnpmSetup::~PnpmSetup()
{
    delete ui;
}