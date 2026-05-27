#include "route.h"
#include "./ui_route.h"

#include <QFont>

RouteSetup::RouteSetup(const QString &labelText, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RouteSetup)
{
    ui->setupUi(this);

}

RouteSetup::~RouteSetup()
{
    delete ui;
}

void RouteSetup::setPageLabel(const QString &text)
{

}