#include "Move.h"
#include <string>

Move::Move(std::string lan) : mMove(0)
{
    // setting special move flags
    if (lan.length() == 5) {
        switch (lan[4]) {
            case 'b':
                mMove |= 1 << 12;
                break;
            case 'r':
                mMove |= 1 << 13;
            case 'q':
                mMove |= (1 << 12) | (1 << 13);
        }
        mMove |= 1 << 15;
    }   
}