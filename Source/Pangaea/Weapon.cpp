// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "DefenseTower.h"
#include "PangaeaGameMode.h"
#include "PlayerAvatar.h"
#include "Components/MeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));
	MeshComp->SetGenerateOverlapEvents(true);
	SetRootComponent(MeshComp);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBeginOverlap);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	拾われていない時は回転する
	if (Holder == nullptr && HasAuthority())
	{
		FQuat rotQuat = FQuat(FRotator(0.0f, 300.0f * DeltaTime, 0.0f));
		AddActorLocalRotation(rotQuat);
	}
}

//	攻撃と拾う時の処理　(OverlappedActorはエンジン側がthisになるので、OtherActorは別のActor)
void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	auto character = Cast<APangaeaCharacter>(OtherActor);
	//	対象物がキャラクターの場合
	if (character != nullptr)
	{
		//	この武器は誰にも持っていないなら
		if (Holder == nullptr)
		{
			//	プレイヤーなら拾う処理
			auto playerAvatar = Cast<APlayerAvatar>(character);
			if (playerAvatar != nullptr)
			{
				playerAvatar->DropWeapon();
				playerAvatar->AttachWeapon(this);
			}
		}
		//	他のアクターへの当たり判定処理（characterは所有者じゃない時）
		else if (character != Holder && character->CanBeDamaged() && Holder->IsAttacking() && IsWithinAttackRange(
			0.0f, OtherActor))
		{
			character->Hit(Strength);
			//	キャラクターの処理の判定
			if (character->IsA(APlayerAvatar::StaticClass()))
			{
				if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red,TEXT("Hit PlayerAvatar"));
			}
			else
			{
				if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Cyan,TEXT("Hit Enemy"));
			}
		}
	}
	else if (Holder != nullptr && Holder->IsA(APangaeaCharacter::StaticClass()) && Holder->IsAttacking())
	{
		auto tower = Cast<ADefenseTower>(OtherActor);
		if (tower != nullptr && tower->CanBeDamaged() && IsWithinAttackRange(0.0f, tower))
		{
		if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan,TEXT("Hit Tower"));
			tower->Hit(Strength);
		}
	}
}

bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target)
{
	return (AttackRange <= 0.0f || AttackRange >= GetDistanceTo(Target));
}

void AWeapon::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 対象物はキャラクターの場合
	APangaeaCharacter* character = Cast<APangaeaCharacter>(OtherActor);
	if (character != nullptr)
	{
		//	武器はまだオーナーがいない時
		if (Holder == nullptr)
		{
			//	武器装着
			APlayerAvatar* playerAvatar = Cast<APlayerAvatar>(character);
			if (playerAvatar != nullptr)
			{
				playerAvatar->DropWeapon();
				playerAvatar->AttachWeapon(this);
			}
		}
		else if (character != Holder && IsWithinAttackRange(0.0f,OtherActor) && character->CanBeDamaged() && Holder->IsAttacking())	//　オーナーがいる時、かつ自分じゃない
		{
			character->Hit(Strength);
			if (character->IsA(APlayerAvatar::StaticClass()))
			{
				if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red,TEXT("Hit PlayerAvatar"));
			}
			else
			{
				if (GEngine)GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Cyan,TEXT("Hit Enemy"));
			}
		}
	}
	//	キャラクター以外の場合
	else if (Holder !=nullptr && Holder->IsA(APangaeaCharacter::StaticClass()) && Holder->IsAttacking())
	{
		auto tower = Cast<ADefenseTower>(OtherActor);
		if (tower != nullptr && tower->CanBeDamaged() && IsWithinAttackRange(0.0f, tower))
		{
			tower->Hit(Strength);
			if (GEngine)GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("Hit Tower"));
		}
	}
}
