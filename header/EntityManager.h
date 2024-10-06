//
// Created by David Valenzuela on 12/28/23.
//

#ifndef TRYINGSFML_ENTITYMANAGER_H
#define TRYINGSFML_ENTITYMANAGER_H

#include "Entity.h"
#include <string>
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityVec m_toAdd;
    EntityMap m_entityMap;
    size_t m_totalEntities = 0;
public:
    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const std::string & tag);
    EntityVec& getEntities();
    EntityVec& getEntities(const std::string & tag);
    void removeDeadEntities(EntityVec & vec);
};


#endif //TRYINGSFML_ENTITYMANAGER_H
