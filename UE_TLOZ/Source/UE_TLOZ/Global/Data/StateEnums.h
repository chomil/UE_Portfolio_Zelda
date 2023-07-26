#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class PLAYER_ANISTATE : uint8
{
	NONE,
	IDLE,
	WALK,
	RUN,
	DASH,
	JUMP,
	LAND,
	ATTACK1,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	ATTACK_DASH,
	SWORD_ON,
	SWORD_OFF
};


UENUM(BlueprintType)
enum class MONSTER_AISTATE : uint8
{
	NONE,
	IDLE,
	CHASE,
	FIND,
	HIT,
	ATTACK,
	RETURN,
	PATROL,
	DEATH
};