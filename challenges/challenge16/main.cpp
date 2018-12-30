#include "elfAssembly.h"
#include "fileParser.h"
#include <array>
#include <iostream>
#include <regex>

struct sample{
    std::array<int,4> beforeRegs;
    int opCode;
    int opA;
    int opB;
    int opC;
    std::array<int,4> afterRegs;
};

struct result{
    std::array<int,4> regs;
};

bool operator==(const sample& samp, const result& res){
    return (samp.afterRegs == res.regs);
}

int getNumberOfDuplicates(sample currentSample){
    int numDups = 0;
    std::vector<std::array<int,4> (*)(std::array<int,4>,int,int,int)> allElfAssemblyFunctions;
    allElfAssemblyFunctions = elfAssembly::getAllElfAssemblyFunctions<std::array<int,4> (*)(std::array<int,4>,int,int,int)>();
    for(const auto& assemblyInstruction : allElfAssemblyFunctions){
        if(assemblyInstruction(currentSample.beforeRegs, currentSample.opA, currentSample.opB, currentSample.opC) == currentSample.afterRegs){
            ++numDups;
        }
    }
    return numDups;
}

int getNumberOfSamplesWithGreaterOrEqualNumberOfDuplicates(std::vector<sample> allSamples, int num){
    int numSamples = 0;
    for(const auto& currentSample : allSamples){
        if(getNumberOfDuplicates(currentSample) >= num){
            ++numSamples;
        }
    }
    return numSamples;
}

std::vector<sample> parseOutSamples(std::vector<std::string> lines){
    std::vector<sample> outSamples;
    for(size_t i=0; (i+2) < lines.size(); i+=4){
        sample tempSample;
        std::regex beforeReg("Before\\:\\s+\\[([0-9]+),\\s+([0-9]+),\\s+([0-9]+),\\s+([0-9]+)");
        std::smatch myMatch;
        std::regex_search(lines[i], myMatch, beforeReg);
        tempSample.beforeRegs[0] = std::stoi(myMatch[1]);
        tempSample.beforeRegs[1] = std::stoi(myMatch[2]);
        tempSample.beforeRegs[2] = std::stoi(myMatch[3]);
        tempSample.beforeRegs[3] = std::stoi(myMatch[4]);
        std::regex opReg ("([0-9]+)\\s+([0-9]+)\\s+([0-9]+)\\s+([0-9]+)");
        std::regex_search(lines[i+1], myMatch, opReg);
        tempSample.opCode = std::stoi(myMatch[1]);
        tempSample.opA = std::stoi(myMatch[2]);
        tempSample.opB = std::stoi(myMatch[3]);
        tempSample.opC = std::stoi(myMatch[4]);
        std::regex afterReg("After\\:\\s+\\[([0-9]+),\\s+([0-9]+),\\s+([0-9]+),\\s+([0-9]+)");
        std::regex_search(lines[i+2], myMatch, afterReg);
        tempSample.afterRegs[0] = std::stoi(myMatch[1]);
        tempSample.afterRegs[1] = std::stoi(myMatch[2]);
        tempSample.afterRegs[2] = std::stoi(myMatch[3]);
        tempSample.afterRegs[3] = std::stoi(myMatch[4]);
        outSamples.push_back(tempSample);
    }
    return outSamples;
}

int main(){
    std::vector<std::string> lines = fileParse::storeEachLine("./challenges/challenge16/input.txt");
    std::vector<sample> allSamples = parseOutSamples(lines);
    std::cout << allSamples.size() << "\n";
    int numDups = getNumberOfSamplesWithGreaterOrEqualNumberOfDuplicates(allSamples, 3);
    std::cout << "num Dups: " << numDups << "\n";

    return 0;
}
