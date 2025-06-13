// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PANGAEA_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeapon();

	//	パラメータ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACharacter* Holder;
	UPROPERTY(EditAnywhere, Category="Weapon Params")
	float Strength = 10.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComp;
	UPROPERTY(VIsibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	class USphereComponent* SphereComp;

	//	当たり判定処理
	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	bool IsWithinAttackRange(float AttackRange,AActor* Target);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
