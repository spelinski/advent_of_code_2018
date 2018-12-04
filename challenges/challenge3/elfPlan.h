#include <utility>

namespace elfPlan {

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

    class elfPlan{
        public:
            elfPlan(planId id, initPoint sp, xDist xDistance, yDist yDistance):id(id.id),startPoint(sp.point),xDistance(xDistance.distance),yDistance(yDistance.distance) {}
            std::pair<int,int> getStartPoint() const {return startPoint;}
            int getId() const {return id;}
            int getXDistance() const {return xDistance;}
            int getYDistance() const {return yDistance;}
        private:
            int id;
            std::pair<int,int> startPoint;
            int xDistance;
            int yDistance;
    };
}
