#pragma once
#include "Node.h"
#include"VariableTable.h"
#include "FunctionTable.h"
#include "GlobalFunctionsTable.h"

class Asm;

class Ast
{
public:
	Node* root; // указатель на корень
private:
    vector<std::pair<Node*, Node*>> m_allStatements; // все блоки с их родительскими блоками

    VariableTable m_allVariables; // тут храним все переменные
    VariableTable m_globalVariables; // тут храним все глобальные переменные
    vector<VariableTable*> m_variableTables; // тут храним все таблицы переменных для каждого блока из m_allStatements

    FunctionTable m_allFunction; // тут храним все функции
    GlobalFunctionsTable m_globalFunctions; // тут храним все предопределенные функции типа print, input и т.д.


public:
    Ast()
    {
        this->root = nullptr;
    }

public:
    friend Asm;

public:

    // основная функция для семантического анализа
    void startChecking()
    {
        // все блоки stmt заносим в специально отведенный вектор
        designateAllBlocksWithParent();

        // устаналиваем для основных узлов идентификаторы блоков, в которых они расположены
         setForAllNodeStatementIdWhereItDefined();

        // заменяем присваивания в функциях на оператор return
        replaceForReturnOperator();

        // выделяем все переменные и заносим их в таблицу переменных и сразу проверем 
        designateVariables();

        // выделяем все функции и заносим их в таблицу функций
        designateFunctions();

        // отмечаем каждый оператор return размером аргументов для ассемблера
       markReturnOperator();

        // проверяем, что функции вызываются с верными типами аргументов 
        checkFunctionsCall();
    }


private:

    void replaceForReturnOperator()
    {
        for (const auto& statement : m_allStatements)
        {
            if (statement.second->op1 != nullptr &&
                statement.second->op1->type == NodeType::FUNCTION_IMPLEMENTATION)
            {
                string functionName = statement.second->op1->value;

                replaceForReturnOperatorRecursive(statement.second->op1, functionName);
            }
        }
    }

    void setForAllNodeStatementIdWhereItDefined()
    {
        size_t countBlock = 0;
        setForAllNodeStatementIdWhereItDefinedRecursive(root, countBlock);
    }

    void designateAllBlocksWithParent()
    {
        designateAllBlocksWithParentRecursive(root, nullptr);
    }

    void designateVariables()
    {
        m_variableTables.reserve(m_allStatements.size());

        for (auto& [parentStmt, stmt] : m_allStatements)
        {
            size_t stmtId = stmt->idStatementWhereNodeDefined();
            VariableTable* newVariableTable = new VariableTable(stmtId); //создание таблицы переменных

            m_variableTables.push_back(newVariableTable);

            if (parentStmt != nullptr)
            {
                newVariableTable->setParent(parentStmt->variableTable());
            }

            stmt->setVariableTable(newVariableTable);

            designateVariablesRecursive(stmt->op1, newVariableTable);//заполняем таблицу переменных (рекурсивно выделяем переменные)
        }

        
        for (const auto& variable : m_allVariables.variables())
        {
            if (variable->isGlobal())
            {
                cout << "GLOBAL" << endl;
                m_globalVariables.addVariable(variable); //заполняем отдельно переменные используемые в самой программе
            }
        }
    }

    void designateFunctions()
    {
        designateFunctionsRecursive(m_allStatements[0].second->op1);
    }

    void markReturnOperator()
    {
        for (auto& [_, stmt] : m_allStatements)
        {
            if (stmt->op1 != nullptr &&
                stmt->op1->type == NodeType::FUNCTION_IMPLEMENTATION)
            {
                string functionName = stmt->op1->value;
                markReturnOperatorRecursive(stmt->op1->op3, stmt->idStatementWhereNodeDefined(), functionName);
            }
        }
    }

    void checkFunctionsCall()
    {
        checkFunctionsCallRecursive(root);
    }

private:

