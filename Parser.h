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
		Node* variableDeclaration();	/* ���������� ���������� */
		Node* arrayRange();/* �������� ������� */
		Node* functionStatement();
		Node* functionArgumentList();
		Node* functionArgument();
		Node* variableDeclarationStatement();

	public:
		
		Ast* m_ast; // ������ ��������������� �������
		Parser(vector <Token*> _tokens); // � ����������� �������� ��������� ������ �������
		~Parser();


	private:
		// ������� ���������� ������� ������ �� 1 ������
		void nextToken()
		{
			if (m_currentTokenIndex < m_lexer.size() - 1)
			{
				++m_currentTokenIndex;
			}
		}
		// ������� ������������ �������� �������� ������
		string currentTokenValue()
		{
			return m_lexer[m_currentTokenIndex]->_lexeme;
		}
		// ������� ������������ ��� �������� ������
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
//struct TT_NODE //���� �������
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
//	void Parse(); //�������� ����� �������
//	TT_NODE* _tr; //����� ������ �������, ������� �� ����� ��������� ������������ ������� (������� ������: ��������-���������)
//	void CreateNew(vector <Token*> tokens);//��������� ��� �������������� ��������� � ����������� ��� � �������� ���������, ����� �� ���� ������
//	int SyntaxAnalyz(int p, int h, int k, int n, TT_NODE y[]);
//
//public:
//	TT_NODE* progr, * function;
//	Parser(vector <Token*> tokens); //�� ����� ������ �������
//	~Parser();
//	int koltok;
//};

//class Parser
//{
//private:
//	int n; //���������� �������
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