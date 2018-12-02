#include <algorithm>
#include <map>
#include <string>

namespace stringParse {

    std::map<char, int> getLetterFrequencies(const std::string& inputString) {
        std::map<char, int> outputMap;
        for(auto letter : inputString) {
            outputMap[letter] += 1;
        }
        return outputMap;
    }
 
}
