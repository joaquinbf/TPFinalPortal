#ifndef __GATE_H__
#define __GATE_H__

#include "../body.h"
#include "gate_state.h"
#include "closed_gate_state.h"
#include "opening_gate_state.h"
#include "open_gate_state.h"
#include "closing_gate_state.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2World.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Body.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Fixture.h"
#include "../../../../libs/Box2D-master/Box2D/Collision/Shapes/b2PolygonShape.h"
#include "../../../../common/include/update.h"
#include "../../../../common/include/types.h"
#include "../../boolean_suppliers/boolean_block.h"
#include <cstdint>

#define SIZE_RATE 0.05

class World;
class Chell;
class Block;
class Button;
class Rock;
class BooleanBlock;

class Gate: public Body {
private:
    const float MAX_WIDTH = 2.00;
    const float MAX_HEIGHT = 4.00;
    const float MIN_HEIGHT = 0.01;
    float width;
    float height;
    ClosedGateState closed_gate_state;
    OpeningGateState opening_gate_state;
    OpenGateState open_gate_state;
    ClosingGateState closing_gate_state;
    GateState *state;
    BooleanSupplier *boolean_supplier;

public:
    /* Instancia una compuerta de id 'body_id' en b2world en (x, y) */
    Gate(World *world, float x, float y);

    virtual Update createUpdate(COMMAND command) const;

    /* Asigna un boolea supplier a la compuerta */
    void setBooleanSupplier(BooleanSupplier *boolean_supplier);

    /* Maneja el inicio de contacto con otro cuerpo */
    virtual void handleBeginContactWith(Body *other_body, b2Contact *contact);

    /* Maneja el fin de contacto con otro cuerpo */
    virtual void handleEndContactWith(Body *other_body, b2Contact *contact);

    /* Aplica una accion dependiendo de su estado */
    virtual void applyStateAction() override;

    /* Intenta cambiar su estado si las condiciones de boolean block
     * fueron satisfechas */
    void tryChangeState();

    /* Indica si se satisface la condicion de sus elementos */
    bool conditionIsMeet();

    /* Cambia su estado a abriendo */
    void changeStateToOpening();

    /* Cambia su estado a abierto */
    void changeStateToOpen();

    /* Cambia el estado a cerrando */
    void changeStateToClosing();

    /* Cambia el estado a cerrado */
    void changeStateToClosed();

    /* Achica la compuerta */
    void shrink();

    /* Agranda la compuerta. */
    void grow();

    /* Indica si la compuerta esta en su minima altura */
    bool isOnMinSize() const;

    /* Indica si esta en su maxima altura. */
    bool isOnMaxSize() const;
};

#endif
