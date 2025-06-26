// Fill out your copyright notice in the Description page of Project Settings.


#include "PangaeaAnimInstance.h"
#include "PangaeaCharacter.h"

void UPangaeaAnimInstance::OnStateAnimationEnds()
{
	//	攻撃終了処理
	if (State == ECharacterState::Attack)
	{
		State = ECharacterState::Locomotion;
	}
	else
	{
		auto character = Cast<APangaeaCharacter>(GetOwningActor());
		if (character == nullptr || !character->HasAuthority())return;
		UE_LOG(LogTemp, Display, TEXT("OnStateAnimationEnds"));
		//	ヒット処理
		if (State == ECharacterState::Hit)
		{
			if (character->GetHealthPoints() > 0.0f)
			{
				character->SetCanBeDamaged(true);
				State = ECharacterState::Locomotion;
			}
			else	//	死亡判定
			{
				State = ECharacterState::Die;
			}
		}
		else if (State == ECharacterState::Die)	//	死んだら死亡処理
		{
			character->DieProcess();
		}
	}
}
