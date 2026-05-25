#include "qconfigwidget.h"
#include "./ui_qconfigwidget.h"

QConfigWidget::QConfigWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QConfigWidget)
{
    ui->setupUi(this);
}

QConfigWidget::~QConfigWidget()
{
    delete ui;
}
