#include "Board.h"
#include <QPainter>
#include <QMouseEvent>

Board::Board(QWidget *parent) : QWidget(parent)
{
    this->radius = 22;
    setMinimumSize(radius*18+1, radius*20+1);
    init(true);
}


/*二段式初始化*/
void Board::init(bool isRedSide)
{
    for(int i=0; i<32; ++i)
    {
        _ch[i].init(i);
    }
    if(isRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            /*红黑行列映射*/
            _ch[i].map();
        }
    }

    _chooseId = -1;
    _bRedTurn = true;
    _bSide = isRedSide;
    update();
}

void Board::getCoordinate(int& _row,int& _col,int choice)
{
    _row = _ch[choice]._row;
    _col = _ch[choice]._col;
}

void Board::paintEvent(QPaintEvent *)
{
    int r = 22;
    int d= 2*r;
    radius = r;
    offset = QPoint(r+1, r+1);

    QPainter p(this);
    p.setFont(QFont("宋体", 0.7*radius, 1500));
    p.drawText(2*d,5.2*d,"楚河");
    p.drawText(6.5*d,5.2*d,"汉界");

    /*抗锯齿*/
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    p.save();
    drawBoard(p);
    p.restore();

    p.save();
    drawPlace(p);
    p.restore();

    p.save();
    InitPainting(p);
    p.restore();

    p.save();
    PaintChess(p);
    p.restore();
}

void Board::PaintChess(QPainter &p)
{
    for(int i=0; i<32; i++)
    {
        PaintChess(p, i);
    }
}

bool Board::isDead(int id)
{
    if(id == -1)return true;
    return _ch[id].isDead;
}

void Board::PaintChess(QPainter &p, int id)
{
    if(isDead(id)) return; //击杀，刷新时不显示

    QColor color;
    if(red(id)) color = Qt::red;
    else color = Qt::black;

    p.setPen(QPen(QBrush(color), 2));

    if(id == _chooseId) p.setBrush(Qt::gray);
    else p.setBrush(QBrush(QColor(254,233,177)));

    //字体方格
    p.drawEllipse(unit(id));

    p.setFont(QFont("宋体", 0.7*radius, 700));

    /*棋子名称,中心对齐*/
    p.drawText(unit(id), type(id), QTextOption(Qt::AlignCenter));

}

QString Board::type(int id)
{
    return _ch[id].getType();
}

bool Board::red(int id)
{
    return _ch[id]._red;
}

void Board::InitPainting(QPainter &p, int row, int col)
{
    QPoint pt = getCenter(row, col);
    QPoint off = QPoint(radius/6, radius/6);
    int len = radius/3;
    QPoint Start;
    QPoint End;

    if(col != 0)
    {
        /* 左上 */
        Start = QPoint(pt.x() - off.x(), pt.y() - off.y());
        End = Start + QPoint(-len, 0);
        p.drawLine(Start, End);
        End = Start + QPoint(0, -len);
        p.drawLine(Start, End);

        /* 左下 */
        Start = QPoint(pt.x() - off.x(), pt.y() + off.y());
        End = Start + QPoint(-len, 0);
        p.drawLine(Start, End);
        End = Start + QPoint(0, +len);
        p.drawLine(Start, End);
    }

    if(col != 8)
    {
        /* 右下 */
        Start = QPoint(pt.x() + off.x(), pt.y() + off.y());
        End = Start + QPoint(+len, 0);
        p.drawLine(Start, End);
        End = Start + QPoint(0, +len);
        p.drawLine(Start, End);

        /* 右上 */
        Start = QPoint(pt.x() + off.x(), pt.y() - off.y());
        End = Start + QPoint(+len, 0);
        p.drawLine(Start, End);
        End = Start + QPoint(0, -len);
        p.drawLine(Start, End);
    }
}


//竖线要多一个格子
void Board::InitPainting(QPainter &p)
{
    InitPainting(p, 3, 0);
    InitPainting(p, 3, 2);
    InitPainting(p, 3, 4);
    InitPainting(p, 3, 6);
    InitPainting(p, 3, 8);

    InitPainting(p, 6, 0);
    InitPainting(p, 6, 2);
    InitPainting(p, 6, 4);
    InitPainting(p, 6, 6);
    InitPainting(p, 6, 8);

    InitPainting(p, 2, 1);
    InitPainting(p, 2, 7);

    InitPainting(p, 7, 1);
    InitPainting(p, 7, 7);
}

