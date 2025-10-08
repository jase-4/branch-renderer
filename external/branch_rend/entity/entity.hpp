#ifndef ENTITY_HPP
#define  ENTITY_HPP

class Entity {
    public:
        using ID = unsigned int;
        static ID currentID;
    
        Entity() : id(currentID++) {}
    
        ID getID() const { return id; }
    
    private:
        ID id;
    };
    
 #endif