    void replaceForReturnOperatorRecursive(Node* currentNode, const string& functionName)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::EXPRESSION) //это должно быть выражение 
        {
            if (currentNode->op1 != nullptr &&
                currentNode->op1->type == NodeType::SET) //должно быть далее ( в первом потомке) присваивание 
            {
                if (currentNode->op1->op1 != nullptr &&
                    currentNode->op1->op1->type == NodeType::USING_VARIABLE && //в первом потомке присваивания должна быть переменная 
                    currentNode->op1->op1->value == functionName)
                {
                    Node* returnExpressionNode = new Node(NodeType::EXPRESSION, "", currentNode->op1->op2); //в таком случае
                    Node* returnOperatorNode = new Node(NodeType::RETURN, "", returnExpressionNode);//вставляем узел return
                    *currentNode = *returnOperatorNode;
                    return;
                }
            }
        }

        replaceForReturnOperatorRecursive(currentNode->op1, functionName);
        replaceForReturnOperatorRecursive(currentNode->op2, functionName);
        replaceForReturnOperatorRecursive(currentNode->op3, functionName);
        replaceForReturnOperatorRecursive(currentNode->op4, functionName);
    }

    void setForAllNodeStatementIdWhereItDefinedRecursive(Node* currentNode, size_t& countBlock)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->type == NodeType::STATEMENT) //если это инструкция 
        {
            currentNode->idStatementWhereNodeDefined(countBlock);
            ++countBlock;
        }
        else if (currentNode->type == NodeType::IF || //если это условные конструкции
            currentNode->type == NodeType::IF_ELSE)
        {
            currentNode->op2->idStatementWhereNodeDefined(countBlock);
            ++countBlock;
        }
        else if (currentNode->type == NodeType::FOR) //если это цикл 
        {
            currentNode->op4->idStatementWhereNodeDefined(countBlock);
            ++countBlock;
        }
        else if (currentNode->type == NodeType::FUNCTION_IMPLEMENTATION) //или объявление функции
        {
            if (currentNode->op3 != nullptr)
            {
                currentNode->op3->idStatementWhereNodeDefined(countBlock);
                ++countBlock;
            }
        }


        setForAllNodeStatementIdWhereItDefinedRecursive(currentNode->op1, countBlock);
        setForAllNodeStatementIdWhereItDefinedRecursive(currentNode->op2, countBlock);
        setForAllNodeStatementIdWhereItDefinedRecursive(currentNode->op3, countBlock);
        setForAllNodeStatementIdWhereItDefinedRecursive(currentNode->op4, countBlock);
    }

    void designateAllBlocksWithParentRecursive(Node* currentNode, Node* currentStatement)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::STATEMENT)
        {
            m_allStatements.emplace_back(currentStatement, currentNode);
            currentStatement = currentNode;
        }


        designateAllBlocksWithParentRecursive(currentNode->op1, currentStatement);
        designateAllBlocksWithParentRecursive(currentNode->op2, currentStatement);
        designateAllBlocksWithParentRecursive(currentNode->op3, currentStatement);
        designateAllBlocksWithParentRecursive(currentNode->op4, currentStatement);
    }

    void designateVariablesRecursive(Node* currentNode, VariableTable* currentTable)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->type == NodeType::STATEMENT)
            return;



        if (currentNode->type == NodeType::VARIABLE_DECLARATION ||
            currentNode->type == NodeType::FUNCTION_IMPLEMENTATION_ARG)
        {
            string variableName = currentNode->value;


            if (currentTable->hasVariable(variableName))
            {
                throw Err(2, "ERROR: The variable '" + variableName + "' has already been declared!");
            }



            VariableType variableType = Variable::typeFromString(currentNode->op1->value);

            size_t arraySize = 0;
            if (variableType == VariableType::INTEGER_ARRAY &&
                currentNode->type != NodeType::FUNCTION_IMPLEMENTATION_ARG)
            {
                arraySize = stoi(currentNode->op2->op2->value) - stoi(currentNode->op2->op1->value) + 1;
            }

            size_t blockId = currentTable->blockId();

            Variable* newVariable = new Variable
            (
                variableName,
                variableType,
                blockId,
                arraySize
            );



            currentNode->idStatementWhereNodeDefined(blockId);

            currentTable->addVariable(newVariable);

            m_allVariables.addVariable(newVariable);

            return;
        }
        else if (currentNode->type == NodeType::USING_VARIABLE)
        {
            string variableName = currentNode->value;


            if (!currentTable->hasVariableGlobally(variableName))
            {
                cout << "ERROR: The name '" << variableName << "' is not declared!" << endl;
                throw Err(2, "ERROR: The name '" + variableName + "' is not declared!");
            }


            auto [blockId, variable] = currentTable->getVariableAndBlockIdWhereItDefined(variableName);

            currentNode->idStatementWhereNodeDefined(blockId);
        }


        designateVariablesRecursive(currentNode->op1, currentTable);
        designateVariablesRecursive(currentNode->op2, currentTable);
        designateVariablesRecursive(currentNode->op3, currentTable);
        designateVariablesRecursive(currentNode->op4, currentTable);
    }


    void designateFunctionsRecursive(Node* currentNode)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::FUNCTION_IMPLEMENTATION)
        {
            string functionName = currentNode->value;
            VariableType returnType = Variable::typeFromString(currentNode->op1->value);


            vector<VariableType> argumentsDescription;
            vector<Variable*> arguments;
            designateFunctionArgumentsRecursive(currentNode->op2, argumentsDescription, arguments);


            size_t localVariableSize = 0;
            vector<Variable*> variables;
            designateFunctionLocalVariablesRecursive(currentNode->op3, localVariableSize, variables);

            Function* newFunction = new Function(functionName, returnType, argumentsDescription, localVariableSize, variables, arguments);

            m_allFunction.addFunction(newFunction);
        }

        designateFunctionsRecursive(currentNode->op1);
        designateFunctionsRecursive(currentNode->op2);
    }
    void designateFunctionArgumentsRecursive(Node* currentNode, vector<VariableType>& argumentsDescription, vector<Variable*>& arguments)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::FUNCTION_IMPLEMENTATION_ARG)
        {
            string argument_name = currentNode->value;
            size_t blockId = currentNode->idStatementWhereNodeDefined();
            Variable* variable = m_allVariables.getVariable(argument_name, blockId);


            VariableType type = variable->type();
            argumentsDescription.push_back(type);



            variable->isArgument(true);

            arguments.push_back(variable);
        }

        designateFunctionArgumentsRecursive(currentNode->op1, argumentsDescription, arguments);
        designateFunctionArgumentsRecursive(currentNode->op2, argumentsDescription, arguments);
    }
    void designateFunctionLocalVariablesRecursive(Node* currentNode, size_t& size, vector<Variable*>& variables)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->type == NodeType::VARIABLE_DECLARATION)
        {
            string variableName = currentNode->value;
            size_t blockId = currentNode->idStatementWhereNodeDefined();
            Variable* variable = m_allVariables.getVariable(variableName, blockId);


            VariableType type = Variable::typeFromString(currentNode->op1->value);
            size_t variable_size = 4;


            if (type != VariableType::INTEGER_ARRAY)
            {
                size += variable_size;
            }


            variables.push_back(variable);
            return;
        }


        designateFunctionLocalVariablesRecursive(currentNode->op1, size, variables);
        designateFunctionLocalVariablesRecursive(currentNode->op2, size, variables);
        designateFunctionLocalVariablesRecursive(currentNode->op3, size, variables);
        designateFunctionLocalVariablesRecursive(currentNode->op4, size, variables);
    }
    void markReturnOperatorRecursive(Node* currentNode, size_t currentBlock, const string& functionName)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->type == NodeType::FUNCTION_IMPLEMENTATION)
            return;


        if (currentNode->type == NodeType::RETURN)
        {
            currentNode->idStatementWhereNodeDefined(currentBlock);

            size_t size_of_arguments = m_allFunction.getFunction(functionName)->argumentsSize();

            currentNode->value = to_string(size_of_arguments);
        }


        markReturnOperatorRecursive(currentNode->op1, currentBlock, functionName);
        markReturnOperatorRecursive(currentNode->op2, currentBlock, functionName);
        markReturnOperatorRecursive(currentNode->op3, currentBlock, functionName);
        markReturnOperatorRecursive(currentNode->op4, currentBlock, functionName);
    }


    void checkFunctionsCallRecursive(Node* currentNode)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::FUNCTION_CALL)
        {
            string functionName = currentNode->value;
            vector<VariableType> types;

            designateFunctionCallArgumentsRecursive(currentNode, types);

            if (!m_globalFunctions.hasFunction(functionName))
            {
                m_allFunction.getFunction(functionName, types);
            }
        }

        checkFunctionsCallRecursive(currentNode->op1);
        checkFunctionsCallRecursive(currentNode->op2);
        checkFunctionsCallRecursive(currentNode->op3);
        checkFunctionsCallRecursive(currentNode->op4);
    }
    void designateFunctionCallArgumentsRecursive(Node* currentNode, vector<VariableType>& arguments)
    {
        if (currentNode == nullptr)
            return;


        if (currentNode->type == NodeType::FUNCTION_ARGS)
        {
            VariableType type;

            if (currentNode->op1->type == NodeType::INTEGER_CONST)
            {
                type = VariableType::INTEGER;
            }
            else if (currentNode->op1->type == NodeType::STRING_CONST)
            {
                type = VariableType::STRING;
            }
            else
            {
                giveExpressionTypeRecursive(currentNode->op1, type);
            }


            arguments.push_back(type);
        }

        designateFunctionCallArgumentsRecursive(currentNode->op1, arguments);
        designateFunctionCallArgumentsRecursive(currentNode->op2, arguments);
    }
    void giveExpressionTypeRecursive(Node* currentNode, VariableType& type)
    {
        if (currentNode == nullptr)
            return;

        if (currentNode->type == NodeType::ADD ||
            currentNode->type == NodeType::SUB ||
            currentNode->type == NodeType::MUL ||
            currentNode->type == NodeType::DIV)
        {
            Node* op1 = currentNode->op1;
            Node* op2 = currentNode->op2;

            VariableType op1Type = VariableType::UNDEFINED;
            VariableType op2Type = VariableType::UNDEFINED;

            giveExpressionTypeRecursive(op1, op1Type);
            giveExpressionTypeRecursive(op2, op2Type);

            bool is_reducible = op1Type == op2Type;

            if (!is_reducible)
            {
                string op1_type_str = to_string((int)op1Type);
                string op2_type_str = to_string((int)op2Type);

                string action = to_string((int)currentNode->type);

                error("Operator " + action + " cannot be applied to types '" + op1_type_str + "' and '" + op2_type_str + "'.");
            }

            type = op1Type;
        }
        else if (currentNode->type == NodeType::UNARY_MINUS)
        {
            auto op1 = currentNode->op1;

            VariableType op1Type = VariableType::UNDEFINED;
            giveExpressionTypeRecursive(op1, op1Type);

            type = VariableType::INTEGER;
        }
        else if (currentNode->type == NodeType::USING_VARIABLE ||
            currentNode->type == NodeType::INTEGER_CONST ||
            currentNode->type == NodeType::STRING_CONST)
        {
            type = variableTypeOfNode(currentNode);
        }
        else if (currentNode->type == NodeType::FUNCTION_CALL)
        {
            string functionName = currentNode->value;

            vector<VariableType> types;
            designateFunctionCallArgumentsRecursive(currentNode, types);

            Function* function = nullptr;

            if (!m_globalFunctions.hasFunction(new Function(functionName, VariableType::UNDEFINED, types)))
            {
                function = m_allFunction.getFunction(functionName, types);
            }
            else
            {
                function = m_globalFunctions.getFunction(functionName, types);
            }

            type = function->returnType();
            return;
        }
        else if (currentNode->type == NodeType::INDEX_CAPTURE)
        {
            string array_name = currentNode->op1->value;
            size_t block_id = currentNode->op1->idStatementWhereNodeDefined();

            VariableType array_type = m_allVariables.getVariableByName(array_name)->type();

            type = array_type == VariableType::INTEGER_ARRAY ? VariableType::INTEGER : VariableType::UNDEFINED;
            return;
        }


        giveExpressionTypeRecursive(currentNode->op1, type);
        giveExpressionTypeRecursive(currentNode->op2, type);
        giveExpressionTypeRecursive(currentNode->op3, type);
        giveExpressionTypeRecursive(currentNode->op4, type);
    }
    VariableType variableTypeOfNode(Node* currentNode)
    {
        if (currentNode->type == NodeType::INTEGER_CONST)
        {
            return VariableType::INTEGER;
        }
        else if (currentNode->type == NodeType::STRING_CONST)
        {
            return VariableType::STRING;
        }
        else if (currentNode->type == NodeType::USING_VARIABLE)
        {
            string variableName = currentNode->value;
            size_t variableBlockId = currentNode->idStatementWhereNodeDefined();

            Variable* variable = m_allVariables.getVariable(variableName, variableBlockId);

            return variable->type();
        }
        else if (currentNode->type == NodeType::INDEX_CAPTURE)
        {
            string variableName = currentNode->op1->value;
            size_t variableBlockId = currentNode->op1->idStatementWhereNodeDefined();

            Variable* variable = m_allVariables.getVariable(variableName, variableBlockId);

            return variable->type() == VariableType::INTEGER_ARRAY ? VariableType::INTEGER : VariableType::UNDEFINED;
        }
        else if (currentNode->type == NodeType::VARIABLE_DECLARATION)
        {
            return Variable::typeFromString(currentNode->op1->value);
        }

        return VariableType::UNDEFINED;
    }



