//
// Created by dragon5hoarder on 12/14/2015.
//

#include "Game.h"
#include "Food.h"
#include "Advantage.h"
#include "Strategic.h"
#include "Simple.h"
#include "Exceptions.h"

namespace Gaming{


    const unsigned int Game::NUM_INIT_AGENT_FACTOR = 4;
    const unsigned int Game::NUM_INIT_RESOURCE_FACTOR = 2;
    const unsigned Game::MIN_WIDTH = 3;
    const unsigned Game::MIN_HEIGHT = 3;
    const double Game::STARTING_AGENT_ENERGY = 20;
    const double Game::STARTING_RESOURCE_CAPACITY = 10;

    void Game::populate(){

    }
//Default Constructor
    Game::Game() : __grid(MIN_WIDTH * MIN_HEIGHT, nullptr){
        __numInitAgents = 0;
        __numInitResources = 0;
        __width = MIN_WIDTH;
        __height = MIN_HEIGHT;
        __round = 0;
        __status = NOT_STARTED;
        __verbose = false;
    }

    Game::Game(unsigned width, unsigned height, bool manual) : __grid(width * height, nullptr){
        if (width < MIN_WIDTH || height < MIN_HEIGHT)
            throw InsufficientDimensionsEx(MIN_WIDTH, MIN_HEIGHT, width, height);

        __width = width;
        __height = height;
        __round = 0;
        __status = NOT_STARTED;
        __verbose = false;
        if (manual == false){
            __numInitAgents = (__width * __height) / NUM_INIT_AGENT_FACTOR;
            __numInitResources = (__width * __height) / NUM_INIT_RESOURCE_FACTOR;
            unsigned int numStrategic = __numInitAgents / 2;
            unsigned int numSimple = __numInitAgents - numStrategic;
            unsigned int numAdvantages = __numInitResources / 4;
            unsigned int numFoods = __numInitResources - numAdvantages;

            std::default_random_engine gen;
            std::uniform_int_distribution<int> d(0, __width * __height);

// populate Strategic agents
            while (numStrategic > 0) {
                int i = d(gen); // random index in the grid vector
                if (__grid[i] == nullptr) { // is position empty
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Strategic(*this, pos, Game::STARTING_AGENT_ENERGY);
                    numStrategic --;
                }
            }

            while (numSimple > 0) {
                int i = d(gen); // random index in the grid vector
                if (__grid[i] == nullptr) { // is position empty
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Simple(*this, pos, Game::STARTING_AGENT_ENERGY);
                    numSimple --;
                }
            }

            while (numAdvantages > 0) {
                int i = d(gen); // random index in the grid vector
                if (__grid[i] == nullptr) { // is position empty
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Advantage(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                    numAdvantages --;
                }
            }

            while (numFoods > 0) {
                int i = d(gen); // random index in the grid vector
                if (__grid[i] == nullptr) { // is position empty
                    Position pos(i / __width, i % __width);
                    __grid[i] = new Food(*this, pos, Game::STARTING_RESOURCE_CAPACITY);
                    numFoods --;
                }
            }
        }
        else {
            __numInitResources = 0;
            __numInitAgents = 0;
        }



    } // note: manual population by default

    Game::Game(const Game &another){

    }

    Game::~Game(){
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr)
                delete __grid[i];
        }
    }

    unsigned int Game::getNumPieces() const{
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr)
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumAgents() const{
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr && (__grid[i]->getType() == SIMPLE || __grid[i]->getType() == STRATEGIC))
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumSimple() const{
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr && __grid[i]->getType() == SIMPLE)
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumStrategic() const{
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr && __grid[i]->getType() == STRATEGIC)
                pieces++;
        }
        return pieces;
    }

    unsigned int Game::getNumResources(){
        unsigned int pieces = 0;
        for (int i = 0; i < __grid.size(); i++){
            if (__grid[i] != nullptr && (__grid[i]->getType() == FOOD || __grid[i]->getType() == ADVANTAGE))
                pieces++;
        }
        return pieces;

    }

    bool Game::addSimple(const Position &position){
        if (position.x > __height || position.y > __width || position.x < 0 || position.y < 0)
            throw OutOfBoundsEx(__width, __height, position.y, position.x);
        if(__grid[position.x * __width + position.y] == nullptr) {
            __grid[position.x * __width + position.y] = new Simple(*this, position, STARTING_AGENT_ENERGY);
            return true;
        }
        return false;
    }

    bool Game::addSimple(unsigned x, unsigned y){
        if (x > __height || y > __width || x < 0 || y < 0)
            throw OutOfBoundsEx(__width, __height, y, x);
        if (__grid[x * __width + y] == nullptr) {
            __grid[x * __width + y] = new Simple(*this, Position(x, y), STARTING_AGENT_ENERGY);
            return true;
        }
        return false;
    }

    bool Game::addStrategic(const Position &position, Strategy *s){
        if (position.x > __height || position.y > __width || position.x < 0 || position.y < 0)
            throw OutOfBoundsEx(__width, __height, position.y, position.x);
        if(__grid[position.x * __width + position.y] == nullptr) {
            __grid[position.x * __width + position.y] = new Strategic(*this, position, STARTING_AGENT_ENERGY, s);
            return true;
        }
        return false;
    }

    bool Game::addStrategic(unsigned x, unsigned y, Strategy *s){
        if (x > __height || y > __width || x < 0 || y < 0)
            throw OutOfBoundsEx(__width, __height, y, x);
        if (__grid[x * __width + y] == nullptr) {
            __grid[x * __width + y] = new Strategic(*this, Position(x, y), STARTING_AGENT_ENERGY, s);
            return true;
        }
        return false;
    }

    bool Game::addFood(const Position &position){
        if (position.x > __height || position.y > __width || position.x < 0 || position.y < 0)
            throw OutOfBoundsEx(__width, __height, position.y, position.x);
        if(__grid[position.x * __width + position.y] == nullptr) {
            __grid[position.x * __width + position.y] = new Food(*this, position, STARTING_RESOURCE_CAPACITY);
            return true;
        }
        return false;
    }

    bool Game::addFood(unsigned x, unsigned y){
        if (x > __height || y > __width || x < 0 || y < 0)
            throw OutOfBoundsEx(__width, __height, y, x);
        if (__grid[x * __width + y] == nullptr) {
            __grid[x * __width + y] = new Food(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
            return true;
        }
        return false;

    }

    bool Game::addAdvantage(const Position &position){
        if (position.x > __height || position.y > __width || position.x < 0 || position.y < 0)
            throw OutOfBoundsEx(__width, __height, position.y, position.x);
        if(__grid[position.x * __width + position.y] == nullptr) {
            __grid[position.x * __width + position.y] = new Advantage(*this, position, STARTING_RESOURCE_CAPACITY);
            return true;
        }
        return false;
    }

    bool Game::addAdvantage(unsigned x, unsigned y){
        if (x > __height || y > __width || x < 0 || y < 0)
            throw OutOfBoundsEx(__width, __height, y, x);
        if (__grid[x * __width + y] == nullptr) {
            __grid[x * __width + y] = new Advantage(*this, Position(x, y), STARTING_RESOURCE_CAPACITY);
            return true;
        }
        return false;
    }

    const Surroundings Game::getSurroundings(const Position &pos) const{
        Surroundings temp;
        int i = 0;
        for (int x = -1; x < 2; x++){
            for (int y = -1; y < 2; y++){
                if ((pos.x + x) < 0 || (pos.y + y) < 0 || (pos.x + x) > (__height - 1) || (pos.y + y) > (__width - 1))
                    temp.array[i] = INACCESSIBLE;
                else if (y == 0 && x == 0)
                    temp.array[i] = SELF;
                else if (__grid[(pos.x + x) * __width + (pos.y + y)] == nullptr)
                    temp.array[i] = EMPTY;
                else
                    temp.array[i] = __grid[(pos.x + x) * __width + (pos.y + y)]->getType();
                i++;
            }
        }
        return temp;

    }

    const ActionType Game::reachSurroundings(const Position &from, const Position &to){
        if ((from.x + 1) < to.x || (from.y + 1) < to.y || (from.x - 1) > to.x || (from.y - 1) > to.y)
            return STAY;
        else{
            if (to.x > from.x){
                if (to.y > from.y)
                    return SE;
                else if (to.y < from.y)
                    return SW;
                else
                    return S;
            }
            else if (to.x < from.x){
                if (to.y > from.y)
                    return NE;
                else if (to.y < from.y)
                    return NW;
                else
                    return N;
            }
            else{
                if (to.y > from.y)
                    return E;
                else if (to.y < from.y)
                    return W;
                else
                    return STAY;
            }
        }
    }

    bool Game::isLegal(const ActionType &ac, const Position &pos) const{
        Surroundings surr = getSurroundings(pos);
        int direction;
        switch (ac) {
            case N:
                direction = 1;
                break;
            case NE:
                direction = 2;
                break;
            case NW:
                direction = 0;
                break;
            case E:
                direction = 5;
                break;
            case W:
                direction = 3;
                break;
            case SE:
                direction = 8;
                break;
            case SW:
                direction = 6;
                break;
            case S:
                direction = 7;
                break;
            case STAY:
                direction = 4;
        }
        if (surr.array[direction] != INACCESSIBLE)
            return true;
        return false;
    }

    const Position Game::move(const Position &pos, const ActionType &ac) const{
        Position p(pos);
        switch (ac) {
            case N:
                p.x -= 1;
                break;
            case NE:
                p.x -= 1;
                p.y += 1;
                break;
            case NW:
                p.x -= 1;
                p.y -= 1;
                break;
            case E:
                p.y += 1;
                break;
            case W:
                p.y -= 1;
                break;
            case SE:
                p.x += 1;
                p.y += 1;
                break;
            case SW:
                p.x += 1;
                p.y -= 1;
                break;
            case S:
                p.x += 1;
                break;
        }
        return p;
    } // note: assumes legal, use with isLegal()

    void Game::round(){
        __round++;
        ActionType direction;
        Position newPos;

        if (__status == NOT_STARTED)
            __status = PLAYING;
        for (int i = 0; i < __grid.size(); i++){

            bool defeated = false;
            bool win = false;
            if(__grid[i] != nullptr){
                if(!__grid[i]->getTurned()) {
                    direction = __grid[i]->takeTurn(getSurroundings(__grid[i]->getPosition()));
                    __grid[i]->setTurned(true);
                    if(isLegal(direction, __grid[i]->getPosition())) {
                        newPos = move(__grid[i]->getPosition(), direction);
                        __grid[i]->setPosition(newPos);
                    }
                    if (__grid[newPos.x * __width + newPos.y] != nullptr) {
                        *__grid[i] * (*__grid[newPos.x * __width + newPos.y]);
                        if (!__grid[i]->isViable()) {
                            delete __grid[i];
                            defeated = true;
                        }
                        if (!__grid[newPos.x * __width + newPos.y]->isViable()) {
                            delete __grid[newPos.x * __width + newPos.y];
                            win = true;
                        }
                    }
                    if (defeated && win)
                        __grid[newPos.x * __width + newPos.y] = nullptr;
                    if (!defeated)
                        __grid[newPos.x * __width + newPos.y] = __grid[i];
                    if (direction != STAY)
                        __grid[i] = nullptr;
                }
            }
        }
        for (int i = 0; i < __grid.size(); i++){
            if(__grid[i] != nullptr){
                __grid[i]->age();
                __grid[i]->setTurned(false);
                if (!__grid[i]->isViable())
                    delete __grid[i];
            }
        }

        if (getNumResources() == 0) {
            __status = OVER;
        }


    }   // play a single round

    void Game::play(bool verbose){
        while (getStatus() != OVER) {
            if (verbose)
                std::cout << *this;
            round();
        }
    }

    std::ostream &operator<<(std::ostream &os, const Game &game){
        os << "Round " << game.__round << std::endl;
        for (int h = 0; h < game.__height; h++){
            for (int w = 0; w < game.__width; w++){
                os << "[";
                if (game.__grid[h * game.__width + w] != nullptr)
                    os << *(game.__grid[h * game.__width + w]);
                else
                    os << "     ";
                os << "]";
            }
            os << std::endl;
        }
        os << "Status: " << game.__status << std::endl;
    }
}
