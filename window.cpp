#include "window.h"
#include <QHBoxLayout>
#include "ManMachine.h"
#include "retrackButton.h"
#include <QPainter>

window::window()
{
    this->setWindowTitle("中国象棋");
    ManMachine *game = new ManMachine;
    retrackButton* btn = new retrackButton;
    QHBoxLayout* hLay = new QHBoxLayout(this);
    hLay->addWidget(game,1);
    hLay->addWidget(btn);
    this->resize(QSize( 600, 500 ));
    connect(btn, SIGNAL(SIGRETRACK()), game, SLOT(retrackSlot()));
}

void window::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(),QPixmap("2.jpg"),QRect());
}
