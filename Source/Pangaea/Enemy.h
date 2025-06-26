// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangaeaCharacter.h"
#include "Enemy.generated.h"

class AWeapon;

UCLASS()
class PANGAEA_API AEnemy : public APangaeaCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//	AI処理
	UFUNCTION(BlueprintCallable, Category="Pangaea|Enemy")
	void Chase(APawn* targetPawn);
	UFUNCTION(BlueprintCallable, Category = " AI | SightSense")
	float GetSightRadius() const;
	UFUNCTION(BlueprintCallable, Category = "AI | SightSense")
	float GetPeripheralVisionHalfAngle() const;
	
	//	メイン処理
	virtual void DieProcess() override;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	パラメータ
	APawn* ChasedTarget = nullptr;

	//	武器関連
	UClass* WeaponClass= nullptr;
	AWeapon* Weapon= nullptr;

private:
	//	AI関連
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	//	Perception更新
	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);


};
