//
// Created by dragon5hoarder on 12/14/2015.
//

#include "DefaultAgentStrategy.h"

namespace Gaming{

    DefaultAgentStrategy::DefaultAgentStrategy(){

    }

    DefaultAgentStrategy::~DefaultAgentStrategy(){

    }

    ActionType DefaultAgentStrategy::operator()(const Surroundings &s) const{
        std::vector<int> advantagePositions;
        std::vector<int> foodPositions;
        std::vector<int> emptyPositions;
        std::vector<int> simplePositions;
        int emptySpace = 4;
        bool moveToEmpty = false;
        bool nabbed = false;
        for (int i = 0; i < 9; i++){
            if (s.array[i] == ADVANTAGE) {
                //emptySpace = i;
                //break;
                advantagePositions.push_back(i);
            }
            else if (s.array[i] == FOOD) {
                //emptySpace = i;
                //nabbed = true;
                foodPositions.push_back(i);
            }
            else if (s.array[i] == EMPTY) {
                //emptySpace = i;
                //moveToEmpty = true;
                emptyPositions.push_back(i);
            }
            else if (s.array[i] == SIMPLE && !moveToEmpty) {
                //emptySpace = i;
                simplePositions.push_back(i);
            }

        }
        unsigned int randNum;
        if (advantagePositions.size() > 0) {
            randNum = rand() % advantagePositions.size();
            emptySpace = advantagePositions[randNum];
        }
        else if (foodPositions.size() > 0) {
            randNum = rand() % foodPositions.size();
            emptySpace = foodPositions[randNum];
        }
        else if (emptyPositions.size() > 0) {
            randNum = rand() % emptyPositions.size();
            emptySpace = emptyPositions[randNum];
        }
        else if (simplePositions.size() > 0) {
            randNum = rand() % simplePositions.size();
            emptySpace = simplePositions[randNum];
        }
        switch (emptySpace) {
            case 0:
                emptySpace = 2;
                break;
            case 1:
                emptySpace = 0;
                break;
            case 2:
                emptySpace = 1;
                break;
            case 3:
                emptySpace = 4;
                break;
            case 4:
                emptySpace = 8;
                break;
            case 5:
                emptySpace = 3;
                break;
            case 8:
                emptySpace = 5;
                break;
        }
        return static_cast<ActionType >(emptySpace);
    }
}
