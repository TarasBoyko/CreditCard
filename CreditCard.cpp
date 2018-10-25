#include <QCoreApplication>
#include <assert.h>
#include <ctype.h>
#include <algorithm>
#include "CreditCard.h"

using std::string;
using std::remove;
using std::to_string;

static int CalculateSumOfAllDigits(const string& individualAccountIdentifier,  bool withCheckDigit);
static string RemoveSpacesFromCreditCard(const string& inputCardNumber);

// // PUBLIC FUNCTIONS

// Retunrs credit card vendor by @inputCardNumber.
// If succeeded to find out the vendor - returns the name of the vendor, otherwise - returns "Unknown"
// @inputCardNumber specifies credit card of the vendor.
string GetCreditCardVendor(const string& inputCardNumber)
{
    string cardNumber = RemoveSpacesFromCreditCard(inputCardNumber);

    const string first2 = cardNumber.substr(0, 2); // first two digit in a credit card
    const string first3 = cardNumber.substr(0, 3); // first three digit in a credit card
    const string first4 = cardNumber.substr(0, 4); // first four digit in a credit card
    const string first6 = cardNumber.substr(0, 6); // first six digit in a credit card

    if ( first2 == "34" || first2 == "37" )
    {
        return "American Express";
    } else if ( first2 == "50" ||
                first3 == "639" ||
                first2 == "67" ||
                ( "56" <= first2 && first2 <= "58" )
                )
    {
        return "Maestro";
    } else if ( ("51" <= first2 && first2 <= "55") || ("222100" <= first6 && first6 <= "272099") )
    {
        return "Mastercard";
    } else if ( cardNumber[0] == '4' )
    {
        return "Visa";
    } else if ( "3528" <= first4 && first4 <= "3589" )
    {
        return "JCB";
    }
    else
    {
        return "Unknown";
    }
}

// If @inputCardNumber is valid - returns true, otherwise - return false
// The verification is done using Luhn algorithm.
// @inputCardNumber specifies credit card of the vendor.
bool IsCreditCardNumberValid(const string& inputCardNumber)
{
    string cardNumber = RemoveSpacesFromCreditCard(inputCardNumber);
    return CalculateSumOfAllDigits(cardNumber, true) % 10 == 0;
}

// Generates next credit card number with the same issuer identification number.
// If there is a next credit card number with the same issuer identification number -
// returns next credit card number with the same issuer identification number,
// otherwise - returns "Unknown".
// @inputCardNumber specifies credit card.
string GenerateNextCreditCardNumber(const string& inputCardNumber)
{
    string individualAccountIdentifier = RemoveSpacesFromCreditCard(inputCardNumber);

    // discard issuer identification number and check digit
    const string issuerIdentificationNumber = individualAccountIdentifier.substr(0, kIINdigits);
    individualAccountIdentifier.erase(0, kIINdigits);
    individualAccountIdentifier.resize( individualAccountIdentifier.size() - 1 );


    // check individual account identifier for the highest number
    string limit;
    limit.append(individualAccountIdentifier.size(), '9');
    if ( individualAccountIdentifier == limit )
    {
        return "Unknown";
    }
    // get next individual account identifier
    int x = atoi(individualAccountIdentifier.c_str());
    x++;
    individualAccountIdentifier = std::to_string(x);

    // get check digit
    unsigned checkDigit = (CalculateSumOfAllDigits(issuerIdentificationNumber + individualAccountIdentifier, false) * 9 ) % 10;

    return issuerIdentificationNumber + individualAccountIdentifier + to_string(checkDigit);
}

// PRIVATE FUNCTIONS

// Implements "calculate the sum of all the digits" step of Luhn algorithm.
// The function retunrs the sum of all the digits by @individualAccountIdentifier.
// @individualAccountIdentifier specifies individual account identifier with or without check digit.
// @withCheckDigit specifies specifies if whether a check digit is appended to @individualAccountIdentifier.
static int CalculateSumOfAllDigits(const string& individualAccountIdentifier, bool withCheckDigit)
{
    int sum = 0;
    unsigned nDigits = individualAccountIdentifier.size();
    unsigned nParity = (nDigits-1) % 2;
    if ( withCheckDigit )
    {
        nParity = (nDigits-1) % 2;
    }
    else
    {
        nParity = (nDigits) % 2;
    }
    char cDigit[2] = "\0";
    for (unsigned i = nDigits; i > 0 ; i--)
    {
        cDigit[0]  = individualAccountIdentifier[i-1];
        int nDigit = atoi(cDigit);

        if (nParity == i % 2)
            nDigit = nDigit * 2;

        sum += nDigit / 10;
        sum += nDigit % 10;
    }
    return sum;
}

// Returns @inputCardNumber without spaces.
// @inputCardNumber specifies credit card number.
static string RemoveSpacesFromCreditCard(const string& inputCardNumber)
{
    string cardNumber = inputCardNumber;
    cardNumber.erase( remove(cardNumber.begin(), cardNumber.end(), ' '), cardNumber.end() );
    for ( const char& symbol : cardNumber )
    {
        assert( isdigit(symbol));
    }
    return cardNumber;
}
