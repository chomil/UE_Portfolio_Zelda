#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class PLAYER_ANISTATE : uint8
{
	NONE,
	IDLE,
	WALK,
	DASH,
	JUMP,
	LAND,
	ATTACK
};