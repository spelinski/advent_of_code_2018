#include "fileParser.h"
#include <iostream>
#include <map>

typedef std::pair<int,int> Point;

enum direction{
    up, down, left, right
};

enum turn{
    turnLeft, straight, turnRight
};

struct track{
    track():exist(false),intersection(false),upper_left(false),upper_right(false),lower_left(false),lower_right(false),vertical(false),horizontal(false){}
    track(bool exists):exist(exists),intersection(false),upper_left(false),upper_right(false),lower_left(false),lower_right(false),vertical(false),horizontal(false){}
    bool exist;
    bool intersection;
    bool upper_left;
    bool upper_right;
    bool lower_left;
    bool lower_right;
    bool vertical;
    bool horizontal;
};

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

class cart{
    public:
        cart():real(false){}
        cart(direction initDir):real(true),currentDirection(initDir),nextIntersection(turnLeft){}
        void hitIntersection(){
            if(nextIntersection == turnLeft){
                if(currentDirection == down){
                    currentDirection = right;
                } else if(currentDirection == right){
                    currentDirection = up;
                } else if(currentDirection == up){
                    currentDirection = left;
                } else if(currentDirection == left){
                    currentDirection = down;
                }
                nextIntersection = straight;
            } else if(nextIntersection == turnRight){
                if(currentDirection == down){
                    currentDirection = left;
                } else if(currentDirection == right){
                    currentDirection = down;
                } else if(currentDirection == up){
                    currentDirection = right;
                } else if(currentDirection == left){
                    currentDirection = up;
                }
                nextIntersection = turnLeft;
            } else {
                nextIntersection = turnRight;
            }
        };
        void changeDirection(direction newDirection){
            currentDirection = newDirection;
        };
        void resetReal(){real = false;}
        direction getCurrentDirection() const {return currentDirection;};
        bool isReal() const{return real;}
    private:
        bool real;
        direction currentDirection;
        turn nextIntersection;
};

class trackSystem{
    public:
        trackSystem():collision(false){}
        void performTick(){
            std::map<Point, cart, checkPointOrdering> newCartMap = cartMap;
            for(const auto& currCart : cartMap){
                cart newCart = currCart.second;
                if(newCart.isReal()){
                    Point newLocation;
                    track newLocTrack;
                    direction curDir = newCart.getCurrentDirection();
                    if(curDir == up){
                        newLocation = Point(currCart.first.first,currCart.first.second-1);
                        newLocTrack = trackMap[newLocation];
                        if(newLocTrack.upper_right){
                            newCart.changeDirection(left);
                        } else if(newLocTrack.upper_left){
                            newCart.changeDirection(right);
                        }
                    } else if(curDir == left){
                        newLocation = Point(currCart.first.first-1,currCart.first.second);
                        newLocTrack = trackMap[newLocation];
                        if(newLocTrack.upper_left){
                            newCart.changeDirection(down);
                        } else if(newLocTrack.lower_left){
                            newCart.changeDirection(up);
                        }
                    } else if(curDir == right){
                        newLocation = Point(currCart.first.first+1,currCart.first.second);
                        newLocTrack = trackMap[newLocation];
                        if(newLocTrack.upper_right){
                            newCart.changeDirection(down);
                        } else if(newLocTrack.lower_right){
                            newCart.changeDirection(up);
                        }
                    } else if(curDir == down){
                        newLocation = Point(currCart.first.first,currCart.first.second+1);
                        newLocTrack = trackMap[newLocation];
                        if(newLocTrack.lower_left){
                            newCart.changeDirection(right);
                        } else if(newLocTrack.lower_right){
                            newCart.changeDirection(left);
                        }
                    }
                    if(newLocTrack.intersection){
                        newCart.hitIntersection();
                    }
                    if(newCartMap[newLocation].isReal()){
                        collision = true;
                        collisionPoint = newLocation;
                        newCartMap[currCart.first] = cart();
                        newCartMap[newLocation] = cart();
                    } else if(newCartMap[currCart.first].isReal()){
                        newCartMap[currCart.first] = cart();
                        newCartMap[newLocation] = newCart;
                    }
                }
            }
            cartMap = newCartMap;
        }
        std::map<Point, track, checkPointOrdering> trackMap;
        std::map<Point, cart, checkPointOrdering> cartMap;
        bool collision;
        Point collisionPoint;
};

