#include <iostream>
#include <numeric>
#include <set>
#include <vector>
#include <iterator>
#include "fileParser.h"

int findSumSeenTwice(const std::vector<int>& changes) {
    std::set<int> sumsHit = {0};
    auto it = changes.begin();
    int sum = 0;
    while(true) {
        if(it == changes.end()) { it = changes.begin(); }
        sum += *it;
        ++it;
        if(sumsHit.find(sum) != sumsHit.end()) {
            return sum;
        }
        sumsHit.insert(sum);
    }
    return 0;
}

int main(){
    std::vector<int> allChanges;
    fileParse::storeEachLine("./challenges/challenge1/input.txt", allChanges);

    std::cout << "Sum of Numbers: " << std::accumulate(allChanges.begin(), allChanges.end(), 0) << "\n";
    std::cout << "Sum seen twice: " << findSumSeenTwice(allChanges) << "\n";
    return 0;
}
