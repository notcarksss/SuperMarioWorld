#pragma once

#include "Player.h"
#include "ContactListener.h"

class LevelData;
struct Platform;
struct Pipe;
struct Item;

class Camera;

class Level : public ContactListener
{
public:
	Level();
	virtual ~Level();

	Level& operator=(const Level&) = delete;
	Level(const Level&) = delete;

	void Tick(double deltaTime);
	void Paint();

	void Reset();

	void AddItem(Item* newItem);

	double GetWidth();
	double GetHeight();

	bool IsPlayerOnGround();

	void SetShowingMessage(bool showingMessage);
	bool IsShowingMessage();

	void GiveItemToPlayer(Item* itemPtr);

private:
	void PreSolve(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr, bool & enableContactRef);
	void BeginContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr);
	void EndContact(PhysicsActor *actThisPtr, PhysicsActor *actOtherPtr);

	void DEBUGPaintZoomedOut();

	void PaintHUD();
	unsigned int GetNumberOfDigits(unsigned int i);
	void PaintSeveralDigitNumber(int x, int y, int number, bool yellow);
	void PaintSeveralDigitLargeNumber(int x, int y, int number);
	RECT2 GetSmallSingleNumberSrcRect(int number, bool yellow);
	RECT2 GetLargeSingleNumberSrcRect(int number);

	void TogglePaused();
	void SetPaused(bool paused);

	void ReadLevelData(int levelIndex);
	LevelData* m_LevelDataPtr = nullptr;

	// NOTE: We *could* make this an array, in case the player tries to collect two items
	// in the same frame, but that isn't very likely and will be caught in the next frame
	// anyways, so this should be just fine
	Item* m_ItemToBeRemovedPtr = nullptr;

	PhysicsActor* m_ActLevelPtr = nullptr;

	bool m_Paused = false;
	// NOTE: This stupid variable is only here because
	// Box2D won't let us set all the actors to inactive during a PreSolve OR a BeginContact
	// so we need to save the state and update it next tick instead....
	bool m_WasPaused = false;

	bool m_ShowingMessage = false;

	int m_TotalTime; // How long the player has to complete this level
	double m_SecondsElapsed = 0.0; // How many real-time seconds have elapsed

	// How many pixels wide the foreground of the level is
	double m_Width;
	// How many pixels high the foreground of the level is
	double m_Height;

	// TODO: Rename this to Mario? What about Luigi? Add player 2?
	Player *m_PlayerPtr = nullptr;
	bool m_IsPlayerOnGround = false;

	Camera* m_Camera = nullptr;
};
