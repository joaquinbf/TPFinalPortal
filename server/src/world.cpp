#include "../include/world.h"

#include "../include/bodies/body.h"
#include "../include/bodies/receiver/receiver.h"
#include "../include/bodies/chell/chell.h"
#include "../include/bodies/button/button.h"
#include "../include/bodies/gate/gate.h"
#include "../include/bodies/acid/acid.h"
#include "../include/bodies/bullet/bullet.h"
#include "../include/bodies/launcher/launcher.h"
#include "../include/instructions/instruction.h"
#include "../include/instructions/instruction_factory.h"
#include "../include/boolean_suppliers/boolean_block_factory.h"
#include "../../libs/Box2D-master/Box2D/Dynamics/b2World.h"
#include "../../common/include/protected_queue.h"
#include "../../common/include/action.h"
#include "../../common/include/update.h"
#include <cstdint>
#include <vector>
#include <list>
#include <iostream>

World::World():
    b2world(new b2World(GRAVITY)),
    body_count(0),
    b2world_is_internal(true) {
    this->b2world->SetContactListener(&this->contact_listener);
}

World::World(float time_step):
    b2world(new b2World(GRAVITY)),
    body_count(0),
    b2world_is_internal(true),
    TIME_STEP(time_step) {
}

World::World(b2World *b2world):
    b2world(b2world),
    body_count(0),
    b2world_is_internal(false) {
    this->b2world->SetContactListener(&this->contact_listener);
}

World::~World() {
    this->deleteBodiesForDeletion();
    this->deleteAllBodies();
    this->deleteB2WorldIfInternal();
    for(auto it : this->pins){
        delete it.second;
    }
}

void World::insertNewBody(Body *body) {
    this->new_bodies.insert(body);
    this->bodies.insert(body);
    this->body_count++;
}

uint32_t World::getBodyCount() const {
    return this->body_count;
}

uint32_t World::getBodySize() const {
    return this->bodies.size();
}

b2World *World::getB2World() {
    return this->b2world;
}

Chell *World::createChell(float x, float y) {
    Chell *chell = new Chell(this, x, y);
    this->chells[chell->getBodyId()] = chell;
    return chell;
}

Block *World::createSquareMetalBlock(float x, float y) {
    Shape *shape = new SquareShape();
    Material *material = new MetalMaterial();
    Block *block = new Block(this, x, y, shape, material);
    return block;
}

Block *World::createSquareStoneBlock(float x, float y) {
    Shape *shape = new SquareShape();
    Material *material = new StoneMaterial();
    Block *block = new Block(this, x, y, shape, material);
    return block;
}

Button *World::createButton(float x, float y) {
    Button *button = new Button(this, x, y);
    return button;
}

void World::createGateWithButton(
    float x1, float y1,
    float x2, float y2,
    bool open_gate_when_button_is_pressed) {
    Gate *gate = new Gate(this, x1, y1);
    Button *button = new Button(this, x2, y2);

    button->setGate(gate);

    BooleanBlock *block;
    if (open_gate_when_button_is_pressed) {
        block = this->boolean_block_factory.createSameBlock();
    } else {
        block = this->boolean_block_factory.createNotBlock();
    }

    block->add(button);
    gate->setBooleanBlock(block);
}

Gate *World::createGate(float x, float y) {
    Gate *gate = new Gate(this, x, y);
    return gate;
}

Acid *World::createAcid(float x, float y) {
    Acid *acid = new Acid(this, x, y);
    return acid;
}

Launcher *World::createLauncher(float x, float y, DIRECTION direction) {
    Launcher *launcher = new Launcher(this,x, y, direction);
    return launcher;
}

Bullet *World::createBullet(float x, float y, DIRECTION direction) {
    Bullet *bullet = new Bullet(this, x, y, direction);
    return bullet;
}

Receiver *World::createReceiver(float x, float y) {
    Receiver *receiver = new Receiver(this, x, y);
    return receiver;
}

std::list<Update> World::getNewPlayerUpdates() const {
    std::list<Update> updates;
    for (Body *body: this->bodies) {
        Update update = body->createUpdate(COMMAND::CREATE_COMMAND);
        updates.push_back(update);
    }
    return updates;
}

