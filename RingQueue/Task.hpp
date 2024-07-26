#pragma once

#include <iostream>

using namespace std;

const string symbols = "+-*/%";
const int defaultret = -1;

enum
{
    RIGHT = 0,
    WRONG_DIV,
    WRONG_MOD,
    WRONG_OTHER
};

class Task
{
public:
    Task()
    {}

    Task(int x, int y, char symbol)
        : _x(x), _y(y), _symbol(symbol), _ret(defaultret), _code(RIGHT)
    {}

    void running()
    {
        switch (_symbol)
        {
        case '+':
            _ret = _x + _y;
            break;
        case '-':
            _ret = _x - _y;
            break;
        case '*':
            _ret = _x * _y;
            break;
        case '/':
        {
            if (0 == _y)
                _code = WRONG_DIV;
            else
                _ret = _x / _y;
        }
        break;
        case '%':
        {
            if (0 == _y)
                _code = WRONG_MOD;
            else
                _ret = _x % _y;
        }
        break;
        default:
            _code = WRONG_OTHER;
            break;
        }
    }

    string PrintTask()
    {
        string tmp;
        tmp += to_string(_x); // 第一个操作数
        tmp += " ";
        tmp += _symbol; // 操作符
        tmp += " ";
        tmp += to_string(_y); // 第二个操作数
        tmp += " =? ";

        return tmp;
    }

    string PrintRet()
    {
        string tmp;
        tmp += to_string(_x); // 第一个操作数
        tmp += " ";
        tmp += _symbol; // 操作符
        tmp += " ";
        tmp += to_string(_y); // 第二个操作数
        tmp += " = ";
        tmp += to_string(_ret); // 运算结果
        tmp += " ";
        tmp += ", code:";
        tmp += to_string(_code);

        return tmp;
    }

    void operator()()
    {
        running();
    }

    ~Task()
    {}

private:
    int _x;
    int _y;
    char _symbol; // 选择操作符

    int _ret;
    int _code; // 为0结果可信，为1、2、3等结果不可信
};