#include "../../../include/bodies/bullet/bullet.h"
#include "../../../include/world.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2World.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Body.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Fixture.h"
#include "../../../../libs/Box2D-master/Box2D/Collision/Shapes/b2PolygonShape.h"
#include "../../../../common/include/types.h"
#include "../../../../common/include/update.h"
#include <cstdint>

Bullet::Bullet(
    uint32_t body_id, World *world,
    float x, float y, DIRECTION direction):
    Body(body_id, world, ENTITY::BULLET),
    direction(direction) {
    b2BodyDef b2bodydef;
    b2bodydef.type = b2_dynamicBody;
    b2bodydef.position.Set(x, y);
    b2bodydef.userData = (void *) this;
    b2bodydef.awake = false;

    this->b2body = world->getB2World()->CreateBody(&b2bodydef);

    b2PolygonShape b2polygonshape;
    b2polygonshape.SetAsBox(this->WIDTH/2, this->HEIGHT/2);

    b2FixtureDef b2fixturedef;
    b2fixturedef.shape = &b2polygonshape;
    b2fixturedef.userData = (void *) this;

    this->b2body->CreateFixture(&b2fixturedef);
}

Update Bullet::createUpdate(COMMAND command) const {
    Update update(
        command,
        this->entity,
        this->BODY_ID,
        STATUS::NONE_STATUS,
        this->b2body->GetPosition().x * ZOOM_FACTOR,
        this->b2body->GetPosition().y * ZOOM_FACTOR,
        0);
    return update;
}

void Bullet::handleBeginContactWith(Body *other_body) {
    other_body->handleBeginContactWith(this);
}

void Bullet::handleEndContactWith(Body *other_body) {
    other_body->handleEndContactWith(this);
}
