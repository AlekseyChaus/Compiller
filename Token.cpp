#include "Token.h"



Token::Token(const string& lexeme) //сущность токена
{
	_lexeme = lexeme;

	// вызываем функцию, которая по лексеме возвратит ее тип
	_type = which_type_of_lexeme(lexeme);
}

token_type Token::which_type_of_lexeme(const string& lexeme)
{
	// описываем каждую возможную лексему и соответствие типу




	// если это число
	if (is_number(lexeme))
		return token_type::NUM;
	





	// в том случае, если ничего выше, то значит это нераспознанный токен
	return token_type::UNDEFINED;
}

bool Token::is_number(const string& lexeme)
{
	// проходим по всем символам лексемы
	for (const auto& s : lexeme)
		// если символ не 0-9, значит это не целое число
		if (s < '0' || s > '9')
			return false;

	// если все символы прошли проверку, то значит это число
	return true;
}
