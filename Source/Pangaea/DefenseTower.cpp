// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseTower.h"

#include "PangaeaCharacter.h"
#include "PlayerAvatar.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ADefenseTower::ADefenseTower()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	_SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	_SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	_SphereComponent->SetGenerateOverlapEvents(true);
	_SphereComponent->SetSphereRadius(800.0f);
	SetRootComponent(_SphereComponent);

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	_MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	_MeshComponent->SetupAttachment(_SphereComponent);

	//	ファイヤーボール生成
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Fireball.BP_Fireball'"));
	//static ConstructorHelpers::FClassFinder<AProjectile> fireball_BP(TEXT("/Game/TopDown/Blueprints/BP_Fireball"));
	//_FireballClass = fireball_BP.Class;
	_FireballClass = (UClass*)blueprint_finder.Object->GeneratedClass;
}

// Called when the game starts or when spawned
void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(0.5f);	//	0.5秒で更新
	_SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ADefenseTower::OnBeginOverlap);
}

// Called every frame
void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_Target != nullptr)
	{
		Fire();
	}
}

int ADefenseTower::GetHealthPoints()
{
	return _HealthPoints;
}

bool ADefenseTower::IsDestroyed()
{
	return (_HealthPoints > 0.0f);
}

bool ADefenseTower::CanFire()
{
	return (_ReloadCountingDown <= 0.0f);
}

void ADefenseTower::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerAvatar* player = Cast<APlayerAvatar>(OtherActor);
	if (player)
	{
		_Target = player;
	}
}

void ADefenseTower::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (_Target != nullptr && OtherActor == _Target)
	{
		_Target = nullptr;
	}
}

void ADefenseTower::OnMeshBeginOverlap(AActor* OtherActor)
{
	//	武器キャスト
	AWeapon* weapon = Cast<AWeapon>(OtherActor);
	if (weapon == nullptr || weapon->Holder == nullptr)
	{
		return;
	}
	//	プレイヤーが持つ武器の場合
	APlayerAvatar* character = Cast<APlayerAvatar>(weapon->Holder);
	if (character && character->IsAttacking() && CanBeDamaged())
	{
		UE_LOG(LogTemp, Display, TEXT("Player's　Weapon Damaged"));
		Hit(weapon->Strength);
	}
}

void ADefenseTower::Fire()
{
	//	ファイヤーぼる生成
	auto fireball = Cast<AProjectile>(GetWorld()->SpawnActor(_FireballClass));
	//	発射位置
	FVector startLocation = GetActorLocation();
	startLocation.Z += 100.0f;
	//	目標位置
	FVector targetLocation = _Target->GetActorLocation();
	targetLocation.Z = startLocation.Z;
	//	向き
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLocation);
	fireball->SetActorLocation(startLocation);
	fireball->SetActorRotation(rotation);
}

void ADefenseTower::Hit(int damage)
{
	_HealthPoints -= damage;
	UE_LOG(LogTemp, Warning, TEXT("Tower Hit: %d"), _HealthPoints);
	if (_HealthPoints <= 0)
	{
		Destroy();
	}
}

