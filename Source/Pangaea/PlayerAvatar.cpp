// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAvatar.h"

#include "PlayerAvataAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"

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
	characterMovement->bOrientRotationToMovement = true;	//	移動方向に向く
	characterMovement->RotationRate = FRotator(0, 640.0f, 0);	//	回転速度
	characterMovement->bConstrainToPlane =true;	//	平面移動
	characterMovement->bSnapToPlaneAtStart = true;	//	床につく
	
	//	カメラアーム作成
	_springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));	//	FTextは翻訳対応できる（UI、翻訳）、FName（ハッシュ化され高速、名前識別子として使える、FString普通の文字列（合成するならこれ）、TEXTで自動変更もできる）
	_springArmComponent->SetupAttachment(RootComponent);
	_springArmComponent->SetUsingAbsoluteRotation(true);
	_springArmComponent->TargetArmLength = 800.0f;
	_springArmComponent->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	_springArmComponent->bDoCollisionTest = false;

	//	カメラ作成
	_cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));	
	_cameraComponent->SetupAttachment(_springArmComponent,USpringArmComponent::SocketName);	//	アームに付ける
	_cameraComponent->bUsePawnControlRotation = false;
	
	_aiPerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionStimuliSource"));


}

// Called when the game starts or when spawned
void APlayerAvatar::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;
}

void APlayerAvatar::DieProcess()
{
	Destroy();
}

// Called every frame
void APlayerAvatar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	速度更新
	UPlayerAvataAnimInstance* animInstance = Cast<UPlayerAvataAnimInstance>(GetMesh()->GetAnimInstance());
	animInstance->_Speed = GetCharacterMovement()->Velocity.Size2D();

	//	攻撃発動したらAttackStateに遷移
	if (_AttackCountingDown == AttackInterval)
	{
		animInstance->_State = EPlayerState::Attack;
	}
	if (_AttackCountingDown >0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}
}

// Called to bind functionality to input
void APlayerAvatar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

int APlayerAvatar::GetHealthPoints()
{
	return _HealthPoints;
}

bool APlayerAvatar::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APlayerAvatar::CanAttack()
{
	UPlayerAvataAnimInstance* animInstance = Cast<UPlayerAvataAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInstance == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerAvatar Instance nullptr"));
	}
	//	待機モードかつクールダウン終わったら
	return (_AttackCountingDown <= 0.0f && animInstance->_State == EPlayerState::Locomotion);
}

bool APlayerAvatar::IsAttacking()
{
	UPlayerAvataAnimInstance* animInstance = Cast<UPlayerAvataAnimInstance>(GetMesh()->GetAnimInstance());
	if (animInstance == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("PlayerAvatar Instance nullptr"));
		return false;
	}
	return (animInstance->_State == EPlayerState::Attack);
}

void APlayerAvatar::Attack()
{
	//	タイマーリセット
	_AttackCountingDown = AttackInterval;
}

void APlayerAvatar::Hit(int damage)
{
	_HealthPoints -= damage;
	UE_LOG(LogTemp,Display,TEXT("HealthPoints%d"),_HealthPoints);
	
	if (_HealthPoints <= 0.0f)
	{
	}
}

