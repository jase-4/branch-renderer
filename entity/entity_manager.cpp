#include "entity_manager.hpp"

EntityManager& EntityManager::getInstance() {
    static EntityManager instance;
    return instance;
}

EntityManager::EntityManager() {
   
    std::cout << "EntityManager initialized\n";
}

void EntityManager::addEntity(Entity entity) {
    entities.push_back(entity);
}

const std::vector<Entity>& EntityManager::getEntities() const {
    return entities;
}
