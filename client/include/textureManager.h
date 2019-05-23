#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../include/SdlTexture.h"

#define CHELL_IDLE_TEXTURE "../resources/textures/chell-resting-idle.png"
#define CHELL_JIG_TEXTURE "../resources/textures/chell-jig.png"
#define CHELL_RUNNING_TEXTURE "../resources/textures/chell-running.png"
#define CHELL_STOPING_TEXTURE "../resources/textures/chell-stoping.png"
#define CHELL_TURN_TEXTURE "../resources/textures/chell-turn.png"
#define CHELL_JUMP_RISE_TEXTURE "../resources/textures/chell-jump-rise.png"
#define CHELL_JUMP_APEX_TEXTURE "../resources/textures/chell-jump-apex.png"
#define CHELL_JUMP_FALL_TEXTURE "../resources/textures/chell-jump-fall.png"
#define CHELL_JUMP_LAND_TEXTURE "../resources/textures/chell-jump-land.png"
#define CHELL_FIRE_TEXTURE "../resources/textures/chell-fire.png"
#define CHELL_FIRE_TO_IDLE_TEXTURE "../resources/textures/chell-fire-to-idle.png"

class TextureManager{
	SdlTexture chellIdleTexture;
	SdlTexture chellJigTexture;
	SdlTexture chellRunTexture;
	SdlTexture chellStopTexture;
	SdlTexture chellTurnTexture;
	SdlTexture chellJumpRiseTexture;
	SdlTexture chellJumpApexTexture;
	SdlTexture chellJumpFallTexture;
	SdlTexture chellJumpLandTexture;
	SdlTexture chellFireTexture;
	SdlTexture chellFireToIdleTexture;
public:	
	TextureManager();
	~TextureManager();
	
};

#endif
