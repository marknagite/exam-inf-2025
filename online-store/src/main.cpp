#include <iostream>
#include <memory>
#include "DatabaseConnection.h"
#include "models.h"
#include "payment.h"
using DB = DatabaseConnection<std::string>;
int main(){
    std::string connstr="host=localhost user=postgres password=password dbname=onlinestore";
    try{
        DB db(connstr);
    } catch(...){
        std::cout<<"DB connection failed\n";
    }
    auto admin = std::make_shared<Admin>(1,"admin","a@a");
    auto manager = std::make_shared<Manager>(2,"mgr","m@m");
    auto customer = std::make_shared<Customer>(3,"cust","c@c");
    while(true){
        std::cout<<"1 Admin\n2 Manager\n3 Customer\n4 Exit\n";
        int role; if(!(std::cin>>role)) break;
        if(role==4) break;
        if(role==1){
            while(true){
                std::cout<<"1 Add product\n2 Update product\n3 Delete product\n4 Back\n";
                int c; if(!(std::cin>>c)) return 0;
                if(c==4) break;
                if(c==1){ int pid; std::string name; double price; int qty; std::cin>>pid>>name>>price>>qty; admin->addProduct(pid,name,price,qty); }
                if(c==2){ int pid; std::string name; double price; int qty; std::cin>>pid>>name>>price>>qty; admin->updateProduct(pid,name,price,qty); }
                if(c==3){ int pid; std::cin>>pid; admin->deleteProduct(pid); }
            }
        } else if(role==2){
            while(true){
                std::cout<<"1 Approve order\n2 Update stock\n3 Back\n";
                int c; if(!(std::cin>>c)) return 0;
                if(c==3) break;
                if(c==1){ }
                if(c==2){ int pid,qty; std::cin>>pid>>qty; manager->updateStock(pid,qty); }
            }
        } else if(role==3){
            auto o = customer->createOrder();
            while(true){
                std::cout<<"1 Add item\n2 Remove item\n3 Pay\n4 Back\n";
                int c; if(!(std::cin>>c)) return 0;
                if(c==4) break;
                if(c==1){ int pid,qty; double price; std::cin>>pid>>qty>>price; customer->addToOrder(o,pid,qty,price); }
                if(c==2){ int pid; std::cin>>pid; customer->removeFromOrder(o,pid); }
                if(c==3){ int t; std::cin>>t; if(t==1) customer->makePayment(o,std::make_unique<CardPayment>()); if(t==2) customer->makePayment(o,std::make_unique<WalletPayment>()); if(t==3) customer->makePayment(o,std::make_unique<SBPPayment>()); }
            }
        }
    }
    return 0;
}
