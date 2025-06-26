// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "EnemyController.h"
#include "PangaeaAnimInstance.h"
#include "PlayerAvatar.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	//	コンポーネント生成
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("UAISense"));
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//	Perception登録
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
	//	武器読み込み
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(
		TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Hammer.BP_Hammer'"));
	WeaponClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//	AIイベントバインド
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemy::OnPerceptionUpdate);

	//	武器生成&ソケットに設置
	Weapon = Cast<AWeapon>(GetWorld()->SpawnActor(WeaponClass));
	Weapon->Holder = this;
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//	待機モードとターゲットがいる時
	if (ChasedTarget != nullptr && AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MakeAttackDecision(ChasedTarget);
	}
}

void AEnemy::Chase(APawn* targetPawn)
{
	if (!AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy's Chase animation instance nullptr"));
		return;
	}
	if (targetPawn != nullptr && AnimInstance->State == ECharacterState::Locomotion)
	{
		auto enemyController = Cast<AEnemyController>(GetController());
		enemyController->MoveToActor(targetPawn, 90.0f);
	}
	ChasedTarget = targetPawn;
}

float AEnemy::GetSightRadius() const
{
	return (SightConfig->SightRadius);
}

float AEnemy::GetPeripheralVisionHalfAngle() const
{
	return SightConfig->PeripheralVisionAngleDegrees;
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

void AEnemy::DieProcess()
{
	Super::DieProcess();
	//	持っている武器も消す
	if (Weapon)Weapon->Destroy();
}
