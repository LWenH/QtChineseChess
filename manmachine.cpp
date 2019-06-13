#include "ManMachine.h"
#include <QTimer>
#include <QDebug>
#include<QMessageBox>

const int pPosition [10][9] = {
                {6, 4, 0, -10, -12, -10, 0, 4, 6},
                {2, 2, 0, -4, -14, -4, 0, 2, 2},
                {2, 2, 0, -10, -8, -10, 0, 2, 2},
                {0, 0, -2, 4, 10, 4, -2, 0, 0},
                {0, 0, 0, 2, 8, 2, 0, 0, 0},
                {-2, 0, 4, 2, 6, 2, 4, 0, -2},
                {0, 0, 0, 2, 4, 2, 0, 0, 0},
                {4, 0, 8, 6, 10, 6, 8, 0, 4},
                {0, 2, 4, 6, 6, 6, 4, 2, 0},
                {0, 0, 2, 6, 6, 6, 2, 0, 0},
        };
const  int mPosition [10][9] = {
                {4, 8, 16, 12, 4, 12, 16, 8, 4},
                {4, 10, 28, 16, 8, 16, 28, 10, 4},
                {12, 14, 16, 20, 18, 20, 16, 14, 12},
                {8, 24, 18, 24, 20, 24, 18, 24, 8},
                {6, 16, 14, 18, 16, 18, 14, 16, 6},
                {4, 12, 16, 14, 12, 14, 16, 12, 4},
                {2, 6, 8, 6, 10, 6, 8, 6, 2},
                {4, 2, 8, 8, 4, 8, 8, 2, 4},
                {0, 2, 4, 4, -2, 4, 4, 2, 0},
                {0, -4, 0, 0, 0, 0, 0, -4, 0},
        };
const int jPosition [10][9] = {
                {14, 14, 12, 18, 16, 18, 12, 14, 14},
                {16, 20, 18, 24, 26, 24, 18, 20, 16},
                {12, 12, 12, 18, 18, 18, 12, 12, 12},
                {12, 18, 16, 22, 22, 22, 16, 18, 12},
                {12, 14, 12, 18, 18, 18, 12, 14, 12},
                {12, 16, 14, 20, 20, 20, 14, 16, 12},
                {6, 10, 8, 14, 14, 14, 8, 10, 6},
                {4, 8, 6, 14, 12, 14, 6, 8, 4},
                {8, 4, 8, 16, 8, 16, 8, 4, 8},
                {-2, 10, 6, 14, 12, 14, 6, 10, -2},
        };

const int zPosition[10][9] = {
                {0, 3, 6, 9, 12, 9, 6, 3, 0},
                {18, 36, 56, 80, 120, 80, 56, 36, 18},
                {14, 26, 42, 60, 80, 60, 42, 26, 14},
                {10, 20, 30, 34, 40, 34, 30, 20, 10},
                {6, 12, 18, 18, 20, 18, 18, 12, 6},
                {2, 0, 8, 0, 8, 0, 8, 0, 2},
                {0, 0, -2, 0, 4, 0, -2, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0},
        };

ManMachine::ManMachine()
{
    this->stepLevel = 4;
}


void ManMachine::retrackSlot()
{
    if(_bRedTurn)
    {
        retract();
        retractOne();
    }
}

void ManMachine::click(int id, int row, int col)
{
    if(_bRedTurn)
    {
        Board::click(id, row, col);
        if(_ch[20].isDead == true)
        {
            QMessageBox::warning(NULL, NULL, QStringLiteral("红方胜利"), QMessageBox::Ok, 0);
            exit(0);
        }
        if(!_bRedTurn)
        {
            QTimer::singleShot(100, this, SLOT(computerMove()));
        }
    }
}

void ManMachine::computerMove()
{
    Step* step = decisionMaking();
    moveChess(step->chooseID, step->killID, step->toRow, step->toCol);
    delete step;
    update();
    if(_ch[4].isDead == true)
    {
        QMessageBox::warning(NULL, NULL, QStringLiteral("黑方胜利"), QMessageBox::Ok, 0);
        exit(0);
    }
}

Step* ManMachine::decisionMaking()
{

    Step* ret = nullptr;
    QVector<Step*> steps;
    pushStep(steps);
    int maxScore = -100000;

    while(steps.count())
    {
        Step* temp = steps.last();
        steps.removeLast();

        evaluateMove(temp);
        int minScore = -alphabeta(this->stepLevel-1,maxScore,-maxScore);
        recoverChess(temp);

        if(minScore > maxScore)
        {
            if(ret) delete ret;

            ret = temp;
            maxScore = minScore;
        }
        else
        {
            delete temp;
        }
    }
    return ret;
}