public:
    // функция для вывода дерева и таблиц
    void print()
    {
        print(root, 0);


        //cout << "\n\nAll Variables\n\n";
        //m_allVariables.print();
        //cout << "\n\n";

        //cout << "\n\nGlobal Variables\n\n";
        //m_globalVariables.print();
        //cout << "\n\n";

        //cout << "\n\nFunction table\n\n";
        //m_allFunction.print();
        //cout << "\n\n";
    }


private:
    // функция для ошибок
    static void error(const string& message)
    {
        
        throw Err(2, "Semantic error! " + message);
    }

    // функция для распечатки одного узла
    static void print(Node* currentNode, size_t level)
    {
        if (currentNode == nullptr)
            return;

        for (size_t i = 0; i < level; i++)
            cout << "  ";


        switch (currentNode->type)
        {
        case NodeType::VARIABLE_DECLARATION:
        {
            cout << "new var ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::USING_VARIABLE:
        {
            cout << "use var ";
            cout << "" << currentNode->value;

            cout << " (defined in block: " << currentNode->idStatementWhereNodeDefined() << ")";
            break;
        }
        case NodeType::VARIABLE_TYPE:
        {
            cout << "var type ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::INTEGER_CONST:
        {
            cout << "number const ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::ADD:
        {
            cout << "add ";
            break;
        }
        case NodeType::SUB:
        {
            cout << "sub ";
            break;
        }
        case NodeType::MUL:
        {
            cout << "mul ";
            break;
        }
        case NodeType::DIV:
        {
            cout << "div ";
            break;
        }
        case NodeType::LESS:
        {
            cout << "less ";
            break;
        }
        case NodeType::GREATER:
        {
            cout << "greater ";
            break;
        }
        case NodeType::EQUAL:
        {
            cout << "equal ";
            break;
        }
        case NodeType::NOT_EQUAL:
        {
            cout << "not equal ";
            break;
        }
        case NodeType::LESS_EQUAL:
        {
            cout << "less equal ";
            break;
        }
        case NodeType::GREATER_EQUAL:
        {
            cout << "greater equal ";
            break;
        }
        case NodeType::FOR:
        {
            cout << "for ";
            break;
        }

        case NodeType::BREAK:
        {
            cout << "break ";
            break;
        }

        case NodeType::IF:
        {
            cout << "if ";
            break;
        }
        case NodeType::IF_ELSE:
        {
            cout << "if else ";
            break;
        }
        case NodeType::UNARY_MINUS:
        {
            cout << "unary - ";
            break;
        }
        case NodeType::SET:
        {
            cout << "set ";
            break;
        }
        case NodeType::INDEX_CAPTURE:
        {
            cout << "index capture ";
            break;
        }
        case NodeType::FUNCTION_CALL:
        {
            cout << "function call ";
            cout << currentNode->value;
            break;
        }
        case NodeType::FUNCTION_ARG:
        {
            cout << "function arg ";
            break;
        }
        case NodeType::EXPRESSION:
        {
            cout << "expr ";
            break;
        }
        case NodeType::STATEMENT:
        {
            cout << "stmt ";
            cout << " (id: " << currentNode->idStatementWhereNodeDefined() << ")";
            break;
        }
        case NodeType::STATEMENT_LIST:
        {
            cout << "seq ";
            break;
        }
        case NodeType::FUNCTION_IMPLEMENTATION:
        {
            cout << "function impl ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::FUNCTION_IMPLEMENTATION_ARG:
        {
            cout << "function impl arg ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::FUNCTION_IMPLEMENTATION_RETURN_TYPE:
        {
            cout << "function impl return type ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::RETURN:
        {
            cout << "return ";
            cout << "(argument size: " << currentNode->value << "byte)";
            break;
        }
        case NodeType::FUNCTION_IMPLEMENTATION_ARGS:
        {
            cout << "function impl args ";
            break;
        }
        case NodeType::FUNCTION_ARGS:
        {
            cout << "function args ";
            break;
        }
        case NodeType::STRING_CONST:
        {
            cout << "string const ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::IDENTIFIER:
        {
            cout << "identifier ";
            cout << "" << currentNode->value;
            break;
        }
        case NodeType::WHILE:
        {
            cout << "while ";
            break;
        }
        case NodeType::REPEAT:
        {
            cout << "repeat ";
            break;
        }
        case NodeType::RANGE:
        {
            cout << "range ";
            break;
        }
        }


        cout << endl;

        print(currentNode->op1, level + 1);
        print(currentNode->op2, level + 1);
        print(currentNode->op3, level + 1);
        print(currentNode->op4, level + 1);
    }
};



