// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAvataAnimInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerState:uint8
{
	Locomotion,
	Attack,
	Hit,
	Die
};

UCLASS()
class PANGAEA_API UPlayerAvataAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerState _State;
	UFUNCTION(BlueprintCallable)
	void OnStateAnimationEnds();
};
