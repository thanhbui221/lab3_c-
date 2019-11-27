/*
MIT License

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов,

Программа-заготовка для домашнего задания
*/

#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include "hw/l4_InfrastructureLayer.h"

const size_t MAX_NAME_LENGTH    = 20;
const size_t MIN_GOOD_NAME_LENGTH  = 2;
const size_t MAX_GOOD_NAME_LENGTH  = 10;
const size_t MAX_ADDRESS_NAME_LENGTH  = 100;

class Order : public ICollectable
{
  uint16_t _order_number;
  std::string _courier_name;
  std::string _address;
  std::string _good_name;
  uint16_t _total;

protected:
    bool invariant() const;

public:
    Order() = delete;
    Order(const Order & p) = delete;

    Order & operator = (const Order & p) = delete;

    Order(uint16_t order_number, const std::string & courier_name, const std::string & address, const std::string & good_name, uint16_t total);

    uint16_t       getOrderNumber() const ;
    const std::string & getCourierName() const ;
    const std::string & getAddress() const ;
    const std::string & getGoodName() const ;
    uint16_t getTotal() const ;

    virtual bool   write(std::ostream& os) override;
};


class ItemCollector: public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream& is) override;
};

#endif // HW_L3_DOMAIN_LAYER_H
