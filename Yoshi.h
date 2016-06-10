#pragma once

#include "Entity.h"
#include "Item.h"
#include "SMWTimer.h"

class Player;
class Enemy;
class Message;
class YoshiEggBreakParticle;
class StarCloudParticle;

class Yoshi : public Entity
{
public:
	enum class AnimationState
	{
		EGG, BABY, WAITING, WILD, FALLING
	};

	Yoshi(DOUBLE2 position, Level* levelPtr);
	virtual ~Yoshi();

	Yoshi(const Yoshi&) = delete;
	Yoshi& operator=(const Yoshi&) = delete;

	void Tick(double deltaTime);
	void Paint();

	int GetWidth() const;
	int GetHeight() const;

	void EatItem(Item* itemPtr);
	void EatEnemy(Enemy* enemyPtr);

	bool IsHatching() const;
	void RunWild();
	void SetCarryingPlayer(bool carryingPlayer, Player* playerPtr = nullptr);

	double GetTongueLength() const;
	int GetDirectionFacing() const;

	AnimationState GetAnimationState() const;
	bool IsAirborne() const;
	void SpitOutItem();
	bool IsTongueStuckOut() const;
	void StickTongueOut(double deltaTime);
	SMWTimer GetTongueTimer() const;

	void SetPaused(bool paused);


	static const int JUMP_VEL; // Yoshi never uses this field directly, but the player class does while they are riding yoshi

	static const float HATCHING_SECONDS_PER_FRAME;
	static const float BABY_SECONDS_PER_FRAME;
	static const float WAITING_SECONDS_PER_FRAME;
	static const float WALKING_SECONDS_PER_FRAME;
	static const float RUNNING_SECONDS_PER_FRAME;
	static const float TONGUE_STUCK_OUT_SECONDS_PER_FRAME;

private:
	bool CalculateOnGround();
	void UpdatePosition(double deltaTime); // Only called when yoshi is "running wild"
	void TickAnimations(double deltaTime);
	void PaintAnimationFrame(double left, double top);
	void ChangeAnimationState(AnimationState newAnimationState);
	void SwallowItem();
	
	std::string AnimationStateToString();

	static const double EGG_GRAVITY_SCALE;
	static const std::string MESSAGE_STRING;

	static const int INITIAL_BUMP_Y_VEL;
	static const int RUN_VEL;
	static const int TONGUE_VEL;
	static const int HOP_VEL;

	static const int MAX_ITEMS_EATEN = 9;

	static const int WIDTH = 12;
	static const int HEIGHT = 16;

	AnimationState m_AnimationState;

	bool m_IsCarryingPlayer = false;
	bool m_IsTongueStuckOut = false;

	bool m_IsOnGround = false;
	bool m_WasOnGround = false;
	bool m_NeedsNewFixture = false;
	bool m_ShouldSpawnMushroom = false;

	double m_TongueXVel = 0.0;
	double m_TongueLength = 0.0;

	SMWTimer m_HatchingTimer;
	SMWTimer m_GrowingTimer;
	SMWTimer m_TongueTimer;  // Records how long yoshi's tongue has been stuck out for
	SMWTimer m_YappingTimer;  // Time until we yap our mouth again (only when not carrying the player and not running wild)

	SpriteSheet* m_SpriteSheetPtr = nullptr;
	PhysicsActor* m_ActToungePtr = nullptr;
	Player* m_PlayerPtr = nullptr;
	Message* m_MessagePtr = nullptr;

	// NOTE: This is used only for items which are not immediately eaten by yoshi
	Item::Type m_ItemInMouthType;
	Item* m_ItemToBeRemovedPtr = nullptr;
	Enemy* m_EnemyToBeRemovedPtr = nullptr;

	int m_ItemsEaten;
	int m_DirFacing;
};
