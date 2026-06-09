#include <iostream>

int getDigit(const int number) {
    return number % 10 + (number /10 % 10);
}

int sumOddDigits(const std::string cardNumber) {
    int sum = 0;

    for(int i = cardNumber.size() - 1; i >= 0; i-=2){
        sum += cardNumber[i] - '0';
    }
    
    return sum;
}

int sumEvenDigits(const std::string cardNumber) {
    int sum = 0;

    for(int i = cardNumber.size() - 2; i >= 0; i-=2){
        sum += getDigit((cardNumber[i] - '0') * 2);
    }
    
    return sum;
}

int main() {
    std::string cardNumber;
    int results = 0;

    std::cout << "Enter a credit card Number: ";
    std::cin >> cardNumber;

    results = sumEvenDigits(cardNumber) + sumOddDigits(cardNumber);

    if(results % 10 == 0){
        std::cout << "Card number is Valid!\n";
    } else {
        std::cout << "Card number is not Valid!\n";
    }

    return 0;
}

//6011 0009 9013 9424
//6 1  0 0  9 1  9 2
//1 2 2  0 0  1 8 2  1 8 4
//  5  +  0  + 11  +  13 = 29
// 0 1  0 9  0 3  4 4 = 21 == 21 + 29 == 50
//it can be devided by 10 so its a valid credit card number


//6011 1111 1111 1117
//6 1  1 1  1 1  1 1 
//1 2 2  2 2  2 2  2 2 
//  5     4    4    4 = 17
// 0 1  1 1  1 1  1 7 = 13 == 13 + 17 = 30
//it can be devided by 10 so its a valid credit card number

//4012 8888 8888 1881
//4 1  8 8  8 8  1 8 
//8 2  1 6 1 6  1 6 1 6  2 1 6
// 10     14       14      9  = 43 
// 0 2  8 8  8 8  8 1 = 43 == 43 + 43 == 86
//it cant be devided by 10 so its not a valid credit card number