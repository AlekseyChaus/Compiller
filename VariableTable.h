#pragma once
#include <vector>
#include "Variable.h"
#include "error.h"

class VariableTable
{
private:
    size_t m_blockId; //идентификатор блока

    vector<Variable*> m_variables;//вектор переменных

    VariableTable* m_parent;

public:
    explicit VariableTable(size_t blockId)
    {
        this->m_blockId = blockId;
        this->m_parent = nullptr;
    }

    VariableTable()
    {
        this->m_blockId = -1;
        this->m_parent = nullptr;
    }

    ~VariableTable()
    {
        m_variables.clear();
    }

public:
    size_t blockId() const
    {
        return m_blockId;
    }

    void blockId(size_t blockId)
    {
        m_blockId = blockId;
    }


    void setParent(VariableTable* parent)
    {
        m_parent = parent;
    }


    void addVariable(Variable* variable) //добавление переменной
    {
        m_variables.push_back(variable);
    }

    bool hasVariable(const string& name) // метод, возвращающий true, если такая переменная есть в таблице:
    {
        auto it = std::find_if(m_variables.begin(), m_variables.end(), [&name](Variable* var)
            {
                return var->name() == name;
            });

        return it != m_variables.end();
    }

    bool hasVariableGlobally(const string& name) // проверка на существование такой глобальной переменной в таблице по имени
    {
        if (!hasVariable(name))
        {
            if (m_parent == nullptr)
            {
                return false;
            }
            else
            {
                return m_parent->hasVariableGlobally(name);
            }
        }
        else
        {
            return true;
        }
    }


    Variable* getVariableByName(const string& name) //проверка наа наличие переменной с прописанным блоком ошибки
    {
        auto it = std::find_if(m_variables.begin(), m_variables.end(), [&name](Variable* var)
            {
                return var->name() == name;
            });

        if (it == m_variables.end())
        {
            if (m_parent == nullptr)
            {
                throw Err(2, "Variable '" + name + "'not found!");
            }
            else
            {
                return m_parent->getVariableByName(name);
            }
        }

        return *it;
    }

    Variable* getVariable(const string& name, size_t block_id)//взятие переменной для генерации кода
    {
        auto it = std::find_if(m_variables.begin(), m_variables.end(), [&name, &block_id](Variable* var)
            {
                return var->name() == name && var->blockId() == block_id;
            });

        if (it == m_variables.end())
        {
            throw Err(2, "Variable '" + name + "' from block " + std::to_string(block_id) + " not found!");
        }

        return *it;
    }
    //возвращает id блока нахождения
    tuple<size_t, Variable*> getVariableAndBlockIdWhereItDefined(const string& name)
    {
        auto it = std::find_if(m_variables.begin(), m_variables.end(), [&name](Variable* var)
            {
                return var->name() == name;
            });


        if (it == m_variables.end())
        {
            if (m_parent == nullptr)
            {
                throw Err(2,"Variable '" + name + "'not found!");
            }
            else
            {
                return m_parent->getVariableAndBlockIdWhereItDefined(name);
            }
        }

        return make_tuple(m_blockId, *it);
    }


    const vector<Variable*>& variables() const
    {
        return m_variables;
    }

    void print()
    {
        for (auto& variable : m_variables)
        {
            variable->print();
        }
    }

};