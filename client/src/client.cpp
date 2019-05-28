#include "../include/client.h"

Client::Client(int x, int y)
: resx(x),resy(y),window(x,y),myChell(nullptr), scale(1),
textureManager(window),serverManager("localhost", PORT){
	//myChellId = this->serverManager.GetChellId();
	//FALTA RECIBIR CHELL ID
	myChellId = 0;
}

Client::~Client(){
	if(this->myChell != nullptr){
		delete this->myChell;
	}
	for( auto it = this->entities.begin(); it != this->entities.end(); ++it ){
		if(it->second != nullptr){
			delete it->second;
		}
	}
}

void Client::main(){
    this->running = true;
    Update update;
    InputManager inputManager(this->serverManager);
    inputManager.start();
    UpdateReceiver updateReceiver(this->serverManager,this->updates);
    updateReceiver.start();
    //std::thread updateReceiver([=]{this->updateReceiver();});

    while (inputManager.isRunning()){
		/*PROCESO UPDATES*/
		while(this->updates.try_pop(update)){
			this->updateHandler(update);
		}
		/*RENDER*/
        this->window.fill(); // Repinto el fondo gris
    	for( auto it = this->entities.begin(); it != this->entities.end(); ++it ){
			it->second->render(this->myChell->getPosX(),this->myChell->getPosY(),
				this->resx,this->resy,this->scale);
		}
		if(this->myChell != nullptr){
			this->myChell->renderCentered(this->resx,this->resy,this->scale);	
		}
        
        this->window.render();
        usleep(100000);
    }
    this->serverManager.stop();
    updateReceiver.stop();
    inputManager.join();
    updateReceiver.join();    
}

void Client::updateReceiver(){

}

void Client::updateHandler(Update update){
	EntityFactory ef;
	uint32_t id;
	switch(update.getCommand()){
		case COMMAND::CREATE_COMMAND:
			
			id = update.getIdObject();
			if(id == this->myChellId){ 
				this->myChell=(Chell *)ef.create(update,this->textureManager);
			} else {
				this->entities[id] = ef.create(update,this->textureManager);
			}
			break;
		case COMMAND::UPDATE_COMMAND:
			id = update.getIdObject();
			if(id == this->myChellId){
				this->myChell->update(update);
			}else{
				this->entities[id]->update(update);
			}
			break;
		case COMMAND::DESTROY_COMMAND:
			//FALTA IMPLEMENTAR
			break;
		default:
			break;
	}
}

void Client::zoomIn(){
	if(this->scale < 1){
		this->scale+=0.05;
	}
}

void Client::zoomOut(){
	if(this->scale > 0.5){
		this->scale-=0.05;
	}
}
