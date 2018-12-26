#include <algorithm>
#include <iterator>
#include <map>
namespace grid{

    typedef std::pair<int,int> Point;

    struct checkPointOrdering {
        bool operator()(const Point& p1, const Point& p2){
            if(p1.second < p2.second){
                return true;
            }
            if(p2.second < p1.second){
                return false;
            }
            if(p1.first < p2.first){
                return true;
            }
            return false;
        }
    };

    template <typename T>
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
            int getSize(){
                return gridMap.size();
            }
            typename std::map<Point, T>::iterator begin(){
                return gridMap.begin();
            }
            typename std::map<Point, T>::iterator end(){
                return gridMap.end();
            }
            typename std::map<Point, T>::const_iterator begin() const{
                return gridMap.begin();
            }
            typename std::map<Point, T>::const_iterator end() const{
                return gridMap.end();
            }
        private:
            std::map<Point, T, checkPointOrdering> gridMap;
    };
}
