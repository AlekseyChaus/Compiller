#pragma once

#include "Function.h"

class FunctionTable
{
private:
    vector<Function*> m_functions;

public:
    FunctionTable() = default;

    ~FunctionTable()
    {
        m_functions.clear();
    }


public:
    void addFunction(Function* newFunction) 
    {
        m_functions.push_back(newFunction);
    }

    bool hasFunction(Function* function) //проверка на наличие функции по имени
    {
        for (const auto& item : m_functions)
        {
            if (*item == *function)
                return true;
        }

        return false;
    }
    //функци€ дл€ проверки соответстви€ аргументов переданных и объ€вленных
    Function* getFunction(const string& name, const vector<VariableType>& argumentsDescription)  
    {
        auto it = std::find_if(m_functions.begin(), m_functions.end(), [&](Function* currentFunc)
            {
                bool eq = currentFunc->name() == name &&
                    currentFunc->arguments().size() == argumentsDescription.size();

                if (!eq)
                    return false;

                const vector<VariableType>& currentArgs = currentFunc->arguments();

                for (int i = currentArgs.size() - 1, j = 0; i >= 0; --i, ++j)
                {
                    if (currentArgs[i] != argumentsDescription[j])
                    {
                        return false;
                    }
                }

                return true;
            });

        if (it == m_functions.end())
        {
           
            throw Err(2, "Error! A Function with name '" + name + "' and this parameter list was not found!");
        }

        return *it;
    }

    Function* getFunction(const string& name)
    {
        auto it = std::find_if(m_functions.begin(), m_functions.end(), [&](Function* currentFunc)
            {
                return currentFunc->name() == name;
            });

        if (it == m_functions.end())
        {
            throw Err(2, "Error! A Function with name '" + name + "' was not found!");
        }

        return *it;
    }

    const vector<Function*>& functions()
    {
        return m_functions;
    }

public:
    void print()
    {
        for (const auto& function : m_functions)
        {
            function->print();
        }
    }

};