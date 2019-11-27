/* Программа-заготовка для домашнего задания
*/

#include "hw/l3_DomainLayer.h"

using namespace std;


bool Order::invariant() const
{
  return !_courier_name.empty() && _courier_name.size() <= MAX_NAME_LENGTH
      && _good_name.size() >= MIN_GOOD_NAME_LENGTH && _good_name.size() <= MAX_GOOD_NAME_LENGTH
      && !_address.empty() && _address.size() <= MAX_ADDRESS_NAME_LENGTH
      && _order_number > 0
      && _total > 0;
}

Order::Order(uint16_t order_number, const std::string & courier_name, const std::string & address, const std::string & good_name, uint16_t total)
  : _order_number(order_number), _courier_name(courier_name), _address(address), _good_name(good_name), _total(total)
{
    assert(invariant());
}

uint16_t       Order::getOrderNumber() const { return _order_number; }
const string & Order::getCourierName() const { return _courier_name; }
const string & Order::getAddress() const { return _address; }
const string & Order::getGoodName() const { return _good_name; }
uint16_t Order::getTotal() const { return _total; }

bool    Order::write(ostream& os)
{
    writeNumber(os, _order_number);
    writeString(os, _courier_name);
    writeString(os, _address);
    writeString(os, _good_name);
    writeNumber(os, _total);

    return os.good();
}



shared_ptr<ICollectable> ItemCollector::read(istream& is)
{
  uint16_t order_number = readNumber<uint16_t>(is);
  string   courier_name = readString(is, MAX_NAME_LENGTH);
  string   address = readString(is, MAX_ADDRESS_NAME_LENGTH);
  string   good_name = readString(is, MAX_GOOD_NAME_LENGTH);
  uint16_t total = readNumber<uint16_t>(is);

  return std::make_shared<Order>(order_number, courier_name, address, good_name, total);
}
