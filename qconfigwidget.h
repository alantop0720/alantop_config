#ifndef QCONFIGWIDGET_H
#define QCONFIGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class QConfigWidget;
}
QT_END_NAMESPACE

class QConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QConfigWidget(QWidget *parent = nullptr);
    ~QConfigWidget() override;

private:
    Ui::QConfigWidget *ui;
};
#endif // QCONFIGWIDGET_H
