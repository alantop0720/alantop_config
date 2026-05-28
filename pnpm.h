#ifndef PNPM_H
#define PNPM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class PnpmSetup;
}
QT_END_NAMESPACE

class PnpmSetup : public QWidget
{
    Q_OBJECT

public:
    explicit PnpmSetup(const QString &labelText, QWidget *parent = nullptr);
    ~PnpmSetup() override;

private slots:
    void onLs();
    void onRoot();
    void onRm();
    void onUninstall();

private:
    void runPnpmCommand(const QString &cmd, const QString &desc);

    Ui::PnpmSetup *ui;
};

#endif // PNPM_H