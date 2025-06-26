// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"
#include "PangaeaAnimInstance.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"


// Sets default values
APlayerAvatar::APlayerAvatar()
{
	// 毎フレーム更新
	PrimaryActorTick.bCanEverTick = true;

	//	カメラ動かないので全部False
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//	キャラ移動設定
	auto characterMovement = GetCharacterMovement();
	characterMovement->bOrientRotationToMovement = true; //	移動方向に向く
	characterMovement->RotationRate = FRotator(0, 640.0f, 0); //	回転速度
	characterMovement->bConstrainToPlane = true; //	平面移動
	characterMovement->bSnapToPlaneAtStart = true; //	床につく

	//	カメラアーム作成
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	//	FTextは翻訳対応できる（UI、翻訳）、FName（ハッシュ化され高速、名前識別子として使える、FString普通の文字列（合成するならこれ）、TEXTで自動変更もできる）
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = 800.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;

	//	カメラ作成
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); //	アームに付ける
	CameraComponent->bUsePawnControlRotation = false;

	//	AIソース作成
	AIPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(
		TEXT("PerceptionStimuliSource"));
}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerAvatar::AttachWeapon(AWeapon* Weapon)
{
	if (Weapon == nullptr)return;
	Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("hand_rSocket"));
	Weapon->Holder = this;
}

void APlayerAvatar::DropWeapon()
{
	//	全武器リスト確保
	TArray<AActor*> attachedWeapons;
	GetAttachedActors(attachedWeapons,true);
	
	//	持っている武器をドロップ
	for (int i=0; i < attachedWeapons.Num(); i++)
	{
		//	情報リセット
		attachedWeapons[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		attachedWeapons[i]->SetActorRotation(FQuat::Identity);
		AWeapon* weapon = Cast<AWeapon>(attachedWeapons[i]);
		if (weapon != nullptr)
		{
			weapon->Holder = nullptr;
		}
	}
}

void APlayerAvatar::Attack_RPC_Implementation()
{
	Attack_Broadcast_RPC();
}
