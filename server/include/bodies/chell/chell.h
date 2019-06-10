#ifndef __CHELL_H__
#define __CHELL_H__

#include "../body.h"
#include "chell_state.h"
#include "idle_state.h"
#include "running_state.h"
#include "jumping_state.h"
#include "dead_state.h"
#include "../../../../common/include/key.h"
#include "../../../../common/include/keypad.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2World.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Body.h"
#include "../../../../libs/Box2D-master/Box2D/Dynamics/b2Fixture.h"
#include "../../../../libs/Box2D-master/Box2D/Collision/Shapes/b2PolygonShape.h"

#define JUMPSPEED 5
#define LEFTSPEED 1.2
#define RIGHTSPEED 1.2

class World;
class Acid;
class Button;
class Block;
class Gate;

class Chell: public Body {
private:
    bool is_facing_right;
    IdleState idle_state;
    RunningState running_state;
    JumpingState jumping_state;
    DeadState dead_state;
    ChellState *state;
    Keypad keypad;
    const float HALF_WIDTH = 0.50;
    const float HALF_HEIGHT = 1.00;
    const float DENSITY = 1;
    const float ANGLE = 0;
    const float FRICTION = 0.5;
    const float RESTITUTION = 0.0;

public:
    /* Instancia a chell de id 'body_id' sobre world en la
     * posicion (x, y) mirando a la derecha en estado idle. */
    Chell(World *world, float x, float y);

    /* Dispara un portal hacia la posicion (x, y) */
    void firePortalOne(float x, float y);

    /* Indica si chell esta mirando hacia la derecha */
    bool isFacingRight();

    /* Hace mirar a la derecha */
    void faceRight();

    /* Hace mirar a la izquierda */
    void faceLeft();

    /* Hace mirar en la direccion opuesta */
    void faceOppositeDirection();

    /* Devuelve un update de Command de chell */
    virtual Update createUpdate(COMMAND command) const override;

    /* Presiona la tecla izquierda de chell*/
    void pressLeft();

    /* Suelta la tecla izquierda de chell. */
    void releaseLeft();

    /* Presiona la tecla derecha */
    void pressRight();

    /* Suelta la tecla derecha */
    void releaseRight();

    void pressUp();

    void releaseUp();

    /* Cambia el estado a running */
    void changeStateToRunning();

    /* Cambia el estado a idle */
    void changeStateToIdle();

    /* Cambia el estado a jumping */
    void changeStateToJumping();

    /* Cambia el estado de chell a muerta */
    void changeStateToDead();

    /* Aterriza a chell */
    void land();

    /* Aplica un impulso a la izquierda */
    void applyLinearImpulseToLeft();

    /* Aplica un impulso a la derecha */
    void applyLinearImpulseToRight();

    /* Aplica un impulso hacia arriba */
    void applyLinearImpulseToUp();

    /* Para el movimiento hacia la izqueirda */
    void stopLeftMovement();

    /* Para el movimiento hacia la derecha */
    void stopRightMovement();

    /* Devuelve un puntero al keypad de chell */
    Keypad *getKeypad();

    /* Aplica una accion sobre chell dependiendo de su estado */
    virtual void applyStateAction();

    /* Maneja el contacto con otro cuerpo */
    virtual void handleBeginContactWith(Body *other_body, b2Contact *contact);

    /* Muere por el contacto con el acido */
    virtual void handleBeginContactWith(Acid *acid, b2Contact *contact) override;

    /* Aterriza sobre el bloque si esta saltando */
    virtual void handleBeginContactWith(Block *block, b2Contact *contact) override;

    /* Presionaa y aterriza sobre el boton si esta saltando */
    virtual void handleBeginContactWith(Button *button, b2Contact *contact) override;

    /* Maneja el fin de contacto con otro cuerpo */
    virtual void handleEndContactWith(Body *other_body, b2Contact *contact);

    /* Suelta el boton */
    virtual void handleEndContactWith(Button *button, b2Contact *contact) override;
};

#endif