trackSystem parseSystem(std::vector<std::string> allTracks){
    trackSystem output;
    int x,y = 0;
    for(const auto& line: allTracks) {
        x=0;
        for(size_t position = 0; position < line.size(); ++position) {
            char piece = line[position];
            track currentTrack(true);
            if(piece == '/'){
                if((position == 0) || ((line[position-1] != '-') && (line[position-1] != '>') && (line[position-1] != '<') && (line[position-1] != '+'))){
                    currentTrack.upper_left = true;
                } else {
                    currentTrack.lower_right = true;
                }
            } else if(piece == '\\'){
                if((position == 0) || ((line[position-1] != '-') && (line[position-1] != '>') && (line[position-1] != '<') && (line[position-1] != '+'))){
                    currentTrack.lower_left = true;
                } else {
                    currentTrack.upper_right = true;
                }
            } else if(piece == '+') {
                currentTrack.intersection = true;
            } else if((piece == '-') || (piece == '>') || (piece == '<')){
                currentTrack.horizontal = true;
                if(piece == '>'){
                    output.cartMap[Point(x,y)] = cart(right);
                } else if(piece == '<') {
                    output.cartMap[Point(x,y)] = cart(left);
                }
            } else if((piece == '|') || (piece == '^') || (piece == 'v')){
                currentTrack.vertical = true;
                if(piece == '^'){
                    output.cartMap[Point(x,y)] = cart(up);
                } else if (piece == 'v'){
                    output.cartMap[Point(x,y)] = cart(down);
                }
            } else {
                currentTrack.exist = false;
            }
            output.trackMap[Point(x,y)] = currentTrack;
            ++x;
        }
        ++y;
    }
    return output;
}

void printTracks(trackSystem& printingTrack, std::ostream& outputStream){
    int lastY = 0;
    for(const auto& trackPiece : printingTrack.trackMap){
        if(trackPiece.first.second != lastY){
            outputStream << "\n";
            lastY = trackPiece.first.second;
        }
        if(printingTrack.cartMap[trackPiece.first].isReal()){
            direction curDir = printingTrack.cartMap[trackPiece.first].getCurrentDirection();
            if(curDir == up){
                outputStream << "^";
            } else if(curDir == down){
                outputStream << "v";
            } else if(curDir == right){
                outputStream << ">";
            } else if(curDir == left){
                outputStream << "<";
            }
        } else if(trackPiece.second.upper_left || trackPiece.second.lower_right){
            outputStream << "/";
        } else if(trackPiece.second.upper_right || trackPiece.second.lower_left) {
            outputStream << "\\";
        } else if(trackPiece.second.intersection){
            outputStream << "+";
        } else if(trackPiece.second.horizontal){
            outputStream << "-";
        } else if(trackPiece.second.vertical){
            outputStream << "|";
        } else {
            outputStream << " ";
        }
    }
    outputStream << "\n";
}

int checkCartsLeft(const std::map<Point,cart,checkPointOrdering>& currentCarts){
    int count = 0;
    for(const auto& cart : currentCarts){
        if(cart.second.isReal()){
            ++count;
        }
    }
    return count;
}

int main(){
    std::vector<std::string> allTrackLines = fileParse::storeEachLine("./input.txt");
    trackSystem mySystem = parseSystem(allTrackLines);
    std::ofstream file("output/output1.txt");
    printTracks(mySystem, file);
    file.close();
    int fileCounter = 2;
    while(!mySystem.collision){
        mySystem.performTick();
        std::ofstream fileLoop("output/output"+std::to_string(fileCounter)+".txt");
        printTracks(mySystem, fileLoop);
        fileLoop.close();
        ++fileCounter;
    }
    std::cout << "collisionPoint: " << mySystem.collisionPoint.first << "," << mySystem.collisionPoint.second << "\n";
    while(checkCartsLeft(mySystem.cartMap) > 1){
        mySystem.performTick();
        std::ofstream fileLoop("output/output"+std::to_string(fileCounter)+".txt");
        printTracks(mySystem, fileLoop);
        fileLoop.close();
        ++fileCounter;
    }
    for(const auto& cart : mySystem.cartMap){
        if(cart.second.isReal()){
            std::cout << "cartLeft: " << cart.first.first << "," << cart.first.second << "\n";
        }
    }
    return 0;
}
