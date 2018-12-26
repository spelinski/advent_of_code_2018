#include "grid.h"
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace elfPlan {

    typedef std::pair<int,int> Point;

    struct planId {
        explicit planId(int id):id(id){}
        int id;
    };

    struct initPoint {
        explicit initPoint(std::pair<int,int> sp):point(sp){}
        std::pair<int,int> point;
    };

    struct xDist {
        explicit xDist(int distance):distance(distance){}
        int distance;
    };

    struct yDist {
        explicit yDist(int distance):distance(distance){}
        int distance;
    };

    struct elfPlan{
        public:
            elfPlan(planId id, initPoint sp, xDist xDistance, yDist yDistance):id(id.id),startPoint(sp.point),xDistance(xDistance.distance),yDistance(yDistance.distance) {}
            template<typename T>
            void performOperationWithEachPoint(T oper){
                auto [startX,startY] = startPoint;
                for(int x = startX; x < (startX+xDistance); ++x){
                    for(int y = startY; y < (startY+yDistance); ++y){
                        oper(Point(x,y));
                    }
                }
            }
            int getId(){return id;}
        private:
            int id;
            Point startPoint;
            int xDistance;
            int yDistance;
    };

    class elfPlanHandler{
        public:
            elfPlanHandler(){}
            void insert(elfPlan newPlan){
                auto incrementUsages = [&](Point curPoint){usageGrid.setItem(curPoint, (usageGrid.getItem(curPoint)+1));};
                newPlan.performOperationWithEachPoint(incrementUsages);
                allPlans.push_back(newPlan);
            }

            int getUniquePlan() {
                for(auto& plan : allPlans) {
                    bool unique = true;
                    auto checkUnique = [&](Point curPoint){if(usageGrid.getItem(curPoint) > 1) {unique = false;}};
                    plan.performOperationWithEachPoint(checkUnique);
                    if(unique) {
                        return plan.getId();
                    }
                }
                return 0;
            }

            int getInchesDuplicated(){
                return std::count_if(usageGrid.begin(), usageGrid.end(), [](auto inch){return (inch.second > 1);});
            }

        private:
            std::vector<elfPlan> allPlans;
            grid::grid<int> usageGrid;
    };

}
