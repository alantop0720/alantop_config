#ifndef QCONFIGWIDGET_H
#define QCONFIGWIDGET_H

#include <QWidget>

class QTabWidget;

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

private slots:
    void onProjectSelected(int row);

private:
    void initProjects();
    QTabWidget *createConfigTabs(const QString &projectName);

    Ui::QConfigWidget *ui;
};
#endif // QCONFIGWIDGET_H