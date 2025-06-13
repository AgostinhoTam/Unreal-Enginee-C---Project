// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class AWeapon;

UCLASS()
class PANGAEA_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//	敵ののパラメータ
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	int HealthPoints = 100;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Strength = 5.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float Armor = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Enemy Params")
	float AttackInterval = 3.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	パラメータ
	int _HealthPoints;
	float _AttackCountingDown;
	APawn* _ChasedTarget = nullptr;

	//	武器関連
	UPROPERTY(EditDefaultsOnly,Category="Weapons")	//	差し替えしやすいように
	FPrimaryAssetId _WeaponAssetId;	//	ハンマークラス保存用
	UPROPERTY(Transient)
	AWeapon* _Weapon;		//	武器
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category="Pangaea|Enemy", meta = (DisplayName = "Get HP"))
	int GetHealthPoints()const;

	UFUNCTION(BlueprintPure, Category="Pangaea|Enemy")
	bool IsKilled()const;

	UFUNCTION(BlueprintPure, Category="Pangaea|Enemy")
	bool CanAttack()const;

	UFUNCTION(BlueprintCallable,Category="Pangaea|Enemy")
	void Chase(APawn* targetPawn);

	UFUNCTION(BlueprintCallable,Category = " AI | SightSense")
	float GetSightRadius()const;

	UFUNCTION(BlueprintCallable,Category = "AI | SightSense")
	float GetPeripheralVisionHalfAngle()const;
	
	void Attack();
	void Hit(int damage);
	void DieProcess();

private:
	//	AI関連
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
	
	//	Perception更新
	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

	//	キャッシュ用
	UPROPERTY(Transient)
	class UEnemyAnimInstance* EnemyAnimInstance = nullptr;

	void HandleWeaponData();
	
};
