#include "fileParser.h"
#include <cmath>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

typedef std::pair<int,int> Point;

struct destOption {
    public:
        explicit destOption(Point destination):destPoint(destination),done(false),closetUniqueCount(0){}
        bool operator <(const destOption& rhs){
            return closetUniqueCount < rhs.closetUniqueCount;
        }
        Point destPoint;
        bool done;
        bool infinite;
        int closetUniqueCount;
};

std::map<int,int> getDistanceToAllPoints(const std::vector<destOption>& destOptions, Point currPoint){
    std::map<int,int> output;
    for(const auto& dest : destOptions){
        auto [startX,startY] = dest.destPoint;
        int dist = abs(startX - currPoint.first) + abs(startY - currPoint.second);
        output[dist] += 1;
    }
    return output;
}

int sumAllDist(const std::vector<destOption>& destOptions, Point currPoint){
    int distance = 0;
    for(const auto& dest : destOptions){
        auto [startX,startY] = dest.destPoint;
        distance += abs(startX - currPoint.first) + abs(startY - currPoint.second);
    }
    return distance;
}

struct destinationOptions {
    public:
        explicit destinationOptions(std::vector<destOption> inOptions):allOptions(inOptions),maxX(0),maxY(0){}

        template<typename T>
        void performOperationOnAllPoints(int maxDistance, T action){
            for(auto& destOption : allOptions){
                auto [startX,startY] = destOption.destPoint;
                for( int x = startX; x < (startX+maxDistance); ++x) {
                    if(!action(destOption, Point(x,startY))){
                        break;
                    }
                    for( int y = (startY+1); y < (startY+maxDistance); ++y) {
                        if(!action(destOption, Point(x,y))){
                            break;
                        }
                    }
                    for( int y = (startY-1); y > (startY-maxDistance); --y) {
                        if(!action(destOption, Point(x,y))){
                            break;
                        }
                    }
                }
                for( int x = (startX-1); x > (startX-maxDistance); --x) {
                    if(!action(destOption, Point(x,startY))){
                        break;
                    }

                    for( int y = (startY+1); y < (startY+maxDistance); ++y) {
                        if(!action(destOption, Point(x,y))){
                            break;
                        }
                    }
                    for( int y = (startY-1); y > (startY-maxDistance); --y) {
                        if(!action(destOption, Point(x,y))){
                            break;
                        }
                    }
                }
                destOption.done = true;
            }
        }
        void calcUniqueNonInfinite(){
            int maxDistance = 500;
            calcMaxXAndY();
            performOperationOnAllPoints(maxDistance, [&](destOption& currOption, Point currPoint){
                    if(currOption.done){return false;}
                    int myDist = abs(currOption.destPoint.first - currPoint.first) + abs(currOption.destPoint.second - currPoint.second);
                    std::map<int,int> allDist = getDistanceToAllPoints(allOptions, currPoint);
                    if(allDist.begin()->first == myDist && allDist.begin()->second == 1){
                        if(currPoint.first <= 0 || currPoint.second <= 0 || currPoint.first >= maxX || currPoint.second >= maxY) {
                            currOption.done=true;
                            currOption.closetUniqueCount = 0;
                            return false;
                        }
                        currOption.closetUniqueCount += 1;
                        return true;
                    }
                    return false;
                });
        }

        int getHighest(){
            auto pr = std::max_element(allOptions.begin(), allOptions.end());
            return pr->closetUniqueCount;
        }

        int getRegionSizeAroundAllPoints(){
            int maxRegionDistance = 10000;
            std::map<Point,int> region;
            performOperationOnAllPoints(maxRegionDistance, [&](destOption& destOption, Point currPoint){
                        (void)destOption;
                        int myDist = sumAllDist(allOptions, currPoint);
                        if(myDist < maxRegionDistance){
                            region[currPoint] = 1;
                            return true;
                        } else {
                            return false;
                        };
                    });
            return region.size();
        }
    private:
        std::vector<destOption> allOptions;
        int maxX;
        int maxY;
        void calcMaxXAndY(){
            for(const auto& dest : allOptions){
                if(dest.destPoint.first > maxX){maxX = dest.destPoint.first;}
                if(dest.destPoint.second > maxY){maxY = dest.destPoint.second;}
            }
        }
};


std::vector<destOption> getAllDestOptions(const std::vector<std::string>& destinations){
    std::vector<destOption> output;
    std::regex destReg ("([0-9]+),\\s+([0-9]+)");
    for(const auto& dest : destinations){
        std::smatch myMatch;
        std::regex_search(dest,myMatch,destReg);
        output.push_back(destOption(std::pair<int,int>(std::stoi(myMatch[1]),std::stoi(myMatch[2]))));
    }
    return output;
}

int main(){
    std::vector<std::string> destinations = fileParse::storeEachLine("./challenges/challenge6/input.txt");
    destinationOptions optionContainer(getAllDestOptions(destinations));
    optionContainer.calcUniqueNonInfinite();
    std::cout << "nonInfinteCount: " << optionContainer.getHighest() <<  "\n";
    std::cout << "Testing: " << optionContainer.getRegionSizeAroundAllPoints() << "\n";

    return 0;
}
