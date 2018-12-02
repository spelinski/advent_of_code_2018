#include "fileParser.h"
#include "stringParser.h"
#include <algorithm>
#include <iostream>
#include <vector>

bool hasNumberOfSameLetter(const std::string& inputString, const int sameLetterMatch) {
    std::map<char, int> letterFreqs= stringParse::getLetterFrequencies(inputString);
    return std::any_of(letterFreqs.begin(), letterFreqs.end(), [&sameLetterMatch](std::pair<char, int> freq){return (freq.second == sameLetterMatch);});
}

int numWordsWithNumSameLetters(std::vector<std::string> ids, int dupTarget) {
    return std::count_if(ids.begin(), ids.end(),
            [&dupTarget](std::string boxId) { 
                return hasNumberOfSameLetter(boxId, dupTarget); 
            });
}

std::string getSamePartOfStringsOffByOne(std::vector<std::string> ids) {
    for(auto outerIt = ids.begin(); outerIt != ids.end(); ++outerIt) {
        for(auto innerIt = outerIt+1; innerIt != ids.end(); ++innerIt) {
            int differences = 0;
            auto innerCharIt = innerIt->begin();
            auto outerCharSameIt = outerIt->end();
            for(auto outerCharIt = outerIt->begin(); outerCharIt != outerIt->end(); ++outerCharIt) {
                if(*outerCharIt != *innerCharIt) {
                    ++differences;
                    outerCharSameIt = outerCharIt;
                }
                if(differences > 1) {
                    break;
                }
                ++innerCharIt;
            }
            if(differences == 1) {
                outerIt->erase(outerCharSameIt);
                return *outerIt;
            }
        }
    }
    return "";
}

int main() {
    std::vector<std::string> allIds = fileParse::storeEachLine<std::string>("./challenges/challenge2/input.txt");
    int doubleCount = numWordsWithNumSameLetters(allIds, 2);
    int tripleCount = numWordsWithNumSameLetters(allIds, 3);
    int captcha = doubleCount * tripleCount;
    std::cout << "captcha: " << captcha << "\n";

    std::string sameString = getSamePartOfStringsOffByOne(allIds);
    std::cout << "same part: " << sameString << "\n";

    return 0;
}

