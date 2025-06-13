// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
void UEnemyAnimInstance::OnStateAnimationEnds()
{
	//	攻撃中
	if (State == EEnemyState::Attack)
	{
		State = EEnemyState::Locomotion;
	}
	else
	{
		auto ownerActor = this->GetOwningActor();
		auto enemy = Cast<AEnemy>(ownerActor);
		
		//	nullチェック
		if (enemy == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("PlayerAvataAnimInstance playerAvatar nullptr"));
			return;
		}

		//	攻撃当たった時の処理
		if (State == EEnemyState::Hit)
		{
			if (!enemy->IsKilled())
			{
				State = EEnemyState::Locomotion;
			}
			else
			{
				State = EEnemyState::Die;
			}
		}
		//	死亡の時
		else if (State == EEnemyState::Die)
		{
			enemy->DieProcess();
		}
	}
}
