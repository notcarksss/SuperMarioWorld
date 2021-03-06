#include "stdafx.h"

#include "CoinCollectParticle.h"
#include "Particle.h"
#include "SpriteSheetManager.h"
#include "SpriteSheet.h"

CoinCollectParticle::CoinCollectParticle(DOUBLE2 position) : Particle(LIFETIME, position)
{
	m_AnimInfo.secondsPerFrame = 0.055;
}

CoinCollectParticle::~CoinCollectParticle()
{
}

bool CoinCollectParticle::Tick(double deltaTime)
{
	m_AnimInfo.Tick(deltaTime);
	m_LifeRemaining = LIFETIME - m_AnimInfo.frameNumber - 1;

	return (m_LifeRemaining < 0);
}

void CoinCollectParticle::Paint()
{
	SpriteSheetManager::GetSpriteSheetPtr(SpriteSheetManager::COIN_COLLECT_PARTICLE)->Paint(m_Position.x, m_Position.y, m_AnimInfo.frameNumber, 0);
}