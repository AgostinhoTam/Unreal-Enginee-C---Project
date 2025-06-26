// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADefenseTower();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//	ゲッター関数
	UFUNCTION(BlueprintPure, Category = "Pangaea|Defense Tower", meta=(DisplayName="GetHP"))
	FORCEINLINE int GetHealthPoints();
	FORCEINLINE USphereComponent* GetSphereComponent() const { return SphereComponent; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return MeshComponent; }


	//	タワーのパラメータ
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int MaxHealthPoints = 500;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	int ShellDefense = 3;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float AttackRange = 6.0f;
	UPROPERTY(EditAnywhere, Category = "Tower Params")
	float ReloadInterval = 1.0f;

	//	メイン処理
	UFUNCTION(BlueprintPure, Category="Pangaea|Defense Tower")
	bool IsDestroyed();
	UFUNCTION(BlueprintPure, Category="Pangaea|Defense Tower")
	bool CanFire();
	UFUNCTION() //	どの当たり判定が発火、対象Actor、他のアクターの当たり判定、他のアクターの体のインデックス
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                    UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	                  int32 OtherBodyIndex);

	void Fire();
	void Hit(int damage);
	bool IsKilled();
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta =(AllowPrivateAccess="true"))
	USphereComponent* SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess="true"))
	UStaticMeshComponent* MeshComponent;
	class APlayerAvatar* Target;
	UClass* FireballClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int CurrentHealthPoints;
	float ReloadCountingDown;
	class APangaeaGameMode* Gamemode;
};
