// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "PlayerAvatar.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//	飛び道具は物理的な当たり判定がないように
	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	_MeshComponent->SetGenerateOverlapEvents(false);
	SetRootComponent(_MeshComponent);

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	_LifeCountingDown = Lifespan;
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//	弾存続しているなら
	if (_LifeCountingDown > 0.0f)
	{
		//	移動処理
		FVector currentLocation = GetActorLocation();
		FVector vel = GetActorRotation().RotateVector(FVector::ForwardVector*Speed*DeltaTime);
		FVector nextLocation = currentLocation + vel;
		SetActorLocation(nextLocation);
		//	当たり判定（移動が早い時でも対応できるように）
		FHitResult hitResult;
		FCollisionObjectQueryParams objCollisionParams;
		objCollisionParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		if (GetWorld()->LineTraceSingleByObjectType(hitResult,currentLocation,nextLocation,objCollisionParams))
		{
			auto playerAvatar = Cast<APlayerAvatar>(hitResult.GetActor());
			if (playerAvatar != nullptr)
			{
				playerAvatar->Hit(Damage);
				PrimaryActorTick.bCanEverTick = false;
				Destroy();
			}
		}
		_LifeCountingDown -= DeltaTime;
	}
	else
	{
		PrimaryActorTick.bCanEverTick = false;
		Destroy();
	}

	

}