void World::createNewPin(uint32_t id, int32_t x, int32_t y){
    Pin* ptr = new Pin(this->body_count,x,y);
    this->body_count++;
    if(this->pins.find(id) != this->pins.end()){
        this->changedPins[id] = this->pins[id]->getId();
        delete this->pins[id];
        this->pins[id] = ptr;
    } else {
        this->pins[id] = ptr;
    }
}

std::list<Update> World::getPinUpdateList(){
    std::list<Update> list;
    Update update;
    for(auto it : this->changedPins){
        if(it.second){
            list.push_back(Update(COMMAND::DESTROY_COMMAND,ENTITY::PIN,it.second,
                STATUS::NONE_STATUS,0,0,0));
            it.second = 0;
        }
    }
    for( auto it : this->pins){
        if(it.second != nullptr){
            if(it.second->hasUpdate()){
                update = it.second->getUpdate();
                if(update.getCommand()==COMMAND::DESTROY_COMMAND){
                    delete it.second;
                    this->pins.erase(it.first);
                }
                list.push_back(update);
            }
        }
    }
    return list;
}

std::list<Update> World::getBodyUpdates()  {
    std::list<Update> updates;
    this->addNewBodiesToUpdates(updates);
    this->addAllBodiesToUpdates(updates);
    this->addDeletedBodiesToUpdates(updates);
    return updates;
}


void World::step() {
    this->b2world->Step(this->TIME_STEP,
                        this->VELOCITY_ITERATIONS,
                        this->POSITION_ITERATIONS);
}

void World::createWorldOne() {
    for (int i = 0; i < 1; i++) {
        this->createChell(-6.00 + 2.00*i, 1.00);
    }

    for (int i = 0; i < 3; i++) {
        this->createSquareMetalBlock(-6.00 + 2.00*i, -1.00);
    }

    // Cy = 0.22 aprox
    this->createGateWithButton(-2, 2, -4, Cy, true);

    this->createLauncher(1, 1, DIRECTION::RIGHT_DIRECTION);
    this->createLauncher(1, 10, DIRECTION::DOWN_DIRECTION);
    this->createLauncher(10, 10, DIRECTION::LEFT_DIRECTION);
    this->createLauncher(10, 1, DIRECTION::UP_DIRECTION);

}

void World::applyAction(const Action &action) {
    InstructionFactory insf;
    Instruction *instruction = insf.createInstruction(action,this->chells,this);
    instruction->execute();
    delete instruction;
}

void World::applyStateActions() {
    for (Body *body: this->bodies) {
        body->applyStateAction();
    }
}

BooleanBlockFactory *World::getBooleanBlockFactory() {
    return &this->boolean_block_factory;
}

void World::addBodyForDeletion(Body *body) {
    this->bodies_for_deletion.insert(body);
}

void World::deleteBodiesForDeletion() {
    for (Body *body: this->bodies_for_deletion) {
        this->bodies.erase(body);
        delete body;
    }
    this->bodies_for_deletion.clear();
}

void World::deleteAllBodies() {
    for (Body *body: this->bodies) {
        delete body;
    }
}

void World::deleteB2WorldIfInternal() {
    if (this->b2world_is_internal) {
        delete this->b2world;
    }
}

std::list<Update> World::getUpdatesWithCommand(COMMAND command) const {
    std::list<Update> lista;

    for (Body *body: this->bodies) {
        Update update = body->createUpdate(command);
        lista.emplace_back(update);
        std::cout << "UPDATE: (" << update.getPosX()
                  << " ," << update.getPosY() << ")" << std::endl;
    }

    return lista;
}

void World::addNewBodiesToUpdates(std::list<Update> &updates) {
    for (Body *body: this->new_bodies) {
        Update update = body->createUpdate(COMMAND::CREATE_COMMAND);
        updates.push_back(update);
    }
    this->new_bodies.clear();
}

void World::addAllBodiesToUpdates(std::list<Update> &updates) {
    for (Body *body: this->bodies) {
        Update update = body->createUpdate(COMMAND::UPDATE_COMMAND);
        updates.push_back(update);
    }
}

void World::addDeletedBodiesToUpdates(std::list<Update> &updates) {
    for (Body *body: this->bodies_for_deletion) {
        Update update = body->createUpdate(COMMAND::DESTROY_COMMAND);
        updates.push_back(update);
    }
}
