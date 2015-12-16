//
// Created by dragon5hoarder on 12/14/2015.
//

#include "Resource.h"
#include "Agent.h"


namespace Gaming{

    const double Resource::RESOURCE_SPOIL_FACTOR = 1.2;

    Resource::Resource(const Game &g, const Position &p, double __capacity) : Piece(g, p){
        this->__capacity = __capacity;
    }

    Resource::~Resource(){

    }

    double Resource::consume(){

    }

    void Resource::age(){
        if(__capacity < 0.000001)
            __capacity = 0.0;
        __capacity /= RESOURCE_SPOIL_FACTOR;
    }

    ActionType Resource::takeTurn(const Surroundings &s) const{
        return STAY;
    }

    Piece &Resource::operator*(Piece &other){
        return other.interact(this);
    }

    Piece &Resource::interact(Agent *other){
        other->addEnergy(this->getCapacity());
        this->finish();
        return *this;
    }

    Piece &Resource::interact(Resource *){
        //std::cout << "you did something wrong" << std::endl;
        return *this;
    }
}