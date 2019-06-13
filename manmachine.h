#ifndef MANMACHINE_H
#define MANMACHINE_H
#include "board.h"
#include "Step.h"

class ManMachine : public Board
{
    Q_OBJECT
public:
    ManMachine();

    int stepLevel;

    Step* decisionMaking();

    virtual void click(int id, int row, int col)override;
    void pushStep(QVector<Step *>& s);
    void evaluateMove(Step *s);
    void recoverChess(Step *s);
    int evaluateScore();
    int isGameOver(int);
    int negaMax(int depth);
    int alphabeta(int depth,int alpha,int beta);
    int evalPiecePosition(int p,int row,int col);
public slots:
    void computerMove();
    void retrackSlot();

};

#endif // MANMACHINE_H
