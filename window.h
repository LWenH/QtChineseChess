#ifndef WINDOW_H
#define WINDOW_H
#include<QWidget>

class window : public QWidget
{
    Q_OBJECT
public:
    window();
    virtual void paintEvent(QPaintEvent *)override;
};

#endif // WINDOW_H
