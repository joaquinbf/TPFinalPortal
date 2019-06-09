#ifndef __LAUNCH_H__
#define __LAUNCH_H__

#include "../body.h"
#include "../../../../common/include/types.h"
#include "../../../../common/include/update.h"
#include <cstdint>

class World;
class Bullet;

class Launcher: public Body {
private:
    DIRECTION direction;
    uint32_t counter;
    const float WIDTH = 2.00;
    const float HEIGHT = 2.00;
    const float MIN_CREATE_DISTANCE = 3;
    const float FRAMES_PER_LAUNCH = 50;

public:
    /* Instancia un launcher de id 'body_id' sobre world en la
     * posicion (x, y) apuntado hacia la direccion dada. */
    Launcher(
        World *world,
        float x, float y,
        DIRECTION direction);

    /* Crea una update con el comando indicado */
    virtual Update createUpdate(COMMAND command) const;

    /* Maneja el inicio de contacto con otro cuerpo */
    virtual void handleBeginContactWith(Body *other_body);

    /* Mata a bullet */
    virtual void handleBeginContactWith(Bullet *bullet) override;

    /* Maneja el fin de contacto con otro cuerpo */
    virtual void handleEndContactWith(Body *other_body);

    /* Aplica una accion dependiendo de su estado */
    virtual void applyStateAction() override;

    /* Dispara una bala en la direccion del launcher */
    Bullet *fireABullet();
};

#endif
