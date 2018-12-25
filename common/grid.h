#include <algorithm>
#include <map>
namespace grid{
    typedef std::pair<int,int> Point;
    template<typename T>
    class grid {
        public:
            grid(){};
            void setItem(Point insertionPoint, T item){
                gridMap[insertionPoint] = item;
            }
            T getItem(Point retrievalPoint){
                return gridMap[retrievalPoint];
            }
            template<typename FT>
            int performCountWithConditional(FT conditional){
                return std::count_if(gridMap.begin(), gridMap.end(), conditional);
            }
        private:
            std::map<Point, T> gridMap;
    };
}
