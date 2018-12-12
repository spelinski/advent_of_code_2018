#include "fileParser.h"
#include <algorithm>
#include <iostream>
#include <numeric>

struct node{
    explicit node():numChildren(0),numMetaData(0){}
    int numChildren;
    int numMetaData;
    std::vector<int> metaData;
    std::vector<node> children;
};

node getHeadNode(std::vector<int>& input){
    node Head;
    Head.numChildren = input.back();
    input.pop_back();
    Head.numMetaData = input.back();
    input.pop_back();
    return Head;
}

void getChildrenNodes(std::vector<int>& input, std::vector<node>& childrenContainer){
    node currentNode;
    currentNode.numChildren = input.back();
    input.pop_back();
    currentNode.numMetaData = input.back();
    input.pop_back();
    if(currentNode.numChildren == 0){
        for(int i = 0; i < currentNode.numMetaData; ++i){
            currentNode.metaData.push_back(input.back());
            input.pop_back();
        }
        childrenContainer.push_back(currentNode);
        return;
    }
    for(int i = 0; i < currentNode.numChildren; ++i){
        getChildrenNodes(input, currentNode.children);
    };
    for(int i = 0; i < currentNode.numMetaData; ++i){
        currentNode.metaData.push_back(input.back());
        input.pop_back();
    }
    childrenContainer.push_back(currentNode);
}


node parseForNodes(std::vector<int>& input){
    node Head = getHeadNode(input);
    for(int i = 0; i < Head.numChildren; ++i){
        getChildrenNodes(input, Head.children);
    }
    for(int i = 0; i < Head.numMetaData; ++i){
        Head.metaData.push_back(input.back());
        input.pop_back();
    }
    return Head;
}

int getSumOfMetaEntries(node currNode){
    int sum = std::accumulate(currNode.metaData.begin(), currNode.metaData.end(),0);
    if(currNode.numChildren == 0){
        return sum;
    }
    for(const auto& child : currNode.children){
        sum += getSumOfMetaEntries(child);
    }
    return sum;
}

int getNodeValue(node currNode){
    int value = 0;
    if(currNode.numChildren == 0){
        return getSumOfMetaEntries(currNode);
    }
    for(const auto& metaEntry : currNode.metaData){
        if(metaEntry <= currNode.numChildren){
            value += getNodeValue(currNode.children[metaEntry-1]);
        }
    }
    return value;
}

int main(){
    std::vector<int> input =  fileParse::storeEachWord<int>("./challenges/challenge8/input.txt");
    std::reverse(input.begin(),input.end());
    node Head = parseForNodes(input);
    std::cout << "Sum of meta entries: " << getSumOfMetaEntries(Head) << "\n";
    std::cout << "Node value: " << getNodeValue(Head) << "\n";
    return 0;
}
