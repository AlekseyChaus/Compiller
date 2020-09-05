#pragma once
#include <vector>
#include "Variable.h"

class Function
{
private:
    string m_name; //им€ 
    VariableType m_returnType; //тип возвращаемого значени€
    vector<VariableType> m_arguments; // все аргументы

    size_t m_argumentsSize; // размер аргументов дл€ сверки с передаваемыми
    size_t m_localVariablesSize; //размер переменных объ€вленных внутри функции

    vector<Variable*> m_localVariables;
    vector<Variable*> m_argumentVariables;


public:
    Function(const string& name, VariableType returnType, const vector<VariableType>& arguments,
        size_t sizeLocalVariable = 0, const vector<Variable*>& localVariables = {},
        const vector<Variable*>& argumentVariables = {})
    {
        this->m_name = name;
        this->m_returnType = returnType;
        this->m_arguments = arguments;
        this->m_argumentsSize = 0;
        this->m_localVariablesSize = sizeLocalVariable;
        this->m_localVariables = localVariables;
        this->m_argumentVariables = argumentVariables;

        for (const auto& argument : arguments)
        {
            this->m_argumentsSize += 4;
        }
    }

public:
    void print()
    {
        cout << "======== function '" << m_name << "' description ========" << endl;

        cout << "1. Return type: " << Variable::typeToString(m_returnType) << endl;
        cout << "2. Arguments size: " << m_argumentsSize << " byte" << endl;
        cout << "3. Local variable size: " << m_localVariablesSize << " byte" << "\n\n";


        cout << "Argument variables:" << endl;
        if (m_argumentVariables.empty()) cout << "   Empty" << endl;
        for (const auto& argumentVariable : m_argumentVariables)
        {
            cout << "   ";
            argumentVariable->print();
        }

        cout << "\n\n";

        cout << "Local variables:" << endl;
        if (m_localVariables.empty()) cout << "   Empty" << endl;
        for (const auto& localVariable : m_localVariables)
        {
            cout << "   ";
            localVariable->print();
        }



        cout << "======== End function description ========" << "\n\n\n\n";
    }

public:
    bool operator==(const Function& right) const
    {
        bool eq = m_name == right.m_name &&
            m_returnType == right.m_returnType &&
            m_arguments.size() == right.m_arguments.size();


        if (!eq)
            return false;

        for (int i = 0; i < m_arguments.size(); ++i)
        {
            if (m_arguments[i] != right.m_arguments[i])
            {
                return false;
            }
        }

        return true;
    }


    string name() const
    {
        return m_name;
    }

    VariableType returnType() const
    {
        return m_returnType;
    }

    const vector<VariableType>& arguments() const
    {
        return m_arguments;
    }

    size_t argumentsSize() const
    {
        return m_argumentsSize;
    }

    size_t localVariablesSize() const
    {
        return m_localVariablesSize;
    }

    const vector<Variable*>& localVariables() const
    {
        return m_localVariables;
    }

    const vector<Variable*>& argumentVariables() const
    {
        return m_argumentVariables;
    }
};