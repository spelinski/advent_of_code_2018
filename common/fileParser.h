#include <string>
#include <fstream>
#include <iterator>
#include <vector>

namespace fileParse {
    template<typename T>
    std::vector<T> storeEachWord(const std::string& fileName) {
        std::vector<T> container;
        std::ifstream infile(fileName);
        std::copy(std::istream_iterator<T>(infile),
                std::istream_iterator<T>(),
                std::back_inserter(container));
        return container;
    }

    std::vector<std::string> storeEachLine(const std::string& fileName) {
        std::vector<std::string> container;
        std::string currentLine;
        std::ifstream infile(fileName);
        while (std::getline(infile, currentLine)) {
            container.push_back(currentLine);
        }
        return container;
    }
}
