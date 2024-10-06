//
// Created by David Valenzuela on 12/28/23.
//

#include "../header/EntityManager.h"

void EntityManager::update()
{
    for (auto & e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    removeDeadEntities(m_entities);

    for (auto & [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }

    m_toAdd.clear();
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
    vec.erase(std::remove_if(vec.begin(), vec.end(),
                             [](const std::shared_ptr<Entity>& e) {
                                    return !e->isActive();
                                }), vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
    m_toAdd.push_back(e);
    return e;
}

EntityVec &EntityManager::getEntities()
{
    return m_entities;
}

EntityVec &EntityManager::getEntities(const std::string &tag)
{
    return m_entityMap[tag];
}

EntityManager::EntityManager()
{

}
