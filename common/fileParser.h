#include <string>
#include <fstream>
#include <iterator>
#include <vector>

namespace fileParse {
    template<typename T>
    std::vector<T> storeEachLine(const std::string& fileName) {
        std::vector<T> container;
        std::ifstream infile(fileName);
        std::copy(std::istream_iterator<T>(infile),
                std::istream_iterator<T>(),
                std::back_inserter(container));
        return container;
    }
}
