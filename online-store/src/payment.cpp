#include "payment.h"

bool CardPayment::pay(double amount){ return amount>=0; }
bool WalletPayment::pay(double amount){ return amount>=0; }
bool SBPPayment::pay(double amount){ return amount>=0; }
