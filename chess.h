#ifndef CHESS_H
#define CHESS_H
#include<QString>

class Chess
{
public:
    Chess();

    int _row;
    int _col;

    enum Type{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    Type _type;

    int id;//棋子编号
    bool isDead;//判断是否被击杀
    bool _red;//棋子是否为红色
    int posValue = 0;
    void init(int iter);
    QString getType();
    void map();//红黑行列映射

};

#endif // CHESS_H