int ManMachine::evalPiecePosition(int p,int row,int col)
{
    int t = _ch[p]._type;
    if (t==  Chess::MA) return mPosition[row][col];
    if (t == Chess::CHE) return jPosition[row][col];
    if (t == Chess::PAO) return pPosition[row][col];
    if (t == Chess::BING) return zPosition[row][col];
    return 0;
}

int ManMachine::evaluateScore()
{
//    static int s[] = {1000, 499, 501, 200, 15000, 100, 100};
    int pieceValue[] = {600,300,300,70,1000000,110,110};
    int values[2][2] = {{0,0},{0,0}};
    int scoreBlack = 0;
    int scoreRed = 0;
    for(int i=0; i<16; ++i)
    {
        if(_ch[i].isDead) continue;
//        scoreRed += s[_ch[i]._type];
        getCoordinate(row1,col1,i);
        _ch[i].posValue = evalPiecePosition(i,row1,col1);
        values[0][0]+=pieceValue[_ch[i]._type];
        values[0][1]+=_ch[i].posValue;
    }
    for(int i=16; i<32; ++i)
    {
        if(_ch[i].isDead) continue;
//        scoreBlack += s[_ch[i]._type];
        getCoordinate(row1,col1,i);
        _ch[i].posValue = evalPiecePosition(i,9-row1,8-col1);
        values[1][0]+=pieceValue[_ch[i]._type];
        values[1][1]+=_ch[i].posValue;
    }
    scoreRed = values[0][0]+values[0][1];
    scoreBlack = values[1][0]+values[1][1];
    return scoreBlack - scoreRed;
}


void ManMachine::evaluateMove(Step *step)
{
    killChess(step->killID);
    moveChess(step->chooseID, step->toRow, step->toCol);
}

void ManMachine::recoverChess(Step *step)
{
    reliveChess(step->killID);
    moveChess(step->chooseID, step->fromRow, step->fromCol);
}

void ManMachine::pushStep(QVector<Step *> &steps)
{
    int min, max;
    if(this->_bRedTurn)
    {
        min = 0, max = 16;
    }
    else
    {
        min = 16, max = 32;
    }

    for(int i=min;i<max; i++)
    {
        if(this->_ch[i].isDead) continue;
        for(int row = 0; row<=9; ++row)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = this->getChessId(row, col);
                if(colorJudger(i, killid)) continue;

                if(canMove(i, killid, row, col))
                {
                    /*保存可行步数*/
                    record(i, killid, row, col, steps);
                }
            }
        }
    }
}

int ManMachine::isGameOver(int depth)
{
    int i,j;
    bool RedLive = false;
    bool BlackLive = false;

    //red,判断九宫格里是否有将帅
    for(i=7;i<10;++i)
    {
        for(j=3;j<6;++j)
        {
            if(getChessId(i,j) == 20)
                BlackLive = true;
            if(getChessId(i,j) == 4)
                RedLive = true;
        }
    }

    for(i=0;i<3;++i)
    {
        for(j=3;j<6;++j)
        {
            if(getChessId(i,j) == 20)
                BlackLive = true;
            if(getChessId(i,j) == 4)
                RedLive = true;
        }
    }

    i = (this->stepLevel-depth+1)%2;
    if(!RedLive)
    {
        if(i)
            return 99990+depth;
        else
            return -99990-depth;
    }

    if(!BlackLive)
    {
        if(i)
            return -99990-depth;
        else
            return 99990+depth;
    }
    return 0;
}


int ManMachine::alphabeta(int depth, int alpha, int beta)
{
    int current = -100000;
    if(depth <= 0)
        return this->evaluateScore();
    int i = isGameOver(depth);
    if(i != 0)
        return i;
    QVector<Step*> steps;
    pushStep(steps);
    int score = 0;
    while (steps.count())
    {
        Step *t = steps.last();
        steps.removeLast();
        evaluateMove(t);
        score = -alphabeta(depth-1,-beta,-alpha);
        recoverChess(t);
        delete t;
        if(score > current)
        {
            current = score;
            if(score >= alpha)
                alpha = score;
            if(score >= beta)
                break;
        }
    }
    return current;
}

int ManMachine::negaMax(int depth)
{
    if(depth == 0)
        return evaluateScore();
    QVector<Step*> steps;
    pushStep(steps);
    int bestValue = -100000,value = -100000;
    while (steps.count())
    {
        Step *t = steps.last();
        steps.removeLast();
        evaluateMove(t);
        value = -negaMax(depth - 1);
        recoverChess(t);
        if(value > bestValue)
            bestValue = value;
        delete t;
    }
    return bestValue;
}
