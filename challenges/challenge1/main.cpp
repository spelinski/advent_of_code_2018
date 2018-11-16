//THIS IS A SAMPLE TO TO GET MAKEFILE WORKING BEFORE ADVENT STARTS
#include "sum.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

int main(){
    std::cout << "Please enter in an upper limit (up to 2^32)\n";
    auto limit = 0u;
    std::cin >> limit;

    auto sum = temp::user_sum(limit);

    std::cout << "The sum total is " << sum << "\n";
    return 0;
}