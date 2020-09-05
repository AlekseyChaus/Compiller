#pragma once
#pragma once

#include "Lexer.h"
#include "AST.h"
#include "error.h"
#include <iostream>
#include <string>
#include <vector>
#include<fstream>
using namespace std;

class Parser
	{
	private:
		int m_currentTokenIndex;
		vector <Token*> m_lexer;

		Node* compoundStatement();
		Node* program();
		Node* statement();
		Node* selectionStatement();
		Node* expressionStatement();
		Node* expression();
		Node* assignmentExpression();
		Node* equalityExpression();
		Node* relationalExpression();
		Node* additiveExpression();
		Node* multiplicativeExpression();
		Node* unaryExpression();
		Node* postfixExpression();
		Node* primaryExpression();
		Node* number();
		Node* identifier();
		Node* parenthesizedExpression();
		Node* argumentExpressionList();
		Node* statementList();
		Node* iterationStatement();
		Node* variableDeclaration();	/* ќбъ€вление переменной */
		Node* arrayRange();/* ƒиапазон массива */
		Node* functionStatement();
		Node* functionArgumentList();
		Node* functionArgument();
		Node* variableDeclarationStatement();

	public:
		
		Ast* m_ast; // дерево синтаксического анализа
		Parser(vector <Token*> _tokens); // в конструктор передаем результат работы лексера
		~Parser();


	private:
		// функци€ сдвигающа€ текущий индекс на 1 вперед
		void nextToken()
		{
			if (m_currentTokenIndex < m_lexer.size() - 1)
			{
				++m_currentTokenIndex;
			}
		}
		// функци€ возвращающа€ значение текущего токена
		string currentTokenValue()
		{
			return m_lexer[m_currentTokenIndex]->_lexeme;
		}
		// функци€ возвращающа€ тип текущего токена
		token_type currentTokenType()
		{
			return m_lexer[m_currentTokenIndex]->_type;
		}
		int currentTokenString()
		{
			return m_lexer[m_currentTokenIndex]->pos.stringNumber;
		}
		bool nextTokensIs(token_type tip)
		{
			return (tip == m_lexer[m_currentTokenIndex + 1]->_type);
		}
	
	};

















//----------------------------------------------------------------------
//struct TT_NODE //узлы токенов
//{
//	int strnumber;
//	char lexemclass[30],
//		lexemitself[30];
//	TT_NODE* next, * con, * th, * el, * func;
//} ;

//class Parser
//{
//private:
//	int index;
//	void Parse(); //основной метод парсера
//	TT_NODE* _tr; //новый массив токенов, который мы будем св€зывать определенным образом (образу€ гибрид: матрично-списковый)
//	void CreateNew(vector <Token*> tokens);//переводим все математические выражени€ в постфиксный вид и обнул€ем указатели, чтобы не было мусора
//	int SyntaxAnalyz(int p, int h, int k, int n, TT_NODE y[]);
//
//public:
//	TT_NODE* progr, * function;
//	Parser(vector <Token*> tokens); //но входе список токенов
//	~Parser();
//	int koltok;
//};

//class Parser
//{
//private:
//	int n; //количество токенов
//	void print_tree(TT_NODE* t, int level);
//	TT_NODE* new_table_lexem;
//	void LexemCreate(int * n, TT_NODE* new_table_lexem);
//	int Parse(int* n, TT_NODE* new_table_lexem);
//	int SyntaxAnalyz(int p, int h, int k, int* n, TT_NODE y[]);
//	void print();
//	void read(TT_NODE* t);
//public:
//	TT_NODE* front, *funct;
//	Parser(vector <Token*> tokens);
//	~Parser();
//};