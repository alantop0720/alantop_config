#ifndef UV_H
#define UV_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class UVSetup;
}
QT_END_NAMESPACE

class UVSetup : public QWidget
{
    Q_OBJECT

public:
    explicit UVSetup(const QString &labelText, QWidget *parent = nullptr);
    ~UVSetup() override;

    void setPageLabel(const QString &text);

private slots:
    void onBrowseCacheDir();
    void onBrowsePythonDir();
    void onSetEnvVars();

private:
    void initUV();
    QString runUVCommand(const QString &args);
    void setUserEnvVar(const QString &name, const QString &value);

    Ui::UVSetup *ui;
};

#endif // UV_H