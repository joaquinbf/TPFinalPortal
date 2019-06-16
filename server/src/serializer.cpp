#include "../include/serializer.h"

#include <iostream>
#include <map>
#include "yaml-cpp/yaml.h"
#include "../include/world.h"
#include "../include/bodies/body.h"
#include "../include/bodies/gate/gate.h"
#include "../include/bodies/gate/gateable.h"
#include "../include/bodies/receiver/receiver.h"
#include "../include/bodies/button/button.h"
#include "../../common/include/types.h"
#include "../../common/include/update.h"
#include "../../editor/editor_defines.h"

void Serializer::deserialize(World *world, std::string filepath) const {
    GateLogic gate_logic;

    YAML::Node config = YAML::LoadFile(filepath);
    YAML::Node escenario = config["escenario"];
    std::size_t cant_celdas = escenario["cantidadCeldas"].as<std::size_t>();

    for (std::size_t i = 0; i < cant_celdas; ++i) {
        if (config["celdas"][i]["ocupado"].as<bool>()) {
            this->deserializeBody(world, config["celdas"][i], gate_logic);
        }
    }

    this->connect(config, gate_logic);
}

void Serializer::deserializeBody(
    World *world, const YAML::Node &celda,
    GateLogic &gate_logic) const {
    int idclass = celda["idClass"].as<int>();
    int idobject = celda["idObject"].as<int>();
    int editor_x = celda["x"].as<int>();
    int editor_y = celda["y"].as<int>();
    float x = editor_x/X_SERIAL_FACTOR;
    float y = editor_y/Y_SERIAL_FACTOR;

    switch (idclass) {
        case IDCLASS_NULL:
            break;
        case IDCLASS_CHELL:
            world->createChell(x, y);
            break;
        case IDCLASS_BLOQUE_ROCA:
            world->createSquareStoneBlock(x, y);
            break;
        case IDCLASS_BLOQUE_METAL:
            world->createSquareMetalBlock(x, y);
            break;
        case IDCLASS_COMPUERTA_REG: {
            Gate *gate = world->createRegularGate(x, y + Y_GATE);
            gate_logic.gates[idobject] = gate;
            break;
        }
        case IDCLASS_COMPUERTA_AND: {
            Gate *gate = world->createAndGate(x, y + Y_GATE);
            gate_logic.gates[idobject] = gate;
            break;
        }
        case IDCLASS_COMPUERTA_OR: {
            Gate *gate = world->createOrGate(x, y + Y_GATE);
            gate_logic.gates[idobject] = gate;
            break;
        }
        case IDCLASS_BOTON: {
            Button *button = world->createButton(x, y + Y_BUTTON);
            gate_logic.booleansups[idobject] = (BooleanSupplier *)button;
            break;
        }
        case IDCLASS_RECEPTOR: {
            Receiver *receiver = world->createReceiver(x, y);
            gate_logic.booleansups[idobject] = (BooleanSupplier *)receiver;
            break;
        }
        case IDCLASS_PASTEL:
            world->createCake(x, y);
            break;
        case IDCLASS_ACIDO:
            world->createAcid(x, y);
            break;
        case IDCLASS_ROCA:
            world->createRock(x, y);
            break;
        // case IDCLASS_BARRERA_VERTICAL:
        // case IDCLASS_BARRERA_HORIZONTAL:
        case IDCLASS_DIAGONAL_0:
            world->createDiagonalMetalBlock(x, y, ORIENTATION::ORIENTATION_0);
            break;
        case IDCLASS_DIAGONAL_90:
            world->createDiagonalMetalBlock(x, y, ORIENTATION::ORIENTATION_3);
            break;
        case IDCLASS_DIAGONAL_180:
            world->createDiagonalMetalBlock(x, y, ORIENTATION::ORIENTATION_2);
            break;
        case IDCLASS_DIAGONAL_270:
            world->createDiagonalMetalBlock(x, y, ORIENTATION::ORIENTATION_1);
            break;
        case IDCLASS_EMISOR_0:
            world->createLauncher(x, y, DIRECTION::RIGHT_DIRECTION);
            break;
        case IDCLASS_EMISOR_90:
            world->createLauncher(x, y, DIRECTION::UP_DIRECTION);
            break;
        case IDCLASS_EMISOR_180:
            world->createLauncher(x, y, DIRECTION::LEFT_DIRECTION);
            break;
        case IDCLASS_EMISOR_270:
            world->createLauncher(x, y, DIRECTION::DOWN_DIRECTION);
    }
}

void Serializer::connect(
    const YAML::Node &config,
    GateLogic &gate_logic) const {
    for (const auto pair: gate_logic.gates) {
        int idgate = pair.first;
        Gate *gate = pair.second;
        int size = config["compuertas"][idgate]["size"].as<int>();
        for (int i = 0; i < size; i++) {
            int idbool = config["compuertas"][idgate][i]["idObject"].as<int>();
            BooleanSupplier *bs = gate_logic.booleansups[idbool];
            bool negate = config["compuertas"][idgate][i]["negado"].as<bool>();
            this->add(gate, bs, negate);
        }
    }
}

void Serializer::add(Gate *gate, BooleanSupplier *booleansup, bool negate) const {
    BooleanBlock *gate_block = gate->getBooleanBlock();

    if (negate) {
        BooleanBlockFactory *bbf = gate->getWorld()->getBooleanBlockFactory();
        BooleanBlock *not_block = bbf->createNotBlock();
        not_block->add(booleansup);
        gate_block->add(not_block);
    } else {
        gate_block->add(booleansup);
    }
    Gateable *gateable = (Gateable *) booleansup;
    gateable->setGate(gate);
}
