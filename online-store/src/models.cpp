#include "models.h"
#include <algorithm>

Order::Order(int id,int uid):id(id),user_id(uid),status("pending"){}
void Order::addItem(int product_id,int qty,double price){
    items.push_back(std::make_unique<OrderItem>(product_id,qty,price));
}
void Order::removeItem(int product_id){
    items.erase(std::remove_if(items.begin(),items.end(),[&](const std::unique_ptr<OrderItem>&it){return it->product_id==product_id;}),items.end());
}
double Order::total() const{
    return std::accumulate(items.begin(),items.end(),0.0,[](double acc,const std::unique_ptr<OrderItem>&it){return acc+it->price*it->quantity;});
}
void Order::setStatus(const std::string &s){status=s;}
const std::string &Order::getStatus() const{return status;}

User::User(int id,const std::string &n,const std::string &e,const std::string &r):id(id),name(n),email(e),role(r){}
std::shared_ptr<Order> User::createOrder(){auto o=std::make_shared<Order>(0,id); orders.push_back(o); return o;}
void User::viewOrderStatus(std::shared_ptr<Order> o){}
void User::cancelOrder(std::shared_ptr<Order> o){o->setStatus("canceled");}

Admin::Admin(int id,const std::string &n,const std::string &e):User(id,n,e,"admin"){}
void Admin::addProduct(int pid,const std::string &name,double price,int qty){}
void Admin::updateProduct(int pid,const std::string &name,double price,int qty){}
void Admin::deleteProduct(int pid){}
std::vector<std::shared_ptr<Order>> Admin::viewAllOrders(){return {};}
void Admin::updateOrderStatus(std::shared_ptr<Order> o,const std::string &status){o->setStatus(status);}

Manager::Manager(int id,const std::string &n,const std::string &e):User(id,n,e,"manager"){}
void Manager::approveOrder(std::shared_ptr<Order> o){o->setStatus("completed");}
void Manager::updateStock(int pid,int qty){}

Customer::Customer(int id,const std::string &n,const std::string &e):User(id,n,e,"customer"){}
void Customer::addToOrder(std::shared_ptr<Order> o,int pid,int qty,double price){o->addItem(pid,qty,price);}
void Customer::removeFromOrder(std::shared_ptr<Order> o,int pid){o->removeItem(pid);}
bool Customer::makePayment(std::shared_ptr<Order> o,std::unique_ptr<PaymentStrategy> strat){
    o->payment = std::make_unique<Payment>(std::move(strat));
    bool ok = o->payment->process(o->total());
    if(ok) o->setStatus("completed");
    return ok;
}
void Customer::returnProduct(std::shared_ptr<Order> o,int pid){
auto it = std::find_if(o->items.begin(),o->items.end(),[&](const std::unique_ptr<OrderItem>&it){return it->product_id==pid;});
    if(it!=o->items.end()) o->removeItem(pid);
}
