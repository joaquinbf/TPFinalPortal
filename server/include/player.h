#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <cstdint>
#include "../../common/include/socket.h"
#include "../../common/include/protocol.h"
#include "../../common/include/protected_queue.h"
#include "../include/inputReceiver.h"
#include "../include/updateSender.h"

class Player {
private:
    Protocol protocol;  
    InputReceiver inputReceiver;
    UpdateSender updateSender;
    ProtectedQueue<Update> updates;
public:
    explicit Player(Socket socket);
    ~Player();
    void start();
    void stop();
    void sendChellIdToClient(uint32_t id) const;
    void setInputPtr(ProtectedQueue<Action>* ptr);
    ProtectedQueue<Update>* getUpdatesPtr();
    void pushBackUpdate(Update update);
};

#endif
