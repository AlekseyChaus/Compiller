#pragma once
//Класс исключения ошибки 
#include <string>
#include <iostream>
using namespace std;

class Err {
public:
	inline Err(int a, string str)
	{
		switch (a)
		{
		case 1:
			cout << "Lexer "; break;
		case 2:
			cout << "Parser "; break;
		default:
			cout << "Critical "; break;
		}
		cout << "error: " << str << endl;
	}

};
