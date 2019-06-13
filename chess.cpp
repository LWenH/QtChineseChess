#include "Chess.h"

Chess::Chess()
{

}


QString Chess::getType()
{
    switch(this->_type)
    {
    case CHE:
        return "車";
    case MA:
        return "馬";
    case PAO:
        return "炮";
    case BING:
        if(this->_red)
            return "兵";
        else
            return "卒";
    case JIANG:
        if(this->_red)
            return "帅";
        else
            return "将";
    case SHI:
        return "士";
    case XIANG:
        if(this->_red)
            return "相";
        else
            return "象";
    }
    return "NON";
}

void Chess::init(int id)
{
    struct {
        int row, col;
        Chess::Type _type;
    } pos[16] = {
    {0, 0, Chess::CHE},
    {0, 1, Chess::MA},
    {0, 2, Chess::XIANG},
    {0, 3, Chess::SHI},
    {0, 4, Chess::JIANG},
    {0, 5, Chess::SHI},
    {0, 6, Chess::XIANG},
    {0, 7, Chess::MA},
    {0, 8, Chess::CHE},

    {2, 1, Chess::PAO},
    {2, 7, Chess::PAO},
    {3, 0, Chess::BING},
    {3, 2, Chess::BING},
    {3, 4, Chess::BING},
    {3, 6, Chess::BING},
    {3, 8, Chess::BING},
    };

    if(id < 16)
    {
        this->_col = pos[id].col;
        this->_row = pos[id].row;
        this->_type = pos[id]._type;
    }
    else
    {
        this->_col = 8-pos[id-16].col;
        this->_row = 9-pos[id-16].row;
        this->_type = pos[id-16]._type;
    }

    this->isDead = false;
    this->_red = id<16;
}

void Chess::map()
{
    this->_col = 8-this->_col;
    this->_row = 9-this->_row;
}
