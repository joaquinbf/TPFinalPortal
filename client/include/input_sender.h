#ifndef __INPUT_SENDER_H__
#define __INPUT_SENDER_H__

#include "../../common/include/thread.h"
#include "../../common/include/protected_queue.h"
#include "../../common/include/protocol.h"
#include "../../common/include/exceptions.h"
#include "../../common/include/action.h"

// Clase que se encarga de enviar Inputs y nada mas.
// supongo que se podria hacer mediante Templates.
// (Lo mismo para OutputReceiver).
class InputSender: public Thread {
private:
    Protocol *protocol;
    bool keep_running;
    ProtectedQueue<Action> action_queue;

public:
    InputSender(Protocol *protocol);
    virtual void run() override;
    void stop();
    void push(const Action &action);
};

#endif
