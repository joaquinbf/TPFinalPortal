#include "../include/world.h"

#include "../include/bodies/body.h"
#include "../include/bodies/portal/portal.h"
#include "../include/bodies/receiver/receiver.h"
#include "../include/bodies/chell/chell.h"
#include "../include/bodies/button/button.h"
#include "../include/bodies/gate/gate.h"
#include "../include/bodies/acid/acid.h"
#include "../include/bodies/rock/rock.h"
#include "../include/bodies/cake/cake.h"
#include "../include/bodies/bullet/bullet.h"
#include "../include/bodies/launcher/launcher.h"
#include "../include/bodies/laser/laser.h"
#include "../include/instructions/instruction.h"
#include "../include/instructions/instruction_factory.h"
#include "../include/boolean_suppliers/boolean_block_factory.h"
#include "../../libs/Box2D-master/Box2D/Dynamics/b2World.h"
#include "../../common/include/protected_queue.h"
#include "../../common/include/action.h"
#include "../../common/include/update.h"
#include "../include/bodies/block/block.h"
#include "../include/bodies/block/material.h"
#include "../include/bodies/block/shape.h"
#include "../include/bodies/block/metal_material.h"
#include "../include/bodies/block/stone_material.h"
#include "../include/bodies/block/square_shape.h"
#include "../include/bodies/block/diagonal_shape.h"
#include "../include/game_loop.h"
#include <cstdint>
#include <vector>
#include <list>
#include <iostream>

World::World():
    b2world(new b2World(GRAVITY)),
    body_count(0),
    b2world_is_internal(true),
    cake(nullptr),
    game_loop(
        this,
        &this->bodies,
        &this->internal_updates,
        &this->instructions) {
    this->b2world->SetContactListener(&this->contact_listener);
}

World::World(float time_step):
    b2world(new b2World(GRAVITY)),
    body_count(0),
    b2world_is_internal(true),
    TIME_STEP(time_step),
    cake(nullptr),
    game_loop(
        this,
        &this->bodies,
        &this->internal_updates,
        &this->instructions) {
    this->b2world->SetContactListener(&this->contact_listener);
}

World::World(b2World *b2world):
    b2world(b2world),
    body_count(0),
    b2world_is_internal(false),
    cake(nullptr),
    game_loop(
        this,
        &this->bodies,
        &this->internal_updates,
        &this->instructions) {
    this->b2world->SetContactListener(&this->contact_listener);
}

GameLoop *World::getGameLoop() {
    return &this->game_loop;
}

std::set<Body *> *World::getBodies() {
    return &this->bodies;
}


World::~World() {
    this->destroyChells();
    this->destroyAllBodies();
    this->deleteB2WorldIfInternal();
    for(auto it : this->pins){
        delete it.second;
    }
}

bool World::isValid() {
    return this->chells.size() != 0 && this->cake != nullptr;
}


bool World::isFinished() const {
    return this->everybodyAteTheCake();
}

std::map<uint32_t, Chell *> *World::getChells() {
    return &this->chells;
}

void World::removeFromChells(Chell *chell) {
    this->chells.erase(chell->getBodyId());
}

bool World::everybodyAteTheCake() const {
    if (this->cake == nullptr) {
        return false;
    }
    return this->chells.size() == this->cake->chellCount();
}

bool World::everybodyButOneAteTheCake() const {
    if (this->cake == nullptr) {
        return false;
    }
    return this->chells.size() - 1 == this->cake->chellCount();
}

void World::incBodyCount() {
    this->body_count++;
}

void World::addToBodies(Body *body) {
    this->bodies.insert(body);
}

void World::addUpdate(Update update) {
    this->internal_updates.push_back(update);
}

void World::destroyBody(Body *body) {
    this->bodies.erase(body);
    delete body;
}

void World::addInstruction(Instruction *instruction) {
    this->instructions.push_back(instruction);
}

std::list<uint32_t> World::getChellsIdList() const{
    std::list<uint32_t> chell_id_list;

    for (auto it = this->chells.begin(); it != this->chells.end(); ++it) {
        chell_id_list.push_back(it->first);
    }
    return chell_id_list;
}

void World::addExternalInput(ProtectedQueue<Action> *input) {
    Action action;
    while (input->try_pop(action)) {
        Instruction *instruction = this->instruction_factory.createInstruction(
                                                         action, chells, this);
        this->instructions.emplace_back(instruction);
    }
}

void World::bigStep() {
    this->step();
    this->applyInternalInstructions();
    this->applyStateActions();
    this->createUpdates();
}

void World::applyInternalInstructions() {
    while (!this->instructions.empty()) {
        Instruction *instruction = this->instructions.front();
        this->instructions.pop_front();
        instruction->execute();
        delete instruction;
    }
}

