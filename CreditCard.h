// The file is for work with credit cards

#ifndef CREDITCARD_H
#define CREDITCARD_H

#include <string>

const int kIINdigits = 6; // number of digits of issuer identification number

std::string GetCreditCardVendor(const std::string& cardNumber);
bool IsCreditCardNumberValid(const std::string& cardNumber);
std::string GenerateNextCreditCardNumber(const std::string& cardNumber);

#endif // CREDITCARD_H
