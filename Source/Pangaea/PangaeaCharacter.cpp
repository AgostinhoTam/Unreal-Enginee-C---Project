// Copyright Epic Games, Inc. All Rights Reserved.

#include "PangaeaCharacter.h"

#include "HealthBarWidget.h"
#include "PangaeaAnimInstance.h"
#include "Components/ProgressBar.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

APangaeaCharacter::APangaeaCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void APangaeaCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Tick animation instance nullptr"))
		return;
	}

	//	速度計算(アニメーション用)
	AnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();

	//	同じの場合は攻撃State
	if (AttackCountingDown == AttackInterval)
	{
		AnimInstance->State = ECharacterState::Attack;
	}
	//	攻撃クールダウン
	if (AttackCountingDown > 0.0f)
	{
		AttackCountingDown -= DeltaSeconds;
	}
}

void APangaeaCharacter::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	if (GetNetMode() == NM_ListenServer)
	{
		Destroy();
	}
}

bool APangaeaCharacter::IsAttacking()
{
	return (AnimInstance->State == ECharacterState::Attack);
}

void APangaeaCharacter::Attack_Broadcast_RPC_Implementation()
{
	Attack();
}

void APangaeaCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APangaeaCharacter, CurrentHealth);
}

void APangaeaCharacter::OnHealthPointsChanged()
{
	if (HealthBarWidget != nullptr)
	{
		//	比率計算
		float normalizeHealth = FMath::Clamp((float)CurrentHealth / MaxHealth, 0.0f, 1.0f);
		auto healthBar = Cast<UHealthBarWidget>(HealthBarWidget);
		//	UIに反映
		if (healthBar)healthBar->HealthBar->SetPercent(normalizeHealth);
	}
	if (AnimInstance != nullptr)
	{
		AnimInstance->State = ECharacterState::Hit;
	}
	if (IsKilled())
	{
		PrimaryActorTick.bCanEverTick = false;
	}
}

void APangaeaCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	AnimInstance = Cast<UPangaeaAnimInstance>(GetMesh()->GetAnimInstance());
}

bool APangaeaCharacter::IsKilled()
{
	return (CurrentHealth <= 0);
}

bool APangaeaCharacter::CanAttack()
{
	//	クールダウン終わったら　&& 待機モード中
	return (AttackCountingDown <= 0.0f && AnimInstance->State == ECharacterState::Locomotion);
}

void APangaeaCharacter::Attack()
{
	//	クールダウン開始
	AttackCountingDown = AttackInterval;
}

void APangaeaCharacter::Hit(int Damage)
{
	if (IsKilled())return;
	//	もしホストなら
	if (GetNetMode() == NM_ListenServer && HasAuthority())
	{
		//	ダメージリアクション
		CurrentHealth -= Damage;
		OnHealthPointsChanged();
	}
}
