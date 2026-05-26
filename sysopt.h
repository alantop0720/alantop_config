#ifndef SYSOPT_H
#define SYSOPT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class SysOpt;
}
QT_END_NAMESPACE

class SysOpt : public QWidget
{
    Q_OBJECT

public:
    explicit SysOpt(const QString &labelText, QWidget *parent = nullptr);
    ~SysOpt() override;

    void setPageLabel(const QString &text);

private:
    Ui::SysOpt *ui;
};

#endif // SYSOPT_H