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

const size_t MAX_NAME_LENGTH    = 20;
const size_t MIN_GOOD_NAME_LENGTH  = 2;
const size_t MAX_GOOD_NAME_LENGTH  = 10;
const size_t MAX_ADDRESS_NAME_LENGTH  = 100;

class Order
{
    uint16_t _order_number;
    string _courier_name;
    string _address;
    string _good_name;
    uint16_t _total;

protected:
    bool invariant() const
    {
        return !_courier_name.empty() && _courier_name.size() <= MAX_NAME_LENGTH
            && _good_name.size() >= MIN_GOOD_NAME_LENGTH && _good_name.size() <= MAX_GOOD_NAME_LENGTH
            && !_address.empty() && _address.size() <= MAX_ADDRESS_NAME_LENGTH
            && _order_number > 0
            && _total > 0;
    }

public:
    Order() = delete;

    Order(uint16_t order_number, const string & courier_name, const string & address, const string & good_name, uint16_t total)
      : _order_number(order_number), _courier_name(courier_name), _address(address), _good_name(good_name), _total(total)
    {
        assert(invariant());
    }

    uint16_t       getOrderNumber() const { return _order_number; }
    const string & getCourierName() const { return _courier_name; }
    const string & getAddress() const { return _address; }
    const string & getGoodName() const { return _good_name; }
    uint16_t getTotal() const { return _total; }


    static bool    write(const Order &o, ostream& os)
    {
        writeNumber(os, o.getOrderNumber());
        writeString(os, o.getCourierName());
        writeString(os, o.getAddress());
        writeString(os, o.getGoodName());
        writeNumber(os, o.getTotal());

        return os.good();
    }

    static Order  read(istream& is)
    {
        uint16_t order_number = readNumber<uint16_t>(is);
        string   courier_name = readString(is, MAX_NAME_LENGTH);
        string   address = readString(is, MAX_ADDRESS_NAME_LENGTH);
        string   good_name = readString(is, MAX_GOOD_NAME_LENGTH);
        uint16_t total = readNumber<uint16_t>(is);

        return Order(order_number, courier_name, address, good_name, total);
    }
};

bool performCommand(const vector<string> & args, Collector<Order> & col)
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
        if (args.size() != 6)
        {
            cerr << "Некорректное количество аргументов команды add" << endl;
            return false;
        }

        col.addItem(Order(stoul(args[1]), args[2].c_str(), args[3].c_str(), args[4].c_str(), stoul(args[5])));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 7)
        {
            cerr << "Некорректное количество аргументов команды update" << endl;
            return false;
        }

        col.updateItem(stoul(args[1]), Order(stoul(args[2]), args[3].c_str(), args[4].c_str(), args[5].c_str(), stoul(args[6])));
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
            Order item = col.getItem(i);

            if (!col.isRemoved(i))
            {
                cout << "[" << i << "] "
                        << item.getOrderNumber() << " "
                        << item.getCourierName() << " "
                        << item.getAddress() << " "
                        << item.getGoodName() << " "
                        << item.getTotal() << endl;
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
    Collector<Order> col;

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
