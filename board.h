#ifndef BOARD_H
#define BOARD_H

#include "Chess.h"
#include "Step.h"
#include<QWidget>
#include <QVector>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    //32个棋子
    Chess _ch[32];

    //棋子半径
    int radius;

    QPoint offset;
    bool _bSide;

    //temp
    int row1,col1;

    QVector<Step*> _steps;

    int  _chooseId;
    bool _bRedTurn;
    void init(bool bRedSide);

    /* 绘制函数 */
    virtual void paintEvent(QPaintEvent *)override;
    void drawBoard(QPainter& p);
    void drawPlace(QPainter& p);
    void InitPainting(QPainter& p);
    void InitPainting(QPainter& p, int row, int col);
    void PaintChess(QPainter& p);
    void PaintChess(QPainter& p, int id);

    /* 坐标关系函数 */

    //返回中心像素坐标
    QPoint getCenter(int row, int col);
    QPoint getCenter(int id);

    QPoint distance(int row, int col);
    QPoint distance(int id);
    QRect unit(int row, int col);
    QRect unit(int id);

    bool getClickPos(QPoint p, int& row, int& col);
    QString type(int id);
    bool red(int id);
    bool colorJudger(int id1, int id2);
    int getChessId(int row, int col);
    void killChess(int id);
    void reliveChess(int id);
    void moveChess(int chooseidx, int row, int col);
    bool isDead(int id);
    void getCoordinate(int& _row,int& _col,int choice);

    /* 走棋 */
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectChess(int id);
    void tryMoveChess(int target, int row, int col);

    /*后台移动棋子*/
    void moveChess(int chooseidx, int target, int row, int col);

    void record(int chooseidx, int target, int row, int col, QVector<Step*>& s);

    /*悔棋*/
    void retractOne();
    void retract(Step* s);
    virtual void retract();

    /* 走法规则 */
    bool canMove(int chooseidx, int target, int row, int col);
    bool MoveableChe(int chooseidx, int target, int row, int col);
    bool MoveableMa(int chooseidx, int target, int row, int col);
    bool MoveablePao(int chooseidx, int target, int row, int col);
    bool MoveableBing(int chooseidx, int target, int row, int col);
    bool MoveableJiang(int chooseidx, int target, int row, int col);
    bool MoveableShi(int chooseidx, int target, int row, int col);
    bool MoveableXiang(int chooseidx, int target, int row, int col);

    bool canSelect(int id);
    int getLineCounter(int row1, int col1, int row2, int col2);
    int regulation(int row1, int col1, int row, int col);
    bool isBottomSide(int id);

};

#endif // BOARD_H
