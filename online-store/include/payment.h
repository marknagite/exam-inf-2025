#pragma once
#include "models.h"
#include <iostream>

class CardPayment : public PaymentStrategy {
public:
    bool pay(double amount) override;
};

class WalletPayment : public PaymentStrategy {
public:
    bool pay(double amount) override;
};

class SBPPayment : public PaymentStrategy {
public:
    bool pay(double amount) override;
};
