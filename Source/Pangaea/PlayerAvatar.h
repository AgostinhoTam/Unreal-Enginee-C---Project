// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PangaeaCharacter.h"
#include "PlayerAvatar.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API APlayerAvatar : public APangaeaCharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera",meta =(AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Camera",meta =(AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* AIPerceptionStimuliSourceComponent;

public:
	// Sets default values for this character's properties
	APlayerAvatar();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//	ゲッター関数
	FORCEINLINE USpringArmComponent* GetSpringArm() const{ return SpringArmComponent; }
	FORCEINLINE UCameraComponent* GetCamera() const{ return CameraComponent; }

	//	メイン処理
	void AttachWeapon(class AWeapon* Weapon);
	void DropWeapon();
	UFUNCTION(Server,Reliable)
	void Attack_RPC();
};
