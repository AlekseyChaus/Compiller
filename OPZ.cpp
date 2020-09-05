#include "OPZ.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stack>
using namespace std;
int OPZ::priority(char oper)
{
    switch (oper)
    {
    case '+': return 1;
    case '-': return 1;
    case '*': return 2;
    case '/': return 2;
    case '^': return 3;
    default: return 0;
    }
}

 OPZ::OPZ(char before [30])
{
    int z = 0;
    stack <Prior_op> store;//������� ���� �
    char after[30];
    Prior_op tmp; //������� ��������� ��������
    string inf, postfix;//������ ���������� � ������������ �����
    postfix.clear(); //������ �� ������
    inf.clear();
    inf = before; //��������� ��� �� ��� ����� (�� �����)
    int flag = 5; //�����������
    for (int i = 0, n = inf.size(); i < n; i++)//���� �� ������ �����������
    {
        if (inf.at(i) >= 'a' && inf.at(i) <= 'z' || isdigit(inf.at(i))) //���� �������� ����� ��� �����
        {
            if ((inf.at(i) == 's' && inf.at(i + 1) == 'q' && inf.at(i + 2) == 'r' && inf.at(i + 3) == 't')) // ���� � ��������� ����������� sqrt
            {
                if (inf.at(i + 5) == '-')
                {
                    postfix.push_back(inf.at(i + 6));
                    postfix.push_back(inf.at(i + 5)); z++;
                }
                else postfix.push_back(inf.at(i + 5));
            }
            else postfix.push_back(inf.at(i));//�������� � �������� ������
        }
        if (inf.at(i) == ')') flag /= 5;
        if (inf.at(i) == '(' && ((inf.at(i - 1) >= 'a' && inf.at(i - 1) <= 'z'))) //���� ����������� ������
        {
            int massiv = 0;
            while (inf.at(i) != ')' || massiv)
            {
                if (inf.at(i) == ')') massiv = 0;
                postfix.push_back(inf.at(i)); i++;
                if (inf.at(i) == '(') massiv = 1;
            }
            postfix.push_back(inf.at(i));
        }
        else if (inf.at(i) == '(') flag *= 5;
        if (inf.at(i) == '+' || inf.at(i) == '-' || inf.at(i) == '*' || inf.at(i) == '/' || inf.at(i) == '^' || (inf.at(i) == 's' && inf.at(i + 1) == 'q' && inf.at(i + 2) == 'r' && inf.at(i + 3) == 't'))
        {
            if ((inf.at(i) == 's' && inf.at(i + 1) == 'q' && inf.at(i + 2) == 'r' && inf.at(i + 3) == 't')) // ���� ������� �������� ������, �� ������� � � ����
            {
                tmp.prior = 3 * flag;
                if (store.empty())
                {
                    tmp.op = inf.at(i); store.push(tmp);
                    tmp.op = inf.at(i + 1); store.push(tmp);
                    tmp.op = inf.at(i + 2); store.push(tmp);
                    tmp.op = inf.at(i + 3); store.push(tmp);
                }
                else
                {
                    tmp.op = inf.at(i); while (!store.empty() && tmp.prior <= store.top().prior) { postfix.push_back(store.top().op); store.pop(); } store.push(tmp);
                    tmp.op = inf.at(i + 1); while (!store.empty() && tmp.prior <= store.top().prior) { postfix.push_back(store.top().op); store.pop(); } store.push(tmp);
                    tmp.op = inf.at(i + 2); while (!store.empty() && tmp.prior <= store.top().prior) { postfix.push_back(store.top().op); store.pop(); } store.push(tmp);
                    tmp.op = inf.at(i + 3); while (!store.empty() && tmp.prior <= store.top().prior) { postfix.push_back(store.top().op); store.pop(); } store.push(tmp);
                }
                i += 6 + z;
            }
            else
            {
                tmp.prior = priority(inf.at(i)) * flag;
                tmp.op = inf.at(i);
                if (store.empty()) store.push(tmp);
                else
                {
                    while (!store.empty() && tmp.prior <= store.top().prior) { postfix.push_back(store.top().op); store.pop(); }
                    store.push(tmp);
                }
            }
        }
    }
    while (!store.empty())
    {
        postfix.push_back(store.top().op);
        store.pop();
    }
    for (int i = 0; i < 29; i++)
        before[i] = ' ';
    for (int i = 0, n = postfix.size(); i < n; i++)
        before[i] = postfix[i];
}