void World::createUpdates() {
    for (Body *body: this->bodies) {
        if (body->isAwake() && body->isActive()) {
            Update update = body->createUpdate(COMMAND::UPDATE_COMMAND);
            this->internal_updates.emplace_back(update);
        }
    }
}

void World::fillUpdates(ProtectedQueue<Update> *ext_updates) {
    while (!this->internal_updates.empty()) {
        Update update = internal_updates.front();
        internal_updates.pop_front();
        ext_updates->push(update);
        if (update.getIdClass() == ENTITY::CHELL) {
                std::cout << "UPDATE: STATUS " << update.getStatus()
                          << " --- ENTITY: " << update.getIdClass()
                          << " --- (" << update.getPosX()
                          << ", " << update.getPosY() << ")"
                          << "--- COMMAND: " << update.getCommand()
                          << "--- BODYID: " << update.getIdObject()
                          << std::endl;
        }
    }

    std::list<Update> pins = this->getPinUpdateList();
    for (Update &update: pins) {
        ext_updates->push(update);
    }
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
    Block *block = new Block(
        this, x, y, ORIENTATION::ORIENTATION_0, shape, material);
    return block;
}

Block *World::createSquareStoneBlock(float x, float y) {
    Shape *shape = new SquareShape();
    Material *material = new StoneMaterial();
    Block *block = new Block(
        this, x, y, ORIENTATION::ORIENTATION_0, shape, material);
    return block;
}

Block *World::createDiagonalMetalBlock(
    float x, float y, ORIENTATION orientation) {
    Shape *shape = new DiagonalShape();
    Material *material = new MetalMaterial();
    Block *block = new Block(this, x, y, orientation, shape, material);
    return block;
}


Button *World::createButton(float x, float y) {
    Button *button = new Button(this, x, y);
    return button;
}

Gate *World::createGate(float x, float y) {
    Gate *gate = new Gate(this, x, y);
    return gate;
}

Gate *World::createRegularGate(float x, float y) {
    Gate *gate = this->createGate(x, y);
    BooleanBlock *block = this->boolean_block_factory.createSameBlock();
    gate->setBooleanBlock(block);
    return gate;
}

Gate *World::createAndGate(float x, float y) {
    Gate *gate = this->createGate(x, y);
    BooleanBlock *block = this->boolean_block_factory.createAndBlock();
    gate->setBooleanBlock(block);
    return gate;
}

Gate *World::createOrGate(float x, float y) {
    Gate *gate = this->createGate(x, y);
    BooleanBlock *block = this->boolean_block_factory.createOrBlock();
    gate->setBooleanBlock(block);
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

Portal *World::createPortal(uint8_t portal_number, b2Vec2 pos, b2Vec2 normal) {
    Portal *portal = new Portal(this, portal_number, pos, normal);
    return portal;
}

Cake *World::createCake(float x, float y) {
    if (this->cake != nullptr) {
        return nullptr;
    }
    Cake *cake = new Cake(this, x, y);
    this->cake = cake;
    return cake;
}

Rock *World::createRock(float x, float y) {
    Rock *rock = new Rock(this, x, y);
    return rock;
}

Laser *World::createLaser(float x, float y, ANGLE angle) {
    Laser *laser = new Laser(this, x, y, angle);
    return laser;
}

std::list<Update> World::getNewPlayerUpdates() const {
    std::list<Update> updates;
    for (Body *body: this->bodies) {
        if (body->isActive()) {
            Update update = body->createUpdate(COMMAND::CREATE_COMMAND);
            updates.push_back(update);
        }
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

void World::step() {
    this->b2world->Step(this->TIME_STEP,
                        this->VELOCITY_ITERATIONS,
                        this->POSITION_ITERATIONS);
}

void World::createWorldOne() {
    for (int i = 1; i > -2; i--) {
        this->createChell(-3.00 + 2.00*i, 1.00);
    }
    this->createChell(-3.00 + 2.00*-3, 1.00);

    for (int i = 0; i < 100; i++) {
        this->createSquareMetalBlock(-48.00 + 2.00*i, -1.00);
    }
    this->createCake(50, 0.5);

    this->createRock(-10, 0);
    this->createSquareMetalBlock(-16, 9);

    this->createLauncher(-30, 5, DIRECTION::LEFT_DIRECTION);
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

void World::destroyChells() {
    Chell *chell;
    for (auto pair: this->chells) {
        chell = pair.second;
        this->destroyBody(chell);
    }
    this->chells.clear();
}


void World::destroyAllBodies() {
    for (Body *body: this->bodies) {
        this->destroyBody(body);
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
    }

    return lista;
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
