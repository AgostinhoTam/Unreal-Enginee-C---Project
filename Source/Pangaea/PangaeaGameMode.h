// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "PangaeaGameMode.generated.h"

UCLASS(minimalapi)
class APangaeaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APangaeaGameMode();
	~APangaeaGameMode();

	//	発射物処理
	class AProjectile* SpawnOrGetFireball(UClass* ProjectileClass);
	void RecycleFireball(AProjectile* Fireball);

protected:
	TQueue<AProjectile*, EQueueMode::Spsc> FireballPool;
};
