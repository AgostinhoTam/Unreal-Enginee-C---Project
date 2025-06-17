// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "EnemyController.h"
#include "PlayerAvatar.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WeaponData.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//	コンポーネント生成
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("UAISense"));
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//	Perception登録
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	//	武器読み込み
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Hammer.BP_Hammer'"));
	_WeaponClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	_HealthPoints = HealthPoints;
	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	//	イベントバインド
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdate);

	//	武器生成&ソケットに設置
	_Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(_WeaponClass));
	_Weapon->Holder = this;
	_Weapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetIncludingScale,FName("hand_rSocket"));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//	早期リターン
	if (!EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Tick animation instance nullptr"))
	}

	//	速度計算(アニメーション用)
	EnemyAnimInstance->Speed = GetCharacterMovement()->Velocity.Size2D();

	//	攻撃開始
	if (_AttackCountingDown == AttackInterval)
	{
		EnemyAnimInstance->State = EEnemyState::Attack;
	}
	//	クールダウン時間処理
	if (_AttackCountingDown > 0.0f)
	{
		_AttackCountingDown -= DeltaTime;
	}

	//	待機モードの時とターゲットがない時
	if (_ChasedTarget != nullptr && EnemyAnimInstance->State == EEnemyState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(_ChasedTarget);
	}
}

int AEnemy::GetHealthPoints() const
{
	return _HealthPoints;
}

bool AEnemy::IsKilled() const
{
	return (_HealthPoints <= 0);
}

bool AEnemy::CanAttack() const
{
	if (!EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's CanAttack animation instance nullptr"));
		return false;
	}
	//	クールダウン終わったらかつ待機モード
	return (_AttackCountingDown <= 0.0f && EnemyAnimInstance->State == EEnemyState::Locomotion);
}

void AEnemy::Chase(APawn* targetPawn)
{
	if (!EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Chase animation instance nullptr"));
		return;
	}
	if (targetPawn != nullptr && EnemyAnimInstance->State == EEnemyState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MoveToActor(targetPawn, 90.0f);
	}
	_ChasedTarget = targetPawn;
}

float AEnemy::GetSightRadius() const
{
	return (SightConfig->SightRadius);
}

float AEnemy::GetPeripheralVisionHalfAngle() const
{
	return SightConfig->PeripheralVisionAngleDegrees;
}

void AEnemy::Attack()
{
	//	攻撃開始したら動き止めるとクールダウン開始
	GetController()->StopMovement();
	_AttackCountingDown = AttackInterval;
}

void AEnemy::Hit(int damage)
{
	if (!EnemyAnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Hit animation instance nullptr"));
		return;
	}
	//	体力減らす
	_HealthPoints -= damage;
	EnemyAnimInstance->State = EEnemyState::Hit;
	//	死亡判定
	if (IsKilled())
	{
		DieProcess();
	}
}

void AEnemy::DieProcess()
{
	PrimaryActorTick.bCanEverTick = false;
	Destroy();
}

void AEnemy::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Display, TEXT("Perception update called"));
	if (APlayerAvatar* player = Cast<APlayerAvatar>(Actor))
	{
		UE_LOG(LogTemp, Display, TEXT("player"));
		//	感知成功
		if (Stimulus.WasSuccessfullySensed())
		{
			Chase(player);
		}
	}
}

void AEnemy::HandleWeaponData()
{
	
}
