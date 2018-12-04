#include "fileParser.h"
#include <algorithm>
#include <iostream>
#include <regex>

class wallEntry{
public:
    wallEntry(int year, int month, int day, int hour, int minute, std::string message):
        year(year),day(day),month(month),hour(hour),minute(minute),message(message){}

    bool operator < (const wallEntry& c2) {
        if(year < c2.year){return true;}
        if(year > c2.year){return false;}
        if(month < c2.month){return true;}
        if(month > c2.month){return false;}
        if(day < c2.day){return true;}
        if(day > c2.day){return false;}
        if(hour < c2.hour){return true;}
        if(hour > c2.hour){return false;}
        if(minute < c2.minute){return true;}
        return false;
    }

    int getYear() const {return year;}
    int getDay() const {return day;}
    int getMonth() const {return month;}
    int getHour() const {return hour;}
    int getMinute() const {return minute;}
    std::string getMessage() const {return message;}

private:
    int year;
    int day;
    int month;
    int hour;
    int minute;
    std::string message;
};

std::vector<wallEntry> getAllWallEntries(const std::vector<std::string>& wallStrings) {
    std::vector<wallEntry> container;
    std::regex wallReg ("\\[([0-9]+)-([0-9]+)-([0-9]+)\\s+([0-9]+):([0-9]+)\\]\\s+(.*)");
    for( const auto& msg : wallStrings ) {
        std::smatch myMatch;
        std::regex_search(msg,myMatch,wallReg);
        int year = std::stoi(myMatch[1]);
        int month = std::stoi(myMatch[2]);
        int day = std::stoi(myMatch[3]);
        int hour = std::stoi(myMatch[4]);
        int minute = std::stoi(myMatch[5]);
        std::string message = myMatch[6];
        container.push_back(wallEntry(year,month,day,hour,minute,message));
    }
    return container;
}

std::map<int, std::map<int,int> > getGuardsSleep(const std::vector<wallEntry>& entries) {
    std::map<int, std::map<int,int> > outputMap;
    std::smatch myMatch;
    std::regex guardIdReg(".*Guard\\s+#([0-9]+).*");
    std::regex sleepReg(".*asleep");
    std::regex wakeReg(".*wakes");
    int curGuardId = 0;
    int sleepStart = 0;
    for(const auto& entry : entries) {
        std::string message = entry.getMessage();
        if(std::regex_search(message,myMatch,guardIdReg)){
            curGuardId = std::stoi(myMatch[1]);
        } else if(std::regex_search(message,myMatch,sleepReg)) {
            sleepStart = entry.getMinute();
        } else if(std::regex_search(message,myMatch,wakeReg)) {
            for(;sleepStart < entry.getMinute();++sleepStart){
                outputMap[curGuardId][sleepStart] += 1;
            }
        }

    }
    return outputMap;
}

int getGuardThatSleepsMost(const std::map<int, std::map<int,int> >& guardSleep){
    int maxGuard = 0;
    int maxGuardTimeTotal = 0;
    for(const auto& guard : guardSleep) {
        int currentGuard = guard.first;
        int currentGuardTime = 0;
        for (const auto& minute : guard.second) {
            currentGuardTime += minute.second;
        }
        if(currentGuardTime > maxGuardTimeTotal) {
            maxGuard = currentGuard;
            maxGuardTimeTotal = currentGuardTime;
        }
    }
    return maxGuard;
}

int getGuardMinute(const std::map<int,int>& specificGuard) {
    int maxMinute = 0;
    int maxTimes = 0;
    for (const auto& minute : specificGuard) {
        if(minute.second > maxTimes){
            maxMinute = minute.first;
            maxTimes = minute.second;
        }
    }
    return maxMinute;
}

int getGuardTimesSleepiestMinute(const std::map<int, std::map<int,int> >& guardSleep){
    int maxGuard = 0;
    int maxMinute = 0;
    int maxTimes = 0;
    for(const auto& guard : guardSleep) {
        for(const auto& minute : guard.second) {
            if(minute.second > maxTimes){
                maxGuard = guard.first;
                maxMinute = minute.first;
                maxTimes = minute.second;
            }
        }
    }
    return maxGuard * maxMinute;
}

int main() {
    std::vector<std::string> wallStrings = fileParse::storeEachLine("./challenges/challenge4/input.txt");
    std::vector<wallEntry> allEntries = getAllWallEntries(wallStrings);
    std::sort(allEntries.begin(), allEntries.end());
    std::map<int, std::map<int,int> > guardToSleepMap = getGuardsSleep(allEntries);
    int sleepiestGuard = getGuardThatSleepsMost(guardToSleepMap);
    int guardSleepyMinute = getGuardMinute(guardToSleepMap[sleepiestGuard]);
    std::cout << "Guards times minutes: " << (sleepiestGuard*guardSleepyMinute) << "\n";
    std::cout << "Guards times sleepiest overall minute: " << getGuardTimesSleepiestMinute(guardToSleepMap) << "\n";

    return 0;
}
