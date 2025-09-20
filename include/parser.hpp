#ifndef PARSER_H
#define PARSER_H

#include <map>

static std::map<char, int> BinOpPrecedence = {
    {'>', 10},
    {'+', 20},
    {'-', 30},
    {'*', 40},
};

void MainLoop();

#endif // !PARSER_H
