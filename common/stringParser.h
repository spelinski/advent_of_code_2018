#include <algorithm>
#include <iterator>
#include <map>
#include <string>
#include <vector>

namespace stringParse {

    std::map<char, int> getLetterFrequencies(const std::string& inputString) {
        std::map<char, int> outputMap;
        for(auto it = inputString.begin(); it != inputString.end(); ++it) {
            outputMap[*it] += 1;
        }
        return outputMap;
    }

    bool hasNumberOfSameLetter(const std::string& inputString, const int sameLetterMatch) {
        std::map<char, int> letterFreqs= getLetterFrequencies(inputString);
        return std::any_of(letterFreqs.begin(), letterFreqs.end(), [&sameLetterMatch](std::pair<char, int> freq){return (freq.second == sameLetterMatch);});
    }
}
