#include "Token.h"



Token::Token(const string& lexeme) //�������� ������
{
	_lexeme = lexeme;

	// �������� �������, ������� �� ������� ��������� �� ���
	_type = which_type_of_lexeme(lexeme);
}

token_type Token::which_type_of_lexeme(const string& lexeme)
{
	// ��������� ������ ��������� ������� � ������������ ����




	// ���� ��� �����
	if (is_number(lexeme))
		return token_type::NUM;
	





	// � ��� ������, ���� ������ ����, �� ������ ��� �������������� �����
	return token_type::UNDEFINED;
}

bool Token::is_number(const string& lexeme)
{
	// �������� �� ���� �������� �������
	for (const auto& s : lexeme)
		// ���� ������ �� 0-9, ������ ��� �� ����� �����
		if (s < '0' || s > '9')
			return false;

	// ���� ��� ������� ������ ��������, �� ������ ��� �����
	return true;
}
