#ifndef RETRACKBUTTON_H
#define RETRACKBUTTON_H

#include <QWidget>
#include <QPushButton>

class retrackButton : public QWidget
{
    Q_OBJECT
public:
    explicit retrackButton(QWidget *parent = nullptr);
    QPushButton * _retracker;
signals:
    void SIGRETRACK();

public slots:
};

#endif // RETRACKBUTTON_H
