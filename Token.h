#pragma once
#include <iostream>
#include <string>
using namespace std;

// ������������ ��� ����� ������
enum class token_type
{
 //   KEYWORD, // �������� �����
 //  COMMENT, // !
 //   STRLITERAL, // ��������� �������
    PROGRAM,

    IDENTIFIER, // �������������
    DESCRIPTION, //�������� ���������� (var)
    
    //������������ ����
    INIT, // ::

    //��������� ���� ����������
    CHARACTER,
   // LEN, //������ character
    INTEGER,

    //�������
    DIMENSION,
    STARR, // (/ ������ ������������� �������
    FINARR, // /) ����� ������������� �������

    //���������
    INT_C,
    CHAR_C,

    //�����
    DO,
    WHILE,
    END,
    //END_DO,

    BEGIN,
    //��������
    IF,
    THEN,
    ELSE,
    //END_IF,

    //���������
    //SUBROUTINE,
    FUNCTION,//*
    RETURN,//*
    CALL,
    INTENT,
    IN,
    OUT,

    //�������� �������
   // PRINT,
   //MULTICOMMA, // *, ����� print
  //  READ,
  //  WRITE,
    LCBRMULTI, // (* ��� read � write
   RCBRMULTI, // *) ��� read � write



 //��������������
    OPPLUS, // +
    OPMIN, // -
    OPMULTI, // *
    OPDIV, // /
    OPPOWER, // ** ���������� � �������
    SQRT, //������ �����


    //������ 
    AMP, // &
    DOT, // .
    COMMA, // ,
    DCOMMA, // ;
    LBR, // {
    RBR, // }
    RCBR, // )
    LCBR, // (
    LSQR, // [
    RSQR, // ]
    COLON, // :
  
    //���������
    OPNOTEQ, // /= �� �����
    OPMORE, // >
    OPMOREEQ, // >=
    OPLESS, // <
    OPLESSEQ, // <=
    OPCOMPRARE, // ==
    OPEQ, // =  ������������ ��������

    ERROR, // ������ 
    ENDOF // ����� ������ ���������
};

 struct posInFile
{
	unsigned int cursorPos; // ������� �������
	unsigned int stringNumber; // ����� ������
};

class Token
{
public:
	string _lexeme; // �������
	token_type _type; // ���
	posInFile pos;//������������ � �����

};





/*
typedef struct identifier
{
	string type; // ��� ����������
	bool iden_fun; // ���������� ��� ��������� ���������� (���������� �������)
	bool initialized; // ������������������� �� ����������
	string store; // ���� ��������� ��������, �� ��� ����� ����������� � ���� ����
} identifier;*/