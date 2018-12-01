#include <iostream>
#include <fstream>
#include <numeric>
#include <set>
#include <vector>
#include <iterator>

int findSumSeenTwice(const std::vector<int>& changes) {
    std::set<int> sumsHit = {0};
    std::vector<int>::const_iterator it = changes.begin();
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
    std::ifstream infile("input.txt");
    std::copy(std::istream_iterator<int>(infile),
              std::istream_iterator<int>(),
              std::back_inserter(allChanges));

    std::cout << "Sum of Numbers: " << std::accumulate(allChanges.begin(), allChanges.end(), 0) << std::endl;
    std::cout << "Sum seen twice: " << findSumSeenTwice(allChanges) << std::endl;
    return 1;
}
