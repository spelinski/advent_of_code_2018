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
        Point destPoint;
        bool done;
        bool infinite;
        int closetUniqueCount;
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

std::map<int,int> getDistanceToAllPoints(const std::vector<destOption>& destOptions, Point currPoint){
    std::map<int,int> output;
    for(const auto& dest : destOptions){
        auto [startX,startY] = dest.destPoint;
        int dist = abs(startX - currPoint.first) + abs(startY - currPoint.second);
        output[dist] += 1;
    }
    return output;
}

void calcUniqueNonInfinite(std::vector<destOption>& destOptions){
    for(auto& destOption : destOptions){
        auto [startX,startY] = destOption.destPoint;
        int outerLoopCount = 0;
        for( int x = startX; x < (startX+500); ++x) {
            if(destOption.done){break;}
            if(outerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;};
            int myDist = abs(startX - x);
            std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,startY));
            if(allDist.begin()->first == myDist && allDist.begin()->second == 1){
                destOption.closetUniqueCount += 1;
	    } else {break;}

            int innerLoopCount = 0;
            for( int y = (startY+1); y < (startY+500); ++y) {
                if(destOption.done){break;}
                if(innerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;}
                int myDist = abs(startX - x) + abs(startY - y);
                std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,y));
                if(allDist.begin()->first == myDist && allDist.begin()->second == 1){
                    destOption.closetUniqueCount += 1;
                }
                else{break;}

                ++innerLoopCount;
            }
            innerLoopCount = 0;
            for( int y = (startY-1); y > (startY-500); --y) {
                if(destOption.done){break;}
                if(innerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;}
                int myDist = abs(startX - x) + abs(startY - y);
                std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,y));
                if(allDist.begin()->first == myDist && allDist.begin()->second == 1){destOption.closetUniqueCount += 1;}
                else{break;}

                ++innerLoopCount;
            }

            ++outerLoopCount;
        }

        outerLoopCount = 0;
        for( int x = (startX-1); x > (startX-500); --x) {
            if(destOption.done){break;}
            if(outerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;}
            int myDist = abs(startX - x);
            std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,startY));
            if(allDist.begin()->first != myDist || allDist.begin()->second > 1){break;};


            int innerLoopCount = 0;
            for( int y = startY; y < (startY+500); ++y) {
                if(destOption.done){break;}
                if(innerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;}
                int myDist = abs(startX - x) + abs(startY - y);
                std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,y));
                if(allDist.begin()->first == myDist && allDist.begin()->second == 1){destOption.closetUniqueCount += 1;}
                else{break;}

                ++innerLoopCount;
            }
            innerLoopCount = 0;
            for( int y = (startY-1); y > (startY-500); --y) {
                if(destOption.done){break;}
                if(innerLoopCount > 400){destOption.done=true; destOption.closetUniqueCount=0; break;}
                int myDist = abs(startX - x) + abs(startY - y);
                std::map<int,int> allDist = getDistanceToAllPoints(destOptions, Point(x,y));
                if(allDist.begin()->first == myDist && allDist.begin()->second == 1){destOption.closetUniqueCount += 1;}
                else{break;}

                ++innerLoopCount;
            }

            ++outerLoopCount;
        }

        destOption.done = true;
    }
}

int getHighest(std::vector<destOption> allOptions){
    int curMax = 0;
    for(const auto& dest : allOptions){
        if(dest.closetUniqueCount > curMax){curMax = dest.closetUniqueCount;}
    }
    return curMax;
}

int sumAllDist(const std::vector<destOption>& destOptions, Point currPoint){
    int distance = 0;
    for(const auto& dest : destOptions){
        auto [startX,startY] = dest.destPoint;
        distance += abs(startX - currPoint.first) + abs(startY - currPoint.second);
    }
    return distance;
}


int main(){
    std::vector<std::string> destinations = fileParse::storeEachLine("./challenges/challenge6/input.txt");
    std::vector<destOption> allOptions = getAllDestOptions(destinations);
    calcUniqueNonInfinite(allOptions);
    std::cout << "nonInfinteCount: " << getHighest(allOptions) <<  "\n";
/*    std::map<Point,int> region;
    for(auto& destOption : allOptions){
        auto [startX,startY] = destOption.destPoint;
        for( int x = startX; x < (startX+10000); ++x) {
            int myDist = sumAllDist(allOptions, Point(x,startY));
            if(myDist >= 10000){break;};

            for( int y = startY; y < (startY+10000); ++y) {
                int myDistance = sumAllDist(allOptions, Point(x,y));
                if(myDistance < 10000){
                    region[Point(x,y)] = 1;
                }
                else{break;}
            }
            for( int y = (startY-1); y > (startY-10000); --y) {
                int myDistance = sumAllDist(allOptions, Point(x,y));
                if(myDistance < 10000){
                    region[Point(x,y)] = 1;
                }
                else{break;}
            }
        }
        for( int x = (startX-1); x > (startX-10000); --x) {
            int myDist = sumAllDist(allOptions, Point(x,startY));
            if(myDist >= 10000){break;};

            for( int y = startY; y < (startY+10000); ++y) {
                int myDistance = sumAllDist(allOptions, Point(x,y));
                if(myDistance < 10000){
                    region[Point(x,y)] = 1;
                }
                else{break;}
            }
            for( int y = (startY-1); y > (startY-10000); --y) {
                int myDistance = sumAllDist(allOptions, Point(x,y));
                if(myDistance < 10000){
                    region[Point(x,y)] = 1;
                }
                else{break;}
            }
        }
    }
    int regionSize = 0;
    for(const auto& here : region){
        if(here.second == 1){
            ++regionSize;
        }
    }
    std::cout << "RegionsSize: " << regionSize << "\n";*/

    return 0;
}