//九宫格
void Board::drawPlace(QPainter &p)
{
    p.drawLine(getCenter(0, 3), getCenter(2, 5));
    p.drawLine(getCenter(2, 3), getCenter(0, 5));

    p.drawLine(getCenter(9, 3), getCenter(7, 5));
    p.drawLine(getCenter(7, 3), getCenter(9, 5));
}


void Board::drawBoard(QPainter &p)
{
    for(int i=0; i<10; ++i)
    {
        if(i==0 || i==9)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
        }
        p.drawLine(getCenter(i, 0), getCenter(i, 8));
    }

    for(int i=0; i<9; ++i)
    {
        if(i==0 || i==8)
        {
            p.setPen(QPen(Qt::black, 3, Qt::SolidLine));
            p.drawLine(getCenter(0, i), getCenter(9, i));
        }
        else
        {
            p.setPen(QPen(Qt::black, 1, Qt::SolidLine));
            p.drawLine(getCenter(0, i), getCenter(4, i));
            p.drawLine(getCenter(5, i), getCenter(9, i));
        }
    }
}

QPoint Board::getCenter(int row, int col)
{
    QPoint pt(radius*col*2, radius*row*2);
    return pt + offset;
}

QPoint Board::getCenter(int id)
{
    return getCenter(_ch[id]._row, _ch[id]._col);
}

QPoint Board::distance(int row, int col)
{
    return getCenter(row, col) - QPoint(radius, radius);
}

QPoint Board::distance(int id)
{
    return getCenter(id) - QPoint(radius, radius);
}

QRect Board::unit(int row, int col)
{
    return QRect(distance(row, col), QSize(radius*2-1, radius*2-1));
}

QRect Board::unit(int id)
{
    return QRect(distance(id), QSize(radius*2-1, radius*2-1));
}

bool Board::getClickPos(QPoint p, int &row, int &col)
{
    for(row=0; row<=9; ++row)
    {
        for(col=0; col<=8; ++col)
        {
            QPoint distance = getCenter(row, col) - p;
            if(distance.x() * distance.x() + distance.y() * distance.y() < radius* radius)
                return true;
        }
    }
    return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _ch[id]._red;
}

void Board::trySelectChess(int id)
{
    if(id == -1)
        return;

    if(!canSelect(id)) return;

    _chooseId = id;
    update();
}

bool Board::colorJudger(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;

    return red(id1) == red(id2);
}

void Board::tryMoveChess(int target, int row, int col)
{
    if(target != -1 && colorJudger(target, _chooseId))
    {
        trySelectChess(target);
        return;
    }

    bool ret = canMove(_chooseId, target, row, col);
    if(ret)
    {
        moveChess(_chooseId, target, row, col);
        _chooseId = -1;
        update();
    }
}

/*获取横竖线上的棋子个数，为車，炮的移动做准备*/
int Board::getLineCounter(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    if(row1 != row2 && col1 != col2)
        return -1;
    if(row1 == row2 && col1 == col2)
        return -1;

    if(row1 == row2)
    {
        int min = col1 < col2 ? col1 : col2;
        int max = col1 > col2 ? col1 : col2;
        for(int col = min+1; col<max; ++col)
        {
            if(getChessId(row1, col) != -1) ++ret;
        }
    }
    else
    {
        int min = row1 < row2 ? row1 : row2;
        int max = row1 > row2 ? row1 : row2;
        for(int row = min+1; row<max; ++row)
        {
            if(getChessId(row, col1) != -1) ++ret;
        }
    }

    return ret;
}

//规则限定表达式
int Board::regulation(int row1, int col1, int row, int col)
{
    return abs(row1-row)*10+abs(col1-col);
}

bool Board::MoveableChe(int chooseidx, int, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int ret = getLineCounter(row1, col1, row, col);
    if(ret == 0)
        return true;
    return false;
}

bool Board::MoveableMa(int chooseidx, int, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int r = regulation(row1, col1, row, col);
    if(r != 12 && r != 21)
        return false;

    if(r == 12)
    {
        //横拌马腿
        if(getChessId(row1, (col+col1)/2) != -1)
            return false;
    }
    else
    {
        //竖拌马腿
        if(getChessId((row+row1)/2, col1) != -1)
            return false;
    }

    return true;
}

bool Board::MoveablePao(int chooseidx, int target, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int ret = getLineCounter(row, col, row1, col1);
    if(target != -1)    //除目标外，目标的行列只有一只棋子
    {
        if(ret == 1) return true;
    }
    else                //无棋子，自由移动
    {
        if(ret == 0) return true;
    }
    return false;
}

