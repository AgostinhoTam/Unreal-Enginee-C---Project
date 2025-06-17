// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "PlayerAvatar.h"
#include "Components/MeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionProfileName(TEXT("OverlapAll"));
	MeshComp->SetGenerateOverlapEvents(true);
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(100.0f);
	SetRootComponent(SphereComp);
	MeshComp->SetupAttachment(RootComponent);
	OnActorBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBeginOverlap);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	拾われていない時は回転する
	if (Holder == nullptr)
	{
		FQuat rotQuat = FQuat(FRotator(0.0f, 300.0f * DeltaTime, 0.0f));
		AddActorLocalRotation(rotQuat);
	}
}

//	攻撃と拾う時の処理　(OverlappedActorはエンジン側がthisになるので、OtherActorは別のActor)
void AWeapon::OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//	キャラじゃないならリターン
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character == nullptr)return;

	//	武器は誰にも持っていないなら
	if (Holder == nullptr)
	{
		//	プレイヤーなら
		APlayerAvatar* PlayerAvatar = Cast<APlayerAvatar>(character);
		if (PlayerAvatar != nullptr)
		{
			Holder = PlayerAvatar;

			//	プレイヤーの手に付いている武器を取得
			TArray<AActor*> attachedActors;
			OtherActor->GetAttachedActors(attachedActors, true);

			//	付いてる武器全部外す
			for (int i = 0; i < attachedActors.Num(); ++i)
			{
				attachedActors[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				attachedActors[i]->SetActorRotation(FQuat::Identity);
				if (AWeapon* weapon = Cast<AWeapon>(attachedActors[i]))
				{
					weapon->Holder = nullptr;
				}
			}
			//	武器をソケットにアタッチ
			AttachToComponent(Holder->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			                  FName("hand_rSocket"));
		}
	}
	//	プレイヤーが武器持っている時、攻撃の判定取る
	else if (IsWithinAttackRange(0.0f, OtherActor))
	{
		//	Damage
	}
}

bool AWeapon::IsWithinAttackRange(float AttackRange, AActor* Target)
{

	return false;
}
