#include<array>
#include<vector>

namespace elfAssembly{
    std::array<int,4> addr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] + currentRegs[B];
        return outputRegs;
    }

    std::array<int,4> addi(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] + B;
        return outputRegs;
    }

    std::array<int,4> mulr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] * currentRegs[B];
        return outputRegs;
    }

    std::array<int,4> muli(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] * B;
        return outputRegs;
    }

    std::array<int,4> banr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] & currentRegs[B];
        return outputRegs;
    }

    std::array<int,4> bani(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] & B;
        return outputRegs;
    }

    std::array<int,4> borr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] | currentRegs[B];
        return outputRegs;
    }

    std::array<int,4> bori(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A] | B;
        return outputRegs;
    }

    std::array<int,4> setr(std::array<int,4> currentRegs, int A, int, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = currentRegs[A];
        return outputRegs;
    }

    std::array<int,4> seti(std::array<int,4> currentRegs, int A, int , int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = A;
        return outputRegs;
    }

    std::array<int,4> gtir(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (A > currentRegs[B]) ? 1 : 0;
        return outputRegs;
    }

    std::array<int,4> gtri(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (currentRegs[A] > B) ? 1 : 0;
        return outputRegs;
    }

    std::array<int,4> gtrr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (currentRegs[A] > currentRegs[B]) ? 1 : 0;
        return outputRegs;
    }

    std::array<int,4> eqir(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (A == currentRegs[B]) ? 1 : 0;
        return outputRegs;
    }

    std::array<int,4> eqri(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (currentRegs[A] == B) ? 1 : 0;
        return outputRegs;
    }

    std::array<int,4> eqrr(std::array<int,4> currentRegs, int A, int B, int C){
        std::array<int,4> outputRegs = currentRegs;
        outputRegs[C] = (currentRegs[A] == currentRegs[B]) ? 1 : 0;
        return outputRegs;
    }
    template<typename Function>
    std::vector<Function> getAllElfAssemblyFunctions(){
        std::vector<Function> allFunctions = {addr,addi,mulr,muli,banr,bani,borr,bori,setr,seti,gtir,gtri,gtrr,eqir,eqri,eqrr};
        return allFunctions;
    }
}