bool Board::MoveableBing(int chooseidx, int, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int r = regulation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    //不能往后走
    if(isBottomSide(chooseidx))
    {
        if(row > row1) return false;
        //未过河时不能横着走
        if(row1 >= 5 && row == row1) return false;
    }
    else
    {
        if(row < row1) return false;
        if(row1 <= 4 && row == row1) return false;
    }

    return true;
}

bool Board::MoveableJiang(int chooseidx, int target, int row, int col)
{
    /*飞帅*/
    if(target != -1 && _ch[target]._type == Chess::JIANG)
        return MoveableChe(chooseidx, target, row, col);
    /*步长为一，不出九宫格*/
    getCoordinate(row1, col1, chooseidx);
    int r = regulation(row1, col1, row, col);
    if(r != 1 && r != 10) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(chooseidx))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

bool Board::MoveableShi(int chooseidx, int, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int r = regulation(row1, col1, row, col);
    if(r != 11) return false;

    if(col < 3 || col > 5) return false;
    if(isBottomSide(chooseidx))
    {
        if(row < 7) return false;
    }
    else
    {
        if(row > 2) return false;
    }
    return true;
}

//放置一个哑元
bool Board::MoveableXiang(int chooseidx, int, int row, int col)
{
    getCoordinate(row1, col1, chooseidx);
    int r = regulation(row1, col1, row, col);
    if(r != 22) return false;

    int rEye = (row+row1)/2;
    int cEye = (col+col1)/2;
    /*拌象脚*/
    if(getChessId(rEye, cEye) != -1)
        return false;

    /*象不能过河*/
    if(isBottomSide(chooseidx))
    {
        if(row < 4) return false;
    }
    else
    {
        if(row > 5) return false;
    }
    return true;
}

bool Board::canMove(int chooseidx, int target, int row, int col)
{
    if(colorJudger(chooseidx, target)) return false;

    switch (_ch[chooseidx]._type)
    {
    case Chess::CHE:
        return MoveableChe(chooseidx, target, row, col);

    case Chess::MA:
        return MoveableMa(chooseidx, target, row, col);

    case Chess::PAO:
        return MoveablePao(chooseidx, target, row, col);

    case Chess::BING:
        return MoveableBing(chooseidx, target, row, col);

    case Chess::JIANG:
        return MoveableJiang(chooseidx, target, row, col);

    case Chess::SHI:
        return MoveableShi(chooseidx, target, row, col);

    case Chess::XIANG:
        return MoveableXiang(chooseidx, target, row, col);

    }
    return false;
}

void Board::reliveChess(int id)
{
    if(id==-1) return;
    _ch[id].isDead = false;
}

void Board::killChess(int id)
{
    if(id==-1) return;
    _ch[id].isDead = true;
}

bool Board::isBottomSide(int id)
{
    return _bSide == _ch[id]._red;
}

void Board::moveChess(int chooseidx, int row, int col)
{
    _ch[chooseidx]._row = row;
    _ch[chooseidx]._col = col;

    _bRedTurn = !_bRedTurn;
}

//记录步数及相关信息
void Board::record(int chooseidx, int target, int row, int col, QVector<Step*>& s)
{
    getCoordinate(row1, col1, chooseidx);
    Step* step = new Step;
    step->fromCol = col1;
    step->toCol = col;
    step->fromRow = row1;
    step->toRow = row;
    step->chooseID = chooseidx;
    step->killID = target;
    s.push_back(step);
}

void Board::moveChess(int chooseidx, int target, int row, int col)
{
    record(chooseidx, target, row, col, _steps);

    killChess(target);
    moveChess(chooseidx, row, col);
}

void Board::click(int id, int row, int col)
{
    if(this->_chooseId == -1)
    {
        trySelectChess(id);
    }
    else
    {
        tryMoveChess(id, row, col);
    }
}

void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getClickPos(pt, row, col);
    if(!bClicked) return;

    int id = getChessId(row, col);
    click(id, row, col);
}

int Board::getChessId(int row, int col)
{

    //反算象棋的id
    for(int i=0; i<32; ++i)
    {
        if(_ch[i]._row == row && _ch[i]._col == col && !isDead(i))
            return i;
    }
    //该点无象棋
    return -1;
}

void Board::retract(Step *s)
{
    reliveChess(s->killID);
    moveChess(s->chooseID, s->fromRow, s->fromCol);
}

void Board::retractOne()
{
    if(this->_steps.size() == 0) return;

    Step* step = this->_steps.last();
    _steps.removeLast();
    retract(step);

    update();
    delete step;
}

void Board::retract()
{
    retractOne();
}

