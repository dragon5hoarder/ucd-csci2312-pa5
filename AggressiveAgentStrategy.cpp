//
// Created by dragon5hoarder on 12/14/2015.
//

#include "AggressiveAgentStrategy.h"
#include "Game.h"

namespace Gaming{
    const double AggressiveAgentStrategy::DEFAULT_AGGRESSION_THRESHOLD = Game::STARTING_AGENT_ENERGY * 0.75;

    AggressiveAgentStrategy::AggressiveAgentStrategy(double agentEnergy){
        __agentEnergy = agentEnergy;
    }

    AggressiveAgentStrategy::~AggressiveAgentStrategy(){

    }

    ActionType AggressiveAgentStrategy::operator()(const Surroundings &s) const{
        std::vector<int> agentPositions;
        std::vector<int> advantagePositions;
        std::vector<int> foodPositions;
        std::vector<int> emptyPositions;
        int emptySpace = 4;
       // bool moveToEmpty = false;
        bool nabbed = false;
        for (int i = 0; i < 9; i++){
            if (__agentEnergy >= DEFAULT_AGGRESSION_THRESHOLD && (s.array[i] == SIMPLE || s.array[i] == STRATEGIC)){
               // emptySpace = i;
                //break;
                agentPositions.push_back(i);
            }
            if (s.array[i] == ADVANTAGE) {
                //emptySpace = i;
                advantagePositions.push_back(i);
            }
            else if (s.array[i] == FOOD) {
                //emptySpace = i;
                //nabbed = true;
                foodPositions.push_back(i);
            }
            else if (s.array[i] == EMPTY) {
                //emptySpace = i;
               // moveToEmpty = true;
                emptyPositions.push_back(i);
            }

        }
        unsigned int randNum;
        if (agentPositions.size() > 0) {
            randNum = rand() % agentPositions.size();
            emptySpace = agentPositions[randNum];
        }
        else if (advantagePositions.size() > 0) {
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

