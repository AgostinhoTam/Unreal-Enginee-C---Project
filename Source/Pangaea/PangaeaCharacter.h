// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PangaeaCharacter.generated.h"

UCLASS(Blueprintable)
class APangaeaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APangaeaCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	
	//	キャラクターの共通パラメータ
	UPROPERTY(EditAnywhere, Category = "Character Params")
	int MaxHealth = 500; // MaxHealth
	UPROPERTY(EditAnywhere, Category = "Character Params")
	float Strength = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Character Params")
	float Armor = 3.0f;
	UPROPERTY(EditAnywhere, Category = "Character Params")
	float AttackRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Character Params")
	float AttackInterval = 1.2f;

	//	体力ゲージ
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	UUserWidget* HealthBarWidget;
	
	//	ゲッター関数
	//	現在体力
	UFUNCTION(BlueprintCallable, Category="Pangaea | Params", meta=(DisplayName="Get HP"))
	FORCEINLINE float GetHealthPoints() { return CurrentHealth; }
	//	攻撃力
	UFUNCTION(BlueprintCallable,Category="Pangaea | Params")
	FORCEINLINE float GetStrength() { return Strength; }

	//	判定フラグ
	UFUNCTION(BlueprintCallable, Category="Pangaea | Params")
	bool IsKilled();
	UFUNCTION(BlueprintCallable, Category="Pangaea | Params")
	bool CanAttack();
	
	//	メイン処理
	virtual void Attack();
	virtual void Hit(int Damage);
	virtual void DieProcess();
	bool IsAttacking();
	
	UFUNCTION(NetMulticast, Reliable)
	void Attack_Broadcast_RPC();

	
protected:
	virtual void BeginPlay() override;
	
	//	ネット更新
	UPROPERTY(ReplicatedUsing = OnHealthPointsChanged)
	int CurrentHealth;
	UFUNCTION()
	void OnHealthPointsChanged();
	
	float AttackCountingDown = 0;
	class UPangaeaAnimInstance* AnimInstance;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
