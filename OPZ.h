#pragma once

typedef struct
{
    char op;
    char op1[4];
    int prior;
} Prior_op;
 
class OPZ
{
public:
	OPZ(char before[30]);
	int priority(char oper);
};

