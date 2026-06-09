#include <iostream>

double getTotal(double prices[], int size)
{
    double total = 0;

    for (int i = 0; i < size; i++)
    {
        total += prices[i];
    }

    return total;
}

int main()
{
    double prices[] = {50.00, 77.50, 19.99, 15.00, 999.99};
    int size = sizeof(prices) / sizeof(prices[0]);
    double total = getTotal(prices, size);

    std::cout << "$" << total;
    return 0;
}