#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../include/SdlTexture.h"
#include "../../common/include/entity.h"
#include "../../common/include/types.h"
#include "../include/Area.h"

#define BLACK_TEXTURE "../resources/textures/miscellaneous/black.jpg"

#define CHELL_IDLE_TEXTURE "../resources/textures/chell/resting-idle.png"
#define CHELL_JIG_TEXTURE "../resources/textures/chell/jig.png"
#define CHELL_RUNNING_TEXTURE "../resources/textures/chell/running.png"
#define CHELL_STOPING_TEXTURE "../resources/textures/chell/stoping.png"
#define CHELL_TURN_TEXTURE "../resources/textures/chell/turn.png"
#define CHELL_JUMP_RISE_TEXTURE "../resources/textures/chell/jump-rise.png"
#define CHELL_JUMP_APEX_TEXTURE "../resources/textures/chell/jump-apex.png"
#define CHELL_JUMP_FALL_TEXTURE "../resources/textures/chell/jump-fall.png"
#define CHELL_JUMP_LAND_TEXTURE "../resources/textures/chell/jump-land.png"
#define CHELL_FIRE_TEXTURE "../resources/textures/chell/fire.png"
#define CHELL_FIRE_TO_IDLE_TEXTURE "../resources/textures/chell/fire-to-idle.png"

#define METAL_BLOCK_TEXTURE "../resources/textures/blocks/metal.png"
#define METAL_TRIAG_BLOCK_TEXTURE "../resources/textures/blocks/metal-triag.png"
#define STONE_BLOCK_TEXTURE "../resources/textures/blocks/stone.png"
#define LAUNCH_BLOCK_TEXTURE "../resources/textures/blocks/launch.png"
#define RECEIVER_BLOCK_TEXTURE "../resources/textures/blocks/receiver.png"

#define BUTTON_ON_TEXTURE "../resources/textures/button/on.png"
#define BUTTON_OFF_TEXTURE "../resources/textures/button/off.png"

#define GATE_CLOSED_TEXTURE "../resources/textures/gate/4-closed.png"
#define GATE_OPENED_TEXTURE	"../resources/textures/gate/4-opened.png"
#define GATE_OPENING_TEXTURE "../resources/textures/gate/4-opening.png"

#define ROCK_TEXTURE "../resources/textures/miscellaneous/rocks.png"
#define ACID_TEXTURE "../resources/textures/miscellaneous/acid.png"
#define BULLET_TEXTURE "../resources/textures/miscellaneous/bullet.png"
#define LASER_TEXTURE "../resources/textures/miscellaneous/laser.png"
#define CROSSHAIR_TEXTURE "../resources/textures/miscellaneous/crosshair.png"
#define PIN_TEXTURE "../resources/textures/miscellaneous/pin.png"

class TextureManager{
	SdlTexture blackTexture;
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
	SdlTexture metalBlockTexture;
	SdlTexture metalTriagBlockTexture;
	SdlTexture stoneBlockTexture;
	SdlTexture launchBlockTexture;
	SdlTexture receiverBlockTexture;
	SdlTexture buttonOnTexture;
	SdlTexture buttonOffTexture;
	SdlTexture rockTexture;
	SdlTexture gateClosedTexture;
	SdlTexture gateOpenedTexture;
	SdlTexture gateOpeningTexture;
	SdlTexture acidTexture;
	SdlTexture bulletTexture;
	SdlTexture laserTexture;
	SdlTexture crosshairTexture;
	SdlTexture pinTexture;
public:	
	TextureManager();
	explicit TextureManager(const SdlWindow& window);
	~TextureManager();
	const SdlTexture * getBlackTexture() const;
	const SdlTexture * getAcidTexturePointer() const;
	const SdlTexture * getBulletTexturePointer() const;
	const SdlTexture * getButtonTexturePointer(const STATUS st) const;
	const SdlTexture * getBlockTexturePointer(const ENTITY e) const;
	const SdlTexture * getChellTexturePointer(const STATUS st) const;
	const SdlTexture * getRockTexturePointer() const;
	const SdlTexture * getLaserTexturePointer() const;
	const SdlTexture * getGateTexturePointer(STATUS st) const;
	const SdlTexture * getCrosshairTexturePointer() const;
	const SdlTexture * getPinTexturePointer() const;	
	Area getChellFrameArea(const STATUS st, int frame) const;
	Area getGateFrameArea(uint32_t frame) const;
};

#endif
