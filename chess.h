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

    int id;//���ӱ��
    bool isDead;//�ж��Ƿ񱻻�ɱ
    bool _red;//�����Ƿ�Ϊ��ɫ
    int posValue = 0;
    void init(int iter);
    QString getType();
    void map();//�������ӳ��

};

#endif // CHESS_H
