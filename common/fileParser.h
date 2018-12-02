#include <string>
#include <fstream>
#include <iterator>

namespace fileParse {
    template<typename T>
    void storeEachLine(const std::string& fileName, std::vector<T>& container) {
        std::ifstream infile(fileName);
        std::copy(std::istream_iterator<T>(infile),
                std::istream_iterator<T>(),
                std::back_inserter(container));
    }
}
