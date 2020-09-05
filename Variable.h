#pragma once

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

enum class VariableType
{
    UNDEFINED,
    INTEGER,
    STRING,

    INTEGER_ARRAY,
     
    VOID
};

class Variable
{
private:
    VariableType m_type; //тип 
    string m_name; // им€ 

    size_t m_blockId; //номер блока

    bool m_isArray; //массив ли 
    size_t m_arraySize; //размер массива


    bool m_isArgument; //€вл€етс€ ли аргументом функции 
    bool m_isGlobal; // объ€влена ли в программе

public:
    Variable(const string& name, VariableType type, size_t blockId, size_t arraySize = 0)
    {
        this->m_name = name;
        this->m_type = type;
        this->m_blockId = blockId;

        this->m_isArray = type == VariableType::INTEGER_ARRAY;
        this->m_arraySize = arraySize;

        this->m_isArgument = false;
        this->m_isGlobal = blockId == 0;
    }


public:
    static VariableType typeFromString(const string& value)
    {
        if (value == "integer")
            return VariableType::INTEGER;

        if (value == "character")
            return VariableType::STRING;

        if (value == "void")
            return VariableType::VOID;

        if (value == "integer array")
            return VariableType::INTEGER_ARRAY;

        return VariableType::UNDEFINED;
    }
    static string typeToString(VariableType type)
    {
        switch (type)
        {
        case VariableType::INTEGER:
            return "integer";
        case VariableType::INTEGER_ARRAY:
            return "integer[]";
        case VariableType::STRING:
            return "character";
        case VariableType::VOID:
            return "void";
        default:
            return "not defined, possible error";
        }
    }



    void print()
    {
        cout << "name: '" << m_name << "' with type: '" << typeToString(m_type);

        if (m_isArray)
        {
            cout << " (size: " << m_arraySize << ")";
        }

        cout << "' in block: '" << m_blockId << "'" << endl;
    }

public:

    bool isArgument() const
    {
        return m_isArgument;
    }
    void isArgument(bool isArgument)
    {
        m_isArgument = isArgument;
    }

    bool isGlobal() const
    {
        return m_isGlobal;
    }
    void isGlobal(bool isGlobal)
    {
        m_isGlobal = isGlobal;
    }

    VariableType type() const
    {
        return m_type;
    }
    void type(VariableType mType)
    {
        m_type = mType;
    }

    string nameWithPostfix() const
    {
        return m_name + to_string(m_blockId);
    }
    string name() const
    {
        return m_name;
    }
    void name(const string& mName)
    {
        m_name = mName;
    }

    size_t blockId() const
    {
        return m_blockId;
    }
    void blockId(size_t mBlockId)
    {
        m_blockId = mBlockId;
    }

    bool isArray() const
    {
        return m_isArray;
    }
    void isArray(bool mIsArray)
    {
        m_isArray = mIsArray;
    }

    size_t arraySize() const
    {
        return m_arraySize;
    }
    void arraySize(size_t mArraySize)
    {
        m_arraySize = mArraySize;
    }
};