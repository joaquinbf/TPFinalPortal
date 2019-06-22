#ifndef __DEAD_STATE_H__
#define __DEAD_STATE_H__

#include "../../../../common/include/types.h"
#include "chell_state.h"

class Chell;
class World;

class DeadState: public ChellState {
private:
    const unsigned int MAX_STEPS = 72;

public:
    DeadState(Chell *chell);
    virtual void handleBeginContactWith(Bullet *bullet) override;
    virtual void applyStateAction() override;

private:
    /* Agrega una update con command LOSE si todas las chells estan muertas*/
    void addLoseUpdateIfAllChellsAreDead(World *world) const;
};

#endif
