#include "retrackButton.h"
#include <QGridLayout>
#include <QVBoxLayout>

retrackButton::retrackButton(QWidget *parent) : QWidget(parent)
{
    QGridLayout *grid = new QGridLayout(this);
    QVBoxLayout *box = new QVBoxLayout();
    grid->setColumnStretch(0,1);
    grid->setColumnStretch(2,1);
    grid->setRowStretch(0, 1);
    grid->setRowStretch(2, 1);
    grid->addLayout(box, 1, 1);
    box->addWidget(this->_retracker = new QPushButton("悔棋"));
    connect(this->_retracker, SIGNAL(clicked()), this, SIGNAL(SIGRETRACK()));
}
