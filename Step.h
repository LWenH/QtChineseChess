#ifndef STEP_H
#define STEP_H

#include <QObject>

class Step : public QObject
{
public:
    explicit Step(QObject *parent = 0);
    int chooseID;
    int killID;
    int fromRow;
    int fromCol;
    int toRow;
    int toCol;
};

#endif // STEP_H
