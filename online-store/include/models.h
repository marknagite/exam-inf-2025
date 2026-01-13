#pragma once
#include <string>
#include <vector>
#include <memory>
#include <numeric>

class OrderItem {
public:
    OrderItem(int pid,int q,double p):product_id(pid),quantity(q),price(p){}
    int product_id;
    int quantity;
    double price;
};

class PaymentStrategy {
public:
    virtual bool pay(double amount)=0;
    virtual ~PaymentStrategy()=default;
};

class Payment {
public:
    Payment(std::unique_ptr<PaymentStrategy> s):strategy(std::move(s)){}
    bool process(double amount){return strategy->pay(amount);}
private:
    std::unique_ptr<PaymentStrategy> strategy;
};

class Order {
public:
    Order(int id,int uid);
    void addItem(int product_id,int qty,double price);
    void removeItem(int product_id);
    double total() const;
void setStatus(const std::string &s);
    const std::string &getStatus() const;
    int id;
    int user_id;
    std::vector<std::unique_ptr<OrderItem>> items;
    std::unique_ptr<Payment> payment;
private:
    std::string status;
};

class User {
public:
    User(int id,const std::string &n,const std::string &e,const std::string &r);
    virtual ~User()=default;
    virtual std::shared_ptr<Order> createOrder();
    virtual void viewOrderStatus(std::shared_ptr<Order> o);
    virtual void cancelOrder(std::shared_ptr<Order> o);
    int id;
    std::string name;
    std::string email;
    std::string role;
    std::vector<std::shared_ptr<Order>> orders;
};

class Admin: public User {
public:
    Admin(int id,const std::string &n,const std::string &e);
    void addProduct(int pid,const std::string &name,double price,int qty);
    void updateProduct(int pid,const std::string &name,double price,int qty);
    void deleteProduct(int pid);
    std::vector<std::shared_ptr<Order>> viewAllOrders();
    void updateOrderStatus(std::shared_ptr<Order> o,const std::string &status);
};

class Manager: public User {
public:
    Manager(int id,const std::string &n,const std::string &e);
    void approveOrder(std::shared_ptr<Order> o);
    void updateStock(int pid,int qty);
};

class Customer: public User {
public:
    Customer(int id,const std::string &n,const std::string &e);
    void addToOrder(std::shared_ptr<Order> o,int pid,int qty,double price);
    void removeFromOrder(std::shared_ptr<Order> o,int pid);
    bool makePayment(std::shared_ptr<Order> o,std::unique_ptr<PaymentStrategy> strat);
    void returnProduct(std::shared_ptr<Order> o,int pid);
};
