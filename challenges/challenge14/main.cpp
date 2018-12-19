#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class scoreBoard{
    public:
        explicit scoreBoard():recipes({3,7}),elfOnePos(0),elfTwoPos(1){}
        void createNewRecipes(){
            elfOnePos += recipes.at(elfOnePos) + 1;
            elfOnePos %= recipes.size();
            elfTwoPos += recipes.at(elfTwoPos) + 1;
            elfTwoPos %= recipes.size();
            int newRecipeScore = recipes.at(elfOnePos) + recipes.at(elfTwoPos);
            std::string newScoreString = std::to_string(newRecipeScore);
            for(const char& number : newScoreString){

                recipes.push_back(number-48);
            }
        }
        void printScoreRange(int startRecipeNumber, int endRecipeNumber){
            for(;startRecipeNumber < endRecipeNumber; ++startRecipeNumber){
                std::cout << recipes.at(startRecipeNumber);
            }
            std::cout << "\n";
        }
        void createTotalRecipes(size_t newSize){
            while(recipes.size() < newSize){
                createNewRecipes();
            }
        }
        int getIndexOfFirstDigit(std::string numberString){
            int maxFittingIndex = recipes.size() - numberString.size();
            for(int firstDigitIndex = 0; firstDigitIndex < maxFittingIndex; ++firstDigitIndex){
                std::string currentNumberString = "";
                int endOfRange = firstDigitIndex + numberString.size();
                for(int currentIndex = firstDigitIndex; currentIndex < endOfRange; ++currentIndex){
                    currentNumberString += std::to_string(recipes.at(currentIndex));
                }
                if(numberString == currentNumberString){
                    return firstDigitIndex;
                }
            }
            return -1;
        }
    private:
        std::vector<int> recipes;
        int elfOnePos;
        int elfTwoPos;
};

int main(){
    int numberOfRecipesAfter = 10;
    int numberOfRecipesToCreateBefore = 702831;
    std::string numberString = "702831";
    int totalRecipesNeeded = numberOfRecipesToCreateBefore + numberOfRecipesAfter;
    scoreBoard elfScores;
    elfScores.createTotalRecipes(totalRecipesNeeded);
    elfScores.printScoreRange(numberOfRecipesToCreateBefore, (totalRecipesNeeded));

    int numRecipesBefore = elfScores.getIndexOfFirstDigit(numberString);
    while(numRecipesBefore == -1){
        totalRecipesNeeded += 10000000;
        elfScores.createTotalRecipes(totalRecipesNeeded);
        numRecipesBefore = elfScores.getIndexOfFirstDigit(numberString);
        std::cout << "advancing\n";
    }

    std::cout << "number of recipes before match: " << numRecipesBefore << "\n";
}
