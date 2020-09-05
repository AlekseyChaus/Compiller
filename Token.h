#pragma once
#include <iostream>
#include <string>
using namespace std;

// перечисление дл€ типов токена
enum class token_type
{
 //   KEYWORD, //  лючевое слово
 //  COMMENT, // !
 //   STRLITERAL, // —троковый литерал
    PROGRAM,

    IDENTIFIER, // »дентификатор
    DESCRIPTION, //описание переменных (var)
    
    //присваивание типа
    INIT, // ::

    //возможные типы переменных
    CHARACTER,
   // LEN, //длинна character
    INTEGER,

    //массивы
    DIMENSION,
    STARR, // (/ начало инициализации массива
    FINARR, // /) конец инициализации массива

    //константы
    INT_C,
    CHAR_C,

    //циклы
    DO,
    WHILE,
    END,
    //END_DO,

    BEGIN,
    //условные
    IF,
    THEN,
    ELSE,
    //END_IF,

    //процедуры
    //SUBROUTINE,
    FUNCTION,//*
    RETURN,//*
    CALL,
    INTENT,
    IN,
    OUT,

    //основные функции
   // PRINT,
   //MULTICOMMA, // *, после print
  //  READ,
  //  WRITE,
    LCBRMULTI, // (* дл€ read и write
   RCBRMULTI, // *) дл€ read и write



 //математические
    OPPLUS, // +
    OPMIN, // -
    OPMULTI, // *
    OPDIV, // /
    OPPOWER, // ** возведение в степень
    SQRT, //вз€тие корн€


    //другие 
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
  
    //сравнени€
    OPNOTEQ, // /= не равно
    OPMORE, // >
    OPMOREEQ, // >=
    OPLESS, // <
    OPLESSEQ, // <=
    OPCOMPRARE, // ==
    OPEQ, // =  присваивание значени€

    ERROR, // ќшибка 
    ENDOF //  онец текста программы
};

 struct posInFile
{
	unsigned int cursorPos; // позици€ курсора
	unsigned int stringNumber; // номер строки
};

class Token
{
public:
	string _lexeme; // лексема
	token_type _type; // тип
	posInFile pos;//расположение в файле

};





/*
typedef struct identifier
{
	string type; // тип переменной
	bool iden_fun; // глобальна€ или локальна€ переменна€ (переменна€ функции)
	bool initialized; // проинициализирована ли переменна€
	string store; // если присовено значение, то оно будет содержатьс€ в этом поле
} identifier;*/