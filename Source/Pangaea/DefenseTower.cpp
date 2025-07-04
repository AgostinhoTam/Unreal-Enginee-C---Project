// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenseTower.h"

#include "PangaeaCharacter.h"
#include "PangaeaGameMode.h"
#include "PlayerAvatar.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
// Sets default values
ADefenseTower::ADefenseTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->SetSphereRadius(800.0f);
	SetRootComponent(SphereComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	MeshComponent->SetGenerateOverlapEvents(true);
	MeshComponent->SetupAttachment(SphereComponent);

	//	ファイヤーボール生成
	static ConstructorHelpers::FObjectFinder<UBlueprint> blueprint_finder(
		TEXT("Blueprint'/Game/TopDown/Blueprints/BP_Fireball.BP_Fireball'"));
	//static ConstructorHelpers::FClassFinder<AProjectile> fireball_BP(TEXT("/Game/TopDown/Blueprints/BP_Fireball"));
	//_FireballClass = fireball_BP.Class;
	FireballClass = (UClass*)blueprint_finder.Object->GeneratedClass;


}

// Called when the game starts or when spawned
void ADefenseTower::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(0.5f); //	0.5秒で更新
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this,&ADefenseTower::OnBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this,&ADefenseTower::OnEndOverlap);
	Gamemode = Cast<APangaeaGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	CurrentHealthPoints = MaxHealthPoints;
}

// Called every frame
void ADefenseTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//	ホストサーバーだけ管理
	if (Target != nullptr && CanFire() && GetNetMode() != NM_Client)
	{
		ReloadCountingDown = ReloadInterval;
		Fire();
	}
	if (ReloadCountingDown > 0)
	{
		ReloadCountingDown -= DeltaTime;
	}
}

int ADefenseTower::GetHealthPoints()
{
	return CurrentHealthPoints;
}

bool ADefenseTower::IsDestroyed()
{
	return (CurrentHealthPoints > 0.0f);
}

bool ADefenseTower::CanFire()
{
	return (ReloadCountingDown <= 0.0f);
}

void ADefenseTower::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	APlayerAvatar* player = Cast<APlayerAvatar>(OtherActor);
	if (player)
	{
		Target = player;
	}
}

void ADefenseTower::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (Target != nullptr && OtherActor == Target)
	{
		Target = nullptr;
	}
}


void ADefenseTower::Fire()
{
	//	ファイヤーぼる生成
	//auto fireball = Cast<AProjectile>(GetWorld()->SpawnActor(_FireballClass));
	auto fireball = Gamemode->SpawnOrGetFireball(FireballClass);
	//	発射位置
	FVector startLocation = GetActorLocation();
	startLocation.Z += 100.0f;
	//	目標位置
	FVector targetLocation = Target->GetActorLocation();
	targetLocation.Z = startLocation.Z;
	//	向き
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(startLocation, targetLocation);
	fireball->SetActorLocation(startLocation);
	fireball->SetActorRotation(rotation);
}

void ADefenseTower::Hit(int damage)
{
	CurrentHealthPoints -= damage;
	UE_LOG(LogTemp, Warning, TEXT("Tower Hit: %d"), CurrentHealthPoints);
	if (IsKilled())
	{
		Destory();
	}
}

bool ADefenseTower::IsKilled()
{
	return CurrentHealthPoints <=0;
}
