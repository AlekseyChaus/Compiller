#pragma once

#include "VariableTable.h"
#include "Lexer.h"


// ������������ ��� ����� �����
enum class NodeType
{
    IDENTIFIER,

    VARIABLE_DECLARATION,
    USING_VARIABLE,
    VARIABLE_TYPE,

    INTEGER_CONST,
    STRING_CONST,

    RANGE,

    ADD,// +
    SUB,// -
    MUL, //  *
    DIV,//  /

    UNARY_MINUS,


    SET, // ������������

    EQUAL, // ==
    NOT_EQUAL, // <>
    LESS, // <
    GREATER, // >
    LESS_EQUAL, // <=
    GREATER_EQUAL, // >=



    FOR,
    WHILE,
    REPEAT,
    BREAK,

    IF,
    IF_ELSE,

    INDEX_CAPTURE,

    FUNCTION_CALL,
    FUNCTION_ARGS,
    FUNCTION_ARG,
    RETURN,


    FUNCTION_IMPLEMENTATION,
    FUNCTION_IMPLEMENTATION_ARG,
    FUNCTION_IMPLEMENTATION_ARGS,
    FUNCTION_IMPLEMENTATION_RETURN_TYPE,


    EXPRESSION,

    STATEMENT,
    STATEMENT_LIST
};

// ����� ����
class Node
{
public:
    string value; // ��� ������ ��� ��������
    NodeType type; // ��� ������ ��� ���

    // 4 �������
    Node* op1;
    Node* op2;
    Node* op3;
    Node* op4;


private:
    size_t m_idStatementWhereNodeDefined; // ������������� ����� Statement � ������� ������ ���� ���������, ����
    // ���� ��� ���� Statement, �� �������� ��� ������������

    VariableTable* m_variableTable; // ��� ������ ��������� �� ������� � ����������� ������ ��� ����� Statement

public:

    // �����������
    explicit Node(NodeType type, const string& value = "", Node* op1 = nullptr, Node* op2 = nullptr,
        Node* op3 = nullptr, Node* op4 = nullptr, VariableTable* variableTable = nullptr)
    {
        this->type = type;
        this->value = value;
        this->op1 = op1;
        this->op2 = op2;
        this->op3 = op3;
        this->op4 = op4;
        this->m_variableTable = variableTable;

        this->m_idStatementWhereNodeDefined = -1;
    }


public:

    [[nodiscard]] VariableTable* variableTable() const
    {
        return m_variableTable;
    }

    void setVariableTable(VariableTable* variableTable)
    {
        m_variableTable = variableTable;
    }

    [[nodiscard]] size_t idStatementWhereNodeDefined() const
    {
        return m_idStatementWhereNodeDefined;
    }

    void idStatementWhereNodeDefined(size_t stmtId)
    {
        m_idStatementWhereNodeDefined = stmtId;
    }

    static bool isComparisonOperator(NodeType type)
    {
        return type == NodeType::LESS ||
            type == NodeType::GREATER ||
            type == NodeType::LESS_EQUAL ||
            type == NodeType::GREATER_EQUAL ||
            type == NodeType::EQUAL ||
            type == NodeType::NOT_EQUAL;
    }
};
