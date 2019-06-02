#include "../../../include/bodies/block/block.h"
#include "../../../include/bodies/block/shape.h"
#include "../../../include/bodies/block/square_shape.h"
#include "../../../include/bodies/block/diagonal_shape.h"
#include "../../../include/bodies/block/material.h"
#include "../../../include/bodies/block/metal_material.h"
#include "../../../include/bodies/block/stone_material.h"
#include "../../../include/bodies/chell/chell.h"




Block::Block(
    uint32_t body_id, b2World *b2world,
    float x, float y,
    Shape *shape, Material *material):
    Body(body_id),
    shape(shape), material(material) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    bodyDef.position.Set(x, y);
    bodyDef.angle = this->ANGLE;
    bodyDef.userData = (void *) this;
    bodyDef.awake = false;

    this->b2body = b2world->CreateBody(&bodyDef);

    b2PolygonShape b2polygonshape = this->shape->giveShape(this->HALF_WIDTH,
                                                           this->HALF_HEIGHT);

    b2FixtureDef boxFixtureDef;
    boxFixtureDef.shape = &b2polygonshape;
    boxFixtureDef.density = this->DENSITY;

    this->b2body->CreateFixture(&boxFixtureDef);

    this->shape = shape;
    this->material = material;
}

Block::~Block() {
    delete shape;
    delete material;
}

Update Block::createUpdate(COMMAND command) const {
    ENTITY entity = this->shape->createEntityWithMaterial(this->material);

    Update update(
        command,
        entity,
        this->BODY_ID,
        STATUS::NONE_STATUS,
        this->b2body->GetPosition().x,
        this->b2body->GetPosition().y,
        0
    );
    return update;
}

void Block::handleBeginContactWith(Body *other_body) {

}

void Block::letBeginContactBeHandledBy(Chell *chell) {
    chell->changeStateToIdle();
}
