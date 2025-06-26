// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Projectile.generated.h"


UCLASS(Blueprintable)
class PANGAEA_API AProjectile : public AActor
{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//	弾のパラメータ
	UPROPERTY(EditAnywhere,Category="Projectile Params")
	float Speed = 100.0f;
	UPROPERTY(EditAnywhere,Category="Projectile Params")
	float Lifespan=5.0f;
	UPROPERTY(EditAnywhere,Category="Projectile Params")
	float Damage = 10.0f;
	//	ゲッター関数
	FORCEINLINE UStaticMeshComponent* GetMesh() const{ return MeshComponent; }
	UFUNCTION(BlueprintCallable)
	virtual void Reset() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float LifeCountingDown;
	class APangaeaGameMode* GameMode;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess="true"))
	UStaticMeshComponent* MeshComponent;
	
};
