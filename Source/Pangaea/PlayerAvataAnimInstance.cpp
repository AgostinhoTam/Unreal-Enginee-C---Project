// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvataAnimInstance.h"
#include "PlayerAvatar.h"


void UPlayerAvataAnimInstance::OnStateAnimationEnds()
{
	//	攻撃中
	if (_State == EPlayerState::Attack)
	{
		_State = EPlayerState::Locomotion;
	}
	else
	{
		auto ownerActor = this->GetOwningActor();
		auto playerAvatar = Cast<APlayerAvatar>(ownerActor);
		//	nullチェック
		if (playerAvatar == nullptr)
		{
			UE_LOG(LogTemp,Error,TEXT("PlayerAvataAnimInstance playerAvatar nullptr"));
			return;
		}

		//	攻撃当たった時の処理
		if (_State == EPlayerState::Hit)
		{
			if (!playerAvatar->IsKilled())
			{
				_State = EPlayerState::Locomotion;
			}
			else
			{
				_State = EPlayerState::Die;
			}
		}
		//	死亡の時
		else if (_State == EPlayerState::Die)
		{
			playerAvatar->DieProcess();
		}
	}
}
