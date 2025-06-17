// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera",meta =(AllowPrivateAccess = "true"))
	class USpringArmComponent* _springArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Camera",meta =(AllowPrivateAccess = "true"))
	class UCameraComponent* _cameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* _aiPerceptionStimuliSourceComponent;

public:
	// Sets default values for this character's properties
	APlayerAvatar();

	//	プライヤーのパラメータ
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	int HealthPoints = 500;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float Strength =10.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float Armor = 3.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float AttackRange = 6.0f;
	UPROPERTY(EditAnywhere, Category = "PlayerAvatar Params")
	float AttackInterval = 1.2f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int _HealthPoints;
	float _AttackCountingDown;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//	ゲッター関数
	UFUNCTION(BlueprintCallable, Category = "Pangaea | PlayerCharacter",meta=(DisplayName="GetHP"))
	FORCEINLINE int GetHealthPoints();
	FORCEINLINE USpringArmComponent* GetSpringArm() const{ return _springArmComponent; }
	FORCEINLINE UCameraComponent* GetCamera() const{ return _cameraComponent; }

	//	メイン処理
	UFUNCTION(BlueprintCallable, Category = "Pangaea | PlayerCharacter")
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | PlayerCharacter")
	bool CanAttack();
	UFUNCTION(BlueprintCallable, Category = "Pangaea | PlayerCharacter")
	bool IsAttacking();
	void Attack();
	void Hit(int damage);
	void DieProcess();
};
