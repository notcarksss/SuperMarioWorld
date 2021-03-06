#include "stdafx.h"

#include "KoopaTroopa.h"
#include "KoopaShell.h"
#include "Game.h"
#include "Player.h"
#include "SpriteSheetManager.h"
#include "SpriteSheet.h"
#include "SoundManager.h"

#include "EnemyPoofParticle.h"
#include "SplatParticle.h"

KoopaTroopa::KoopaTroopa(DOUBLE2 startingPos, Level* levelPtr, Colour colour) :
	Enemy(Type::KOOPA_TROOPA, startingPos, GetWidth(), GetHeight(), BodyType::DYNAMIC, levelPtr, this),
	m_Colour(colour)
{
	m_DirFacing = Direction::LEFT;
	m_AnimInfo.secondsPerFrame = 0.14;
	m_AnimationState = AnimationState::WALKING;

	b2Filter collisionFilter = m_ActPtr->GetCollisionFilter();
	collisionFilter.maskBits |= Level::FIREBALL;
	m_ActPtr->SetCollisionFilter(collisionFilter);
}

KoopaTroopa::~KoopaTroopa()
{
}

void KoopaTroopa::Tick(double deltaTime)
{
	Enemy::Tick(deltaTime);
	if (m_IsActive == false) return;

	if (m_ShouldAddKoopaShell)
	{
		m_ShouldAddKoopaShell = false;
		KoopaShell* koopaShellPtr = new KoopaShell(m_ActPtr->GetPosition() + DOUBLE2(0, -2), m_LevelPtr, m_Colour);
		m_LevelPtr->AddItem(koopaShellPtr, true);
	}
	if (m_ShouldAddMovingUpwardKoopaShell)
	{
		m_ShouldAddMovingUpwardKoopaShell = false;
		KoopaShell* koopaShellPtr = new KoopaShell(m_ActPtr->GetPosition(), m_LevelPtr, m_Colour, true);
		// No need for a contact listener, all this shell is going to do is fall off the screen, then be deleted
		m_LevelPtr->AddItem(koopaShellPtr, false); 
	}

	if (m_ShouldBeRemoved)
	{
		m_LevelPtr->RemoveEnemy(this);
		return;
	}

	if (m_AnimationState == AnimationState::UPSIDEDOWN_SHELLESS)
	{
		if (m_ActPtr->GetPosition().y > Game::HEIGHT + GetHeight())
		{
			m_ShouldBeRemoved = true;
		}

		return;
	}

	if (m_FramesSpentTurningAround > -1)
	{
		if (++m_FramesSpentTurningAround > FRAMES_OF_TURNAROUND)
		{
			m_FramesSpentTurningAround = -1;
		}
	}

	if (m_FramesSpentBeingShelless > -1)
	{
		if (++m_FramesSpentBeingShelless > FRAMES_OF_BEING_SHELLESS)
		{
			m_FramesSpentBeingShelless = -1;

			ChangeAnimationState(AnimationState::WALKING_SHELLESS);
		}
	}

	if (m_FramesSpentBeingSquashed> -1)
	{
		if (++m_FramesSpentBeingSquashed > FRAMES_OF_BEING_SQUASHED)
		{
			m_FramesSpentBeingSquashed = -1;
			
			// LATER: Add sound effect here

			 m_LevelPtr->RemoveEnemy(this);
			 return;
		}
	}

	m_AnimInfo.Tick(deltaTime);
	m_AnimInfo.frameNumber %= 2;

	// NOTE: Checks if this koopa is near an obstacle, if true then turns around
	DOUBLE2 point1 = m_ActPtr->GetPosition();
	DOUBLE2 point2 = m_ActPtr->GetPosition() + DOUBLE2(m_DirFacing * (GetWidth() / 2 + 2), 0);
	DOUBLE2 intersection, normal;
	double fraction = -1.0;
	int collisionBits = Level::LEVEL | Level::BLOCK | Level::ENEMY | Level::SHELL;

	if (m_LevelPtr->Raycast(point1, point2, collisionBits, intersection, normal, fraction))
	{
		ChangeDirections();
	}


	// NOTE: Checks if this koopa about to walk off an edge, and turns around if so
	point1 = m_ActPtr->GetPosition();
	point2 = m_ActPtr->GetPosition() + DOUBLE2(m_DirFacing * (GetWidth() / 2 + 2), GetHeight() / 2 + 4);
	fraction = -1.0;
	collisionBits = Level::LEVEL | Level::BLOCK;

	if (!m_LevelPtr->Raycast(point1, point2, collisionBits, intersection, normal, fraction))
	{
		ChangeDirections();
	}

	double xVel = WALK_VEL;
	if (m_DirFacing == Direction::LEFT)
	{
		xVel = -xVel;
	}

	m_ActPtr->SetLinearVelocity(DOUBLE2(xVel, m_ActPtr->GetLinearVelocity().y));
}

void KoopaTroopa::ChangeDirections()
{
	m_DirFacing = -m_DirFacing;

	if (m_AnimationState == AnimationState::WALKING)
	{
		m_FramesSpentTurningAround = 0;
	}
}

