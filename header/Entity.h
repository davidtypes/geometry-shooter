//
// Created by David Valenzuela on 12/28/23.
//

#ifndef TRYINGSFML_ENTITY_H
#define TRYINGSFML_ENTITY_H

#include "Components.h"
#include <memory>
#include <string>

class Entity
{
private:
    friend class EntityManager;

    bool m_active = true;
    size_t m_id = 0;
    std::string m_tag = "default";

    Entity(const size_t id, const std::string& tag);

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape> cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput> cInput;
    std::shared_ptr<CScore> cScore;
    std::shared_ptr<CLifespan> cLifespan;
//    std::shared_ptr<CSprite> cSprite;

    bool isActive() const;
    const std::string & tag() const;
    const size_t id() const;
    void destroy();
};


#endif //TRYINGSFML_ENTITY_H
