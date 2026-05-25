#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class ConfigPage;
}
QT_END_NAMESPACE

class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPage(const QString &labelText, QWidget *parent = nullptr);
    ~ConfigPage() override;

    void setPageLabel(const QString &text);

private:
    Ui::ConfigPage *ui;
};

#endif // CONFIGPAGE_H