void KoopaTroopa::Paint()
{
	MATRIX3X2 matPrevWorld = GAME_ENGINE->GetWorldMatrix();

	DOUBLE2 playerPos = m_ActPtr->GetPosition();
	double centerX = playerPos.x;
	double centerY = playerPos.y;

	double xScale = 1, yScale = 1;
	if (m_FramesSpentTurningAround > -1)
	{
		if (m_FramesSpentTurningAround > 3)
		{
			xScale = m_DirFacing;
		}
		else
		{
			xScale = -m_DirFacing;
		}
	}
	else if (m_DirFacing == Direction::LEFT)
	{
		xScale = -1;
	}
	if (m_AnimationState == AnimationState::UPSIDEDOWN_SHELLESS)
	{
		yScale = -1;
		centerY += 5;
	}

	if (xScale != 1 || yScale != 1)
	{
		MATRIX3X2 matReflect = MATRIX3X2::CreateScalingMatrix(DOUBLE2(xScale, yScale));
		MATRIX3X2 matTranslate = MATRIX3X2::CreateTranslationMatrix(centerX, centerY);
		MATRIX3X2 matTranslateInverse = MATRIX3X2::CreateTranslationMatrix(-centerX, -centerY);
		GAME_ENGINE->SetWorldMatrix(matTranslateInverse * matReflect * matTranslate * matPrevWorld);
	}

	INT2 animationFrame = DetermineAnimationFrame();
	double yo = -4.5;
	SpriteSheetManager::GetSpriteSheetPtr(SpriteSheetManager::KOOPA_TROOPA)->Paint(centerX, centerY + yo, animationFrame.x, animationFrame.y);

	// TODO: Add crying particle for shelless koopas

	GAME_ENGINE->SetWorldMatrix(matPrevWorld);
}

INT2 KoopaTroopa::DetermineAnimationFrame()
{
	int row = 0, col = 0;

	switch (m_Colour)
	{
	case Colour::GREEN:
		row = 0;
		break;
	case Colour::RED:
		row = 1;
		break;
	default:
		OutputDebugString(String("Unhandled colour in KoopaTroopa::DetermineAnimationFrame:") + String(int(m_Colour)) + String("\n"));
		break;
	}

	switch (m_AnimationState)
	{
	case AnimationState::WALKING:
	{
		if (m_FramesSpentTurningAround > -1)
		{
			col = 0;
		}
		else
		{
			col = 1 + m_AnimInfo.frameNumber;
		}
	} break;
	case AnimationState::WALKING_SHELLESS:
	{
		col = 3 + m_AnimInfo.frameNumber;
	} break;
	case AnimationState::UPSIDEDOWN_SHELLESS:
	{
		col = 3;
	} break;
	case AnimationState::SHELLESS:
	{
		col = 5 + m_AnimInfo.frameNumber;
	} break;
	case AnimationState::SQUASHED:
	{
		col = 7;
	} break;
	default:
	{
		OutputDebugString(String("ERROR: Unhandled animation state in KoopaTroopa::DetermineAnimationState:") +
			String(m_AnimationState) + String("\n"));
	} break;
	}

	return INT2(col, row);
}

void KoopaTroopa::HeadBonk()
{
	SoundManager::PlaySoundEffect(SoundManager::Sound::SHELL_KICK);

	switch (m_AnimationState)
	{
	case AnimationState::WALKING:
	{
		ChangeAnimationState(AnimationState::SHELLESS);
		m_FramesSpentBeingShelless = 0;

		m_ShouldAddKoopaShell = true;
	} break;
	case AnimationState::SHELLESS:
	{
		ChangeAnimationState(AnimationState::UPSIDEDOWN_SHELLESS);

		m_ActPtr->SetSensor(true);
		m_ActPtr->SetLinearVelocity(DOUBLE2(0, -250));
	} break;
	case AnimationState::WALKING_SHELLESS:
	{
		ChangeAnimationState(AnimationState::SQUASHED);
		m_FramesSpentBeingSquashed = 0;
	} break;
	case AnimationState::SQUASHED:
	case AnimationState::UPSIDEDOWN_SHELLESS:
	{
		OutputDebugString(String("ERROR: KoopaTroopa::HeadBonk called when koopa was in invalid state!\n"));
	} break;
	}

	m_LevelPtr->GetPlayer()->AddScore(200, true, m_ActPtr->GetPosition());
}

void KoopaTroopa::ShellHit(bool shellWasBeingHeld)
{
	if (m_AnimationState == AnimationState::WALKING)
	{
		m_ShouldAddMovingUpwardKoopaShell = true;
	}

	SoundManager::PlaySoundEffect(SoundManager::Sound::SHELL_KICK);

	SplatParticle* splatParticlePtr = new SplatParticle(m_ActPtr->GetPosition());
	m_LevelPtr->AddParticle(splatParticlePtr);

	int score = 200;
	if (shellWasBeingHeld) score = 1000;
	m_LevelPtr->GetPlayer()->AddScore(score, true, m_ActPtr->GetPosition());

	m_ShouldBeRemoved = true;
}

void KoopaTroopa::StompKill()
{
	EnemyPoofParticle* poofParticlePtr = new EnemyPoofParticle(m_ActPtr->GetPosition());
	m_LevelPtr->AddParticle(poofParticlePtr);

	m_LevelPtr->GetPlayer()->AddScore(200, true, m_ActPtr->GetPosition());

	SoundManager::PlaySoundEffect(SoundManager::Sound::ENEMY_HEAD_STOMP_START);
	m_LevelPtr->GetPlayer()->ResetNumberOfFramesUntilEndStompSound();

	m_ShouldBeRemoved = true;
}

Colour KoopaTroopa::GetColour() const
{
	return m_Colour;
}

void KoopaTroopa::ChangeAnimationState(AnimationState newAnimationState)
{
	m_AnimationState = newAnimationState;
}

bool KoopaTroopa::IsShelless() const
{
	return  m_AnimationState == AnimationState::SHELLESS ||
			m_AnimationState == AnimationState::WALKING_SHELLESS;
}

KoopaTroopa::AnimationState KoopaTroopa::GetAnimationState() const
{
	return m_AnimationState;
}

int KoopaTroopa::GetWidth() const
{
	return WIDTH;
}

int KoopaTroopa::GetHeight() const
{
	return HEIGHT;
}
