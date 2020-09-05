#include "Parser.h"
#include "OPZ.h"
#pragma warning(disable:4996)

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stack>



Parser::Parser(vector <Token*> _tokens)//определяем
{

    m_lexer = _tokens;
    m_currentTokenIndex = 0;
    m_ast = new Ast();
    
    m_ast->root = program();

    m_ast->startChecking();

    m_ast->print();


}


Node* Parser::program()
{
    return statement();
}


Node* Parser::statement()
{
    Node* tempNode = nullptr;
    
    if (currentTokenType() == token_type::PROGRAM)
    {
        //cout<<"PROG"<< endl;
        nextToken();
        if (currentTokenType() != token_type::IDENTIFIER)
            throw Err(2, "Expected IDENTIFIER after PROGRAM in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() == token_type::INTEGER || currentTokenType() == token_type::CHARACTER)
        return expressionStatement();
    }

    if (currentTokenType() == token_type::BEGIN)
    {
        nextToken();
      //  cout << "BEGIN "<<endl;
        return compoundStatement();
    }
    else 

    if (currentTokenType() == token_type::IF)
    {
      //  cout << "IF" << endl;
           return selectionStatement(); 
    }
    else if (currentTokenType() == token_type::DO )
    {
      //  cout << "DO" << endl;
        return iterationStatement();
    }
    else if ((((currentTokenType() == token_type::INTEGER) || (currentTokenType() == token_type::CHARACTER)) 
        && (nextTokensIs(token_type::FUNCTION))) || (currentTokenType() == token_type::FUNCTION))
    {
        tempNode = functionStatement();
    }
    else
    {
      //  cout << "EXPR"<<endl;
        return expressionStatement();
    }

    tempNode = new Node(NodeType::STATEMENT, "", tempNode);

    return tempNode;
}


Node* Parser::functionArgument()
{
    if (currentTokenType() != token_type::IDENTIFIER)
        throw Err(2, "Expected : IDENTIFIER in line " + to_string(currentTokenString()));

    string argumentName = currentTokenValue();
    nextToken();


    if (currentTokenType() != token_type::COLON) // :
        throw Err(2, "Expected : in line " + to_string(currentTokenString()));
    nextToken();


    if (currentTokenType() != token_type::INTEGER &&
        currentTokenType() != token_type::CHARACTER)
    {
        throw Err(2, "Expected TYPE  in line " + to_string(currentTokenString()));
    }
    string argumentType = currentTokenValue(); //ЛИБО ПЕРЕМЕННАЯ
    string arrayType = currentTokenValue() + " array"; //ЛИБО МАССИВ

    nextToken();

    if (currentTokenType() == token_type::COMMA) //,
    {
        nextToken();
        if (currentTokenType() != token_type::DIMENSION)
             throw Err(2, "Expected DIMENSION  in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() != token_type::LCBR)
            throw Err(2, "Expected (  in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() != token_type::INT_C)
            throw Err(2, "Expected INT_CONST  in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() != token_type::COLON)
            throw Err(2, "Expected :  in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() != token_type::INT_C)
            throw Err(2, "Expected INT_CONST  in line " + to_string(currentTokenString()));
        nextToken();
        if (currentTokenType() != token_type::RCBR)
            throw Err(2, "Expected )  in line " + to_string(currentTokenString()));
        nextToken();


        Node* arrayTypeNode = new Node(NodeType::VARIABLE_TYPE, arrayType);

        return new Node(NodeType::FUNCTION_IMPLEMENTATION_ARG, argumentName, arrayTypeNode);
    }


    Node* argumentTypeNode = new Node(NodeType::VARIABLE_TYPE, argumentType);

    return new Node(NodeType::FUNCTION_IMPLEMENTATION_ARG, argumentName, argumentTypeNode);
}


Node* Parser::functionArgumentList()
{
    Node* tempNode = nullptr;

    if (currentTokenType() != token_type::LCBR) // (
        throw Err(2, "Expected ( in line " + to_string(currentTokenString()));
    nextToken();

    while (currentTokenType() != token_type::RCBR) //пока не )
    {
        Node* functionArgumentNode = functionArgument();

        tempNode = new Node(NodeType::FUNCTION_IMPLEMENTATION_ARGS, "", tempNode, functionArgumentNode);

        if (currentTokenType() == token_type::DCOMMA) // разделяются ;
            nextToken();
    }

    if (currentTokenType() != token_type::RCBR)
        throw Err(2, "Expected ) in line " + to_string(currentTokenString()));
    nextToken();

    return tempNode;
}


Node* Parser::variableDeclarationStatement()
{
    Node* variableDeclarationsNode = nullptr;
   // while (currentTokenType() != token_type::BEGIN)
    while (currentTokenType() == token_type::INTEGER || currentTokenType() == token_type::CHARACTER)
    {
        Node* tempVariableDeclaration = variableDeclaration();

     //   if (currentTokenType() != token_type::SEMICOLON)
    //        error("';' expected!");
     //   nextToken();

        variableDeclarationsNode = new Node(NodeType::STATEMENT_LIST, "", variableDeclarationsNode, tempVariableDeclaration);
    }

    return variableDeclarationsNode;
}





Node* Parser::functionStatement()
{
    string functionReturnType = "void"; //ЕСЛИ сразу ФУНКЦИЯ
    if (currentTokenType() == token_type::INTEGER || currentTokenType() == token_type::CHARACTER)
    {
        functionReturnType = currentTokenValue();
        nextToken();
    }
    nextToken();
    if (currentTokenType() != token_type::IDENTIFIER)
        throw Err(2, "Expected : identifier in line " + to_string(currentTokenString()));
    string functionName = currentTokenValue();
    nextToken();

    Node* functionArgumentListNode = functionArgumentList();


    Node* variableDeclarationsNode = variableDeclarationStatement();

    Node* functionBody = compoundStatement();

    Node* functionBodyWithVariables = new Node(NodeType::STATEMENT_LIST, "", functionBody, variableDeclarationsNode);

    Node* tempFunctionReturnType = new Node(NodeType::FUNCTION_IMPLEMENTATION_RETURN_TYPE, functionReturnType);

    return new Node(NodeType::FUNCTION_IMPLEMENTATION, functionName, tempFunctionReturnType,
        functionArgumentListNode, functionBodyWithVariables);
}

Node* Parser::selectionStatement()
{
    Node* tempNode = new Node(NodeType::IF);

    nextToken();

    if (currentTokenType()!= token_type::LCBR) //!!! условие в скобках
        throw Err(2, "Expected symbol: '(' in line " + to_string(currentTokenString()));
    nextToken();
    tempNode->op1 = expression(); 

    if (currentTokenType() != token_type::RCBR) //!!! условие в скобках
        throw Err(2, "Expected symbol: ')' in line " + to_string(currentTokenString()));
    nextToken();
   if (currentTokenType() != token_type::THEN)
        throw Err(2, "Expected THEN in line " + to_string(currentTokenString()));
    nextToken();


    tempNode->op2 = statement();


    if (currentTokenType() == token_type::ELSE)
    {
        tempNode->type = NodeType::IF_ELSE;
        nextToken();

        tempNode->op3 = statement();
    }

    return tempNode;
}

Node* Parser::iterationStatement()
{
    nextToken();

    if (currentTokenType() == token_type::IDENTIFIER)//цикл for
    {
      // cout << " DO - IDENT" << endl;
        Node* initNode = assignmentExpression(); //начальное условие

        if (currentTokenType() != token_type::COMMA) // должна быть ,
           throw Err (2,"Expected , in line " + to_string(currentTokenString()));
        nextToken();

        Node* endValueNode = assignmentExpression(); //конечное значение

        Node* bodyNode = statement(); //тело DO

        Node* oneConstantNode = new Node(NodeType::INTEGER_CONST, "1");
        Node* addOneNode = new Node(NodeType::ADD, "", initNode->op1, oneConstantNode);
        Node* afterEffectNode = new Node(NodeType::SET, "", initNode->op1, addOneNode);

        Node* condition = new Node(NodeType::LESS, "", initNode->op1, endValueNode);

        return new Node(NodeType::FOR, "", initNode, condition, afterEffectNode, bodyNode);
    }
    else if (currentTokenType() == token_type::WHILE) // DO While
    {
       // cout << " DO - WHILE" << endl;
        nextToken();

        if (currentTokenType() != token_type::LCBR) //!!! условие в скобках
            throw Err(2, "Expected symbol: '(' in line " + to_string(currentTokenString()));
        nextToken();

        Node* conditionNode = equalityExpression(); // условие В ()!!!!!!!

        if (currentTokenType() != token_type::RCBR) //!!! условие в скобках
            throw Err(2, "Expected symbol: ')' in line " + to_string(currentTokenString()));
        nextToken();

        Node* bodyNode = statement();

        return new Node(NodeType::WHILE, "", conditionNode, bodyNode);
    }
}

Node* Parser::relationalExpression()
{
    Node* tempNode = additiveExpression();

    if (currentTokenType() == token_type::OPLESS) // <
    {
        nextToken();
        Node* tempAdditiveExpression = additiveExpression();

        tempNode = new Node(NodeType::LESS, "", tempNode, tempAdditiveExpression);
    }
    else if (currentTokenType() == token_type::OPMORE) // >
    {
        nextToken();
        Node* tempAdditiveExpression = additiveExpression();

        tempNode = new Node(NodeType::GREATER, "", tempNode, tempAdditiveExpression);
    }
    else if (currentTokenType() == token_type::OPMOREEQ) // >=
    {
        nextToken();
        Node* tempAdditiveExpression = additiveExpression();

        tempNode = new Node(NodeType::GREATER_EQUAL, "", tempNode, tempAdditiveExpression);
    }
    else if (currentTokenType() == token_type::OPLESSEQ) // <=
    {
        nextToken();
        Node* tempAdditiveExpression = additiveExpression();

        tempNode = new Node(NodeType::LESS_EQUAL, "", tempNode, tempAdditiveExpression);
    }

    return tempNode;
}



Node* Parser::additiveExpression()
{
    Node* tempNode = multiplicativeExpression();

    if (currentTokenType() == token_type::OPPLUS ||  //+
        currentTokenType() == token_type::OPMIN)     //-
    {
        NodeType tempType = NodeType::ADD;

        if (currentTokenType() == token_type::OPMIN)  //-
            tempType = NodeType::SUB;

        nextToken();

        Node* tempAdditiveExpression = additiveExpression();

        tempNode = new Node(tempType, "", tempNode, tempAdditiveExpression);
    }

    return tempNode;
}

Node* Parser::multiplicativeExpression()
{
    Node* tempNode = unaryExpression();

    if (currentTokenType() == token_type::OPMULTI ||  // *
        currentTokenType() == token_type::OPDIV)      // /
    {
        NodeType tempType = NodeType::MUL;

        if (currentTokenType() == token_type::OPDIV)  // /
            tempType = NodeType::DIV;

        nextToken();

        Node* tempMultiplicativeExpression = multiplicativeExpression();

        tempNode = new Node(tempType, "", tempNode, tempMultiplicativeExpression);
    }

    return tempNode;
}

Node* Parser::unaryExpression()
{
    Node* tempNode = nullptr;

    if (currentTokenType() == token_type::OPMIN)  // -
    {
        NodeType type = NodeType::UNARY_MINUS;

          nextToken();

        tempNode = unaryExpression();
        tempNode = new Node(type, "", tempNode);
    }
    else
    {
        tempNode = postfixExpression();
    }
    return tempNode;
}

Node* Parser::expression()
{
    Node* tempNode = assignmentExpression();

    tempNode = new Node(NodeType::EXPRESSION, "", tempNode);

    return tempNode;
}


/* Диапазон массива */
Node* Parser::arrayRange()
{
    if (currentTokenType() != token_type::LCBR) // (
        throw Err(2, "Expected ( in line " + to_string(currentTokenString()));
    nextToken();

    if (currentTokenType() != token_type::INT_C)
    {
        throw Err(2, "Expected INT in line " + to_string(currentTokenString()));
    }
    string startRangeValue = currentTokenValue();
    Node* startRangeValueNode = new Node(NodeType::INTEGER_CONST, startRangeValue);
    nextToken();


    if (currentTokenType() != token_type::COLON) // :
         throw Err(2, "Expected ) in line " + to_string(currentTokenString()));
    nextToken();


    if (currentTokenType() != token_type::INT_C)
    {
        throw Err(2, "Expected INT_С in line " + to_string(currentTokenString()));
    }
    string endRangeValue = currentTokenValue();
    Node* endRangeValueNode = new Node(NodeType::INTEGER_CONST, endRangeValue);
    nextToken();

    if (currentTokenType() != token_type::RCBR) // )
        throw Err(2, "Expected ) in line " + to_string(currentTokenString()));
    nextToken();

    return new Node(NodeType::RANGE, "", startRangeValueNode, endRangeValueNode);
}



/* Объявление переменной */
Node* Parser::variableDeclaration() // выглядит как integer :: i или integer, dimension(1:10) :: myarray
{
    if (currentTokenType() != token_type::INTEGER &&
        currentTokenType() != token_type::CHARACTER)
    {
        throw Err(2, "Expected integer/character in line " + to_string(currentTokenString()));
    }
  
    if (currentTokenType() == token_type::INTEGER) //что ожидаем после INT
    {
        nextToken();
        if (currentTokenType() == token_type::INIT) // ::
        {
            nextToken();
            if (currentTokenType() != token_type::IDENTIFIER)
            {
                throw Err(2, "Expected identificator in line " + to_string(currentTokenString()));
            }
            else
            {
                string variableName = currentTokenValue();
                string variableType = "integer";
                nextToken();
                Node* variableTypeNode = new Node(NodeType::VARIABLE_TYPE, variableType);
                return new Node(NodeType::VARIABLE_DECLARATION, variableName, variableTypeNode);
            }

        }
        else if (currentTokenType() == token_type::COMMA) //,
        {
            nextToken();
            if (currentTokenType() != token_type::DIMENSION)
            {
                throw Err(2, "Expected DIMENSION in line " + to_string(currentTokenString()));
            }
            else // ЭТО МАССИВ INT
            {
                nextToken();
                Node* rangeNode = arrayRange(); // размер

                if (currentTokenType() != token_type::INIT) // ::
                {
                    throw Err(2, "Expected :: in line " + to_string(currentTokenString()));
                }
                nextToken();

                if (currentTokenType() != token_type::IDENTIFIER)
                {
                    throw Err(2, "Expected identificator in line " + to_string(currentTokenString()));
                }
                string variableName =currentTokenValue();
                string arrayType = "integer array";
                nextToken();

                Node* arrayTypeNode = new Node(NodeType::VARIABLE_TYPE, arrayType);
                return new Node(NodeType::VARIABLE_DECLARATION, variableName, arrayTypeNode, rangeNode);

            }
        }
        else
        {
            throw Err(2, "Expected :: in line " + to_string(currentTokenString()));
        }
         
    }
    
    if (currentTokenType() == token_type::CHARACTER) // ЕСЛИ ЭТО СТРОКА
    {
        nextToken();
        if (currentTokenType() == token_type::INIT) // ::
        {
            nextToken();
            if (currentTokenType() != token_type::IDENTIFIER)
            {
                throw Err(2, "Expected identificator in line " + to_string(currentTokenString()));
            }
            else
            {
                string variableName = currentTokenValue();
                string variableType = "character";
                nextToken();
                Node* variableTypeNode = new Node(NodeType::VARIABLE_TYPE, variableType);
                return new Node(NodeType::VARIABLE_DECLARATION, variableName, variableTypeNode);
            }

        }
        else if (currentTokenType() == token_type::COMMA) //,
        {
            nextToken();
            if (currentTokenType() != token_type::DIMENSION)
            {
                throw Err(2, "Expected DIMENSION in line " + to_string(currentTokenString()));
            }
            else // ЭТО МАССИВ 
            {
                nextToken();
                Node* rangeNode = arrayRange(); // размер

                if (currentTokenType() != token_type::INIT) // ::
                {
                    throw Err(2, "Expected :: in line " + to_string(currentTokenString()));
                }
                nextToken();

                if (currentTokenType() != token_type::IDENTIFIER)
                {
                    throw Err(2, "Expected identificator in line " + to_string(currentTokenString()));
                }
                string variableName = currentTokenValue();
                string arrayType = "character array";
                nextToken();

                Node* arrayTypeNode = new Node(NodeType::VARIABLE_TYPE, arrayType);
                return new Node(NodeType::VARIABLE_DECLARATION, variableName, arrayTypeNode, rangeNode);

            }

        }
        else
        {
            throw Err(2, "Expected :: in line " + to_string(currentTokenString()));
        }
    }
   // Node* variableTypeNode = new Node(NodeType::VARIABLE_TYPE, variableType);

    //return new Node(NodeType::VARIABLE_DECLARATION, variableName, variableTypeNode);
}


Node* Parser::primaryExpression()
{
    Node* tempNode = nullptr;

    if (currentTokenType() == token_type::INT_C)
    {
        tempNode = number();
    }
    else if (currentTokenType() == token_type::CHAR_C)
    {
        string stringConstantValue = currentTokenValue();

        tempNode = new Node(NodeType::STRING_CONST, stringConstantValue);
        nextToken();
    }
    else if (currentTokenType() == token_type::IDENTIFIER)
    {
        tempNode = identifier();
    }
    else if (currentTokenType() == token_type::LCBR) // (
    {
       tempNode = parenthesizedExpression();
    }
    else if (currentTokenType() == token_type::INTEGER || currentTokenType() == token_type::CHARACTER) //перем
    {
        tempNode = variableDeclaration();
    }
    else
    {
        throw Err(2, "Unexpected symbol in line " + to_string(currentTokenString()));
    }
///////////////////////!!!!

    return tempNode;
}

Node* Parser::postfixExpression()
{
    Node* tempNode = primaryExpression();

    if (currentTokenType() == token_type::LSQR) // [
    {
        nextToken();
        Node* tempExpression = expression();

        if (currentTokenType() != token_type::RSQR) // ]
            throw Err (2,"Expected ] in line " + to_string(currentTokenString()));
        nextToken();

        tempNode = new Node(NodeType::INDEX_CAPTURE, "", tempNode, tempExpression);
    }
    else if (currentTokenType() == token_type::LCBR) // (
    {
        nextToken();
        string functionName = tempNode->value;
        Node* tempArgumentExpressionList = argumentExpressionList();

        if (currentTokenType() != token_type::RCBR) // )
            throw Err(2, "Unexpected : '" + string(currentTokenValue()) + "' in line " + to_string(currentTokenString()));
        nextToken();

        tempNode = new Node(NodeType::FUNCTION_CALL, functionName, tempArgumentExpressionList);
    }

    return tempNode;
}

Node* Parser::parenthesizedExpression()
{
    if (currentTokenType() != token_type::LCBR) // НЕ  (
        throw Err(2, "Unexpected : '" + string(currentTokenValue()) + "' in line " + to_string(currentTokenString()));
  nextToken();

    Node* temp_node = expression();

    if (currentTokenType() != token_type::RCBR)
        throw Err(2, "Unexpected : '" + string(currentTokenValue()) + "' in line " + to_string(currentTokenString()));
    nextToken();

    return temp_node;
}

Node* Parser::assignmentExpression()
{
    Node* tempNode = equalityExpression();

    if (currentTokenType() == token_type::OPEQ) //присваивние
    {
        nextToken();
        Node* tempAssignmentExpression = assignmentExpression();

        tempNode = new Node(NodeType::SET, "", tempNode, tempAssignmentExpression);
    }

    return tempNode;
}


Node* Parser::argumentExpressionList()
{
    Node* tempNode = nullptr;

    while (currentTokenType() != token_type::RCBR) // )
    {
        Node* tempFunctionArgument = assignmentExpression();

        tempNode = new Node(NodeType::FUNCTION_ARGS, "", tempFunctionArgument, tempNode);

        if (currentTokenType() == token_type::COMMA) //,
            nextToken();
    }

    return tempNode;
}



Node* Parser::expressionStatement()
{
    Node* tempNode = expression();

    return tempNode;
}

Node* Parser::equalityExpression()
{
    Node* tempNode = relationalExpression();

    if (currentTokenType() == token_type::OPCOMPRARE) // ==
    {
        nextToken();
        Node* tempRelationalExpression = relationalExpression();

        tempNode = new Node(NodeType::EQUAL, "", tempNode, tempRelationalExpression);
    }
    else if (currentTokenType() == token_type::OPNOTEQ) // /= не равно
    {
        nextToken();
        Node* tempRelationalExpression = relationalExpression();

        tempNode = new Node(NodeType::NOT_EQUAL, "", tempNode, tempRelationalExpression);
    }

    return tempNode;
}

Node* Parser::number()
{
    string numberValue = currentTokenValue();

    Node* tempNode = new Node(NodeType::INTEGER_CONST, numberValue);
    nextToken();

    return tempNode;
}


Node* Parser::identifier()
{
    string identifierValue = currentTokenValue();

    Node* temp_node = temp_node = new Node(NodeType::USING_VARIABLE, identifierValue);
    nextToken();

    return temp_node;
}

Node* Parser::statementList()
{
    Node* tempNode = nullptr;

    while (currentTokenType() != token_type::END)
    {
     //   cout << " NO END"<<endl;
        Node* tempStatement = statement();
        tempNode = new Node(NodeType::STATEMENT_LIST, "", tempNode, tempStatement);
    }
  //  cout << " END" << endl;
    nextToken();

   // if (currentTokenType() == token_type::DOT)
    //    nextToken();

    if (currentTokenType() == token_type::DO || currentTokenType() == token_type::IF)
    {
      //  cout << " END DO IF FUN" << endl;
        nextToken(); 
        // cout << currentTokenValue() << endl;
        //  cout << "-----------------" << endl;
    }
    else if (currentTokenType() == token_type::FUNCTION)
    {
        nextToken();
        if (currentTokenType() == token_type::IDENTIFIER)
            nextToken();
    }
    else
        if (currentTokenType() == token_type::PROGRAM)
        {
          //  cout << " END PROG" << endl;
            nextToken();
            if (currentTokenType() != token_type::IDENTIFIER)
                throw Err(2, "Explain identificator in line " + to_string(currentTokenString()));
            else
                nextToken();
        }
        else if (currentTokenType() == token_type::ENDOF)  
        { //nextToken() ;
        }
        else if (currentTokenType() != token_type::ELSE)
            throw Err(2, "Explain END identificator/do/if in line " + to_string(currentTokenString()));
        
    return tempNode;
}

Node* Parser::compoundStatement()
{
    //nextToken();

    Node* tempNode = statementList();

    tempNode = new Node(NodeType::STATEMENT, "", tempNode);

    return tempNode;
}
Parser::~Parser()
{
	
}
