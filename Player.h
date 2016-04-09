#pragma once

#include "Enumerations.h"
#include "SpriteSheet.h"
#include "Entity.h"
#include "GameItem.h"

class Level;
class SpriteSheet;
class Entity;

class Player : public Entity
{
public:
	enum class POWERUP_STATE
	{
		NORMAL, SUPER, FIRE, CAPE, BALLOON, STAR
	};
	enum class ANIMATION_STATE
	{
		// NOTE: FAST_FALLING is when mario is running full speed and has a 
		// positive y vel, not faster falling downwards
		WAITING, WALKING, RUNNING, JUMPING, SPIN_JUMPING, DUCKING, LOOKING_UPWARDS, 
		FALLING, FAST_FALLING, DYING, CLIMBING, CHANGING_DIRECTIONS
	};

	Player(Level* levelPtr);
	virtual ~Player();

	Player& operator=(const Player&) = delete;
	Player(const Player&) = delete;

	void Tick(double deltaTime);
	void TickAnimations(double deltaTime);

	void Paint();

	void Reset();

	int GetLives();
	int GetCoinsCollected();
	int GetDragonCoinsCollected();
	int GetStarsCollected();
	int GetScore();
	Item::TYPE GetExtraItemType();

	DOUBLE2 GetLinearVelocity();
	void SetLinearVelocity(const DOUBLE2& newLinearVelRef);
	DOUBLE2 GetPosition();
	int GetDirectionFacing();
	bool IsOnGround();

	void Die();

	void OnItemPickup(Item* itemPtr, Level* levelPtr);

	int GetWidth();
	int GetHeight();

	ANIMATION_STATE GetAnimationState();

private:
	DOUBLE2 CalculateAnimationFrame();

	void HandleKeyboardInput(double deltaTime, Level* levelPtr);

	void AddCoin(Level* levelPtr, bool playSound = true);
	void AddDragonCoin(Level* levelPtr);
	void AddLife();

	void ChangePowerupState(POWERUP_STATE newPowerupState, bool isUpgrade = true);
	SpriteSheet* GetSpriteSheetForPowerupState(POWERUP_STATE powerupState);

	void TakeDamage();

	static const double WALK_SPEED;
	static const double RUN_SPEED;

	bool m_IsOnGround = true;
	bool m_WasOnGround = true;
	int m_FramesSpentInAir;

	int m_Lives;
	int m_Coins;
	int m_DragonCoins;
	int m_Stars;
	int m_Score;
	bool m_NeedsNewFixture = false;

	static const int TOTAL_FRAMES_OF_POWERUP_TRANSITION = 50;
	int m_FramesOfPowerupTransitionElapsed; // NOTE: Perhaps this variable name is a tad long...
	POWERUP_STATE m_PrevPowerupState; // This is used to transition between states upon state change

	// NOTE: If this is set to something other than NONE, then at the start of Tick() we will create 
	// a new item with this type and assign it to m_ExtraItemPtr
	Item::TYPE m_ExtraItemToBeSpawnedType;
	Item* m_ExtraItemPtr = nullptr; // This is the extra item slot mario has at the top of the screen

	String AnimationStateToString(ANIMATION_STATE state);

	int m_DirFacingLastFrame;
	int m_DirFacing;

	POWERUP_STATE m_PowerupState;
	ANIMATION_STATE m_AnimationState;
};
