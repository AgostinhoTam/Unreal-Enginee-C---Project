// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "Enemy.h"
#include "Navigation/PathFollowingComponent.h"

void AEnemyController::MakeAttackDecision(APawn* targetPawn)
{
	if (!targetPawn)return;

	auto controlledCharacter = Cast<AEnemy>(GetPawn());
	if (controlledCharacter)
	{
		//	距離計算
		auto dist = FVector::Dist(targetPawn->GetActorLocation(), controlledCharacter->GetActorLocation());
		//	攻撃範囲内と攻撃可能の場合
		if (dist < controlledCharacter->AttackRange && controlledCharacter->CanAttack())
		{
			//	攻撃開始したら動き止めるとクールダウン開始
			StopMovement();
			controlledCharacter->Attack_Broadcast_RPC();
		}
		else if (dist > controlledCharacter->AttackRange + 20.0f)
		{
			//	もう追跡状態だったら処理しない
			if (GetMoveStatus() != EPathFollowingStatus::Moving)	
			{
				controlledCharacter->Chase(targetPawn);
			}
		}
	}
}
