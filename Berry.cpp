#include "stdafx.h"

#include "Berry.h"
#include "SpriteSheetManager.h"
#include "SpriteSheet.h"
#include "Level.h"

Berry::Berry(DOUBLE2& positionRef, Level* levelPtr, Colour colour) :
	Item(positionRef, Type::BERRY, levelPtr, Level::BERRY),
	m_Colour(colour)
{
	b2Filter collisionFilter = m_ActPtr->GetCollisionFilter();
	collisionFilter.maskBits = Level::YOSHI_TOUNGE; // I *only* collide with yoshi's tounge
	m_ActPtr->SetCollisionFilter(collisionFilter);

	m_AnimInfo.secondsPerFrame = 0.125;
	m_IsActive = true;
}

Berry::~Berry()
{
}

void Berry::Tick(double deltaTime)
{
	m_AnimInfo.Tick(deltaTime);
	m_AnimInfo.frameNumber %= 4;
}

void Berry::Paint()
{
	assert (m_Colour != Colour::NONE);

	double left = m_ActPtr->GetPosition().x;
	if (m_AnimInfo.frameNumber == 0) left -= 1;
	if (m_AnimInfo.frameNumber == 2) left += 1;
	
	double top = m_ActPtr->GetPosition().y;
	int srcCol = 3 + m_AnimInfo.frameNumber;
	if (m_AnimInfo.frameNumber == 3) 
	{
		srcCol -= 2;
	}

	int srcRow = 21;
	if (m_Colour == Colour::PINK) srcRow += 1;
	else if (m_Colour == Colour::GREEN) srcRow += 2;
	SpriteSheetManager::GetSpriteSheetPtr(SpriteSheetManager::GENERAL_TILES)->Paint(left, top, srcCol, srcRow);
}
