// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaGameMode.h"
#include "PangaeaPlayerController.h"
#include "Projectile.h"
#include "UObject/ConstructorHelpers.h"

APangaeaGameMode::APangaeaGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APangaeaPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_PlayerAvatar"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

APangaeaGameMode::~APangaeaGameMode()
{
	AProjectile* fireball;
	while (!FireballPool.IsEmpty() && FireballPool.Dequeue(fireball))
	{
		fireball->Destroy();
	}
	FireballPool.Empty();
}

AProjectile* APangaeaGameMode::SpawnOrGetFireball(UClass* ProjectileClass)
{
	AProjectile* fireball = nullptr;
	//	リストに弾がないなら
	if (FireballPool.IsEmpty())
	{
		//	ファイヤーボール生成
		fireball = Cast<AProjectile>(GetWorld()->SpawnActor(ProjectileClass));
	}
	else
	{
		//	Poolに弾あるなら、その弾を管理下から取り出す
		FireballPool.Dequeue(fireball);
		fireball->Reset();
	}
	return fireball;
}

void APangaeaGameMode::RecycleFireball(AProjectile* Fireball)
{
	if (Fireball == nullptr)
	{
		return;
	}
	//	再利用できるようにリセット、Poolに入り直す
	Fireball->SetActorHiddenInGame(true);
	Fireball->SetActorEnableCollision(false);
	Fireball->SetActorTickEnabled(false);
	FireballPool.Enqueue(Fireball);
}
