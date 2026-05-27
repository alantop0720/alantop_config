#ifndef ROUTE_H
#define ROUTE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class RouteSetup;
}
QT_END_NAMESPACE

class RouteSetup : public QWidget
{
    Q_OBJECT

public:
    explicit RouteSetup(const QString &labelText, QWidget *parent = nullptr);
    ~RouteSetup() override;

    void setPageLabel(const QString &text);

private slots:
    void onRouteAdd();
    void onRouteDelete();
    void onRoutePrint();

private:
    void runRouteCommand(const QString &cmd, const QString &desc);

    Ui::RouteSetup *ui;
};

#endif // ROUTE_H