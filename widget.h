#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAxObject>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void AddWord();
    void ReadWord();
    void DeleteWord();
    void Reading();
    void StopRead();
    void AddByTxt();
    void Add(const QString &add);
private:
    Ui::Widget *ui;
    QAxObject * voiceObj;
    QStringList strList;
    QTimer * reading;
    int words = 0;

};

#endif // WIDGET_H
