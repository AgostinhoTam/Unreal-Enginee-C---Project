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
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//	パラメータ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class APangaeaCharacter* Holder;
	UPROPERTY(EditAnywhere, Category="Weapon Params")
	float Strength = 10.0f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	メッシュ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess=true))
	UStaticMeshComponent* MeshComp;

	//	当たり判定処理
	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION()
	bool IsWithinAttackRange(float AttackRange,AActor* Target);	//	汎用化でAActor
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
	
};
