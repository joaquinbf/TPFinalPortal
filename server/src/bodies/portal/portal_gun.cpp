#include "../../../include/bodies/portal/portal_gun.h"
#include "../../../include/bodies/portal/portal.h"
#include "../../../include/bodies/chell/chell.h"
#include "../../../include/ray_cast_closest_body_callback.h"
#include "../../../include/world.h"
#include <iostream>
#include "../../../../libs/Box2D-master/Testbed/Framework/DebugDraw.h"


PortalGun::PortalGun(Chell *chell):
    chell(chell),
    portal_one(nullptr),
    portal_two(nullptr) {
}

PortalGun::~PortalGun() {
    this->destroyPortal(portal_one);
    this->destroyPortal(portal_two);
}

Portal *PortalGun::getPortalOne() const {
    return this->portal_one;
}

Portal *PortalGun::getPortalTwo() const {
    return this->portal_two;
}

void PortalGun::firePortalOne(float x, float y) {
    this->destroyPortal(this->portal_one);
    this->portal_one = this->firePortal(NPORTAL1, b2Vec2(x, y));
    this->setOppositePortals(this->portal_one, this->portal_two);
}

void PortalGun::firePortalTwo(float x, float y) {
    this->destroyPortal(this->portal_two);
    this->portal_two = this->firePortal(NPORTAL2, b2Vec2(x, y));
    this->setOppositePortals(this->portal_two, this->portal_one);
}

void PortalGun::destroyPortal(Portal *portal) {
    if (portal != nullptr) {
        chell->getWorld()->destroyBody(portal);
    }
}

Portal *PortalGun::firePortal(uint8_t portal_number, b2Vec2 pos) {
    RayCastClosestBodyCallback callback;
    World *world = chell->getWorld();
    Portal *portal = nullptr;

    std::cout << "chell pos: " << chell->getPosX() << ", "
              << chell->getPosY() << std::endl;

    world->getB2World()->RayCast(
        &callback,
        chell->getPosition(),
        1000 * (pos - chell->getPosition()));

    if (callback.hasHit() && callback.getBody()->canOpenPortalOnSurface()) {
        portal = world->createPortal(
            portal_number,
            callback.getPoint(),
            callback.getNormal());
    }

    return portal;
}

void PortalGun::setOppositePortals(Portal *portal, Portal *opposite) {
    if (portal != nullptr) {
        portal->setOppositePortal(opposite);
    }
    if (opposite != nullptr) {
        opposite->setOppositePortal(portal);
    }
}
