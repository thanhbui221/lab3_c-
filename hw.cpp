/* 
MIT License 

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов, 

Программа-заготовка для домашнего задания
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#include <sstream>

#include "collector.h"

using namespace std;

const size_t MAX_NAME_LENGTH    = 50;
const size_t MIN_YEAR_OF_BIRTH  = 1900;
const size_t MAX_YEAR_OF_BIRTH  = 2019;

class Person
{
    string   _first_name;
    string   _last_name;
    uint16_t _year_of_birth;

protected:
    bool invariant() const
    {
        return _year_of_birth >= MIN_YEAR_OF_BIRTH
            && _year_of_birth <= MAX_YEAR_OF_BIRTH
            && !_first_name.empty() && _first_name.size() <= MAX_NAME_LENGTH 
            && !_last_name.empty() && _last_name.size() <= MAX_NAME_LENGTH;
    }

public:
    Person() = delete;

    Person(const string & first_name, const string & last_name, uint16_t year_of_birth)
        : _first_name(first_name), _last_name(last_name), _year_of_birth(year_of_birth)
    {
        assert(invariant());
    }

    const string & getFirstName() const { return _first_name; }
    const string & getLastName() const { return _last_name; }
    uint16_t       getYearOfBirth() const { return _year_of_birth; }

    static bool    write(const Person &p, ostream& os)
    {
        writeString(os, p.getFirstName());
        writeString(os, p.getLastName());
        writeNumber(os, p.getYearOfBirth());

        return os.good();
    }

    static Person  read(istream& is)
    {
        string   first_name = readString(is, MAX_NAME_LENGTH);
        string   last_name = readString(is, MAX_NAME_LENGTH);
        uint16_t year = readNumber<uint16_t>(is);

        return Person(first_name, last_name, year);
    }
};

bool performCommand(const vector<string> & args, Collector<Person> & col)
{
    if (args.empty())
        return false;

    if (args[0] == "l" || args[0] == "load")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!col.loadCollection(filename))
        {
            cerr << "Ошибка при загрузке файла '" << filename << "'" << endl;
            return false;
        }

        return true;
    }

    if (args[0] == "s" || args[0] == "save")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!col.saveCollection(filename))
        {
            cerr << "Ошибка при сохранении файла '" << filename << "'" << endl;
            return false;
        }

        return true;
    }

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            cerr << "Некорректное количество аргументов команды clean" << endl;
            return false;
        }

        col.clean();

        return true;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 4)
        {
            cerr << "Некорректное количество аргументов команды add" << endl;
            return false;
        }

        col.addItem(Person(args[1].c_str(), args[2].c_str(), stoul(args[3])));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 5)
        {
            cerr << "Некорректное количество аргументов команды update" << endl;
            return false;
        }

        col.updateItem(stoul(args[1]), Person(args[2].c_str(), args[3].c_str(), stoul(args[4])));
        return true;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            cerr << "Некорректное количество аргументов команды remove" << endl;
            return false;
        }

        col.removeItem(stoul(args[1]));
        return true;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            cerr << "Некорректное количество аргументов команды view" << endl;
            return false;
        }

        size_t count = 0;
        for(size_t i=0; i < col.getSize(); ++i)
        {
            Person item = col.getItem(i);

            if (!col.isRemoved(i))
            {
                cout << "[" << i << "] " 
                        << item.getFirstName() << " " 
                        << item.getLastName() << " " 
                        << item.getYearOfBirth() << endl;
                count ++;
            }
        }

        cout << "Количество элементов в коллекции: " << count << endl;

        return true;
    }

    cerr << "Недопустимая команда '" << args[0] << "'" << endl;
    return false;
}

int main(int , char **)
{
    Collector<Person> col;

    for (string line; getline(cin, line); )
    {
        if (line.empty())
            break;

        istringstream  iss(line);
        vector<string> args;
    
        for(string str; iss.good();)
        {
            iss >> str;
            args.emplace_back(str);
        }

        if (!performCommand(args, col))
            return 1;
    }

    cout << "Выполнение завершено успешно" << endl;
    return 0;
}