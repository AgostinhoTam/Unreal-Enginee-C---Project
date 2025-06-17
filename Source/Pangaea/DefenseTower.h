// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DefenseTower.generated.h"

UCLASS(Blueprintable)
class PANGAEA_API ADefenseTower : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta =(AllowPrivateAccess="true"))
	USphereComponent* _SphereComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tower Component", meta = (AllowPrivateAccess="true"))
	UStaticMeshComponent* _MeshComponent;
	class APlayerAvatar* _Target;
	UClass* _FireballClass;
public:	
	// Sets default values for this actor's properties
	ADefenseTower();

	//	タワーのパラメータ
	UPROPERTY(EditAnywhere,  Category = "Tower Params")
	int HealthPoints = 500;
	UPROPERTY(EditAnywhere,  Category = "Tower Params")
	int ShellDefense = 3;
	UPROPERTY(EditAnywhere,  Category = "Tower Params")
	float AttackRange = 6.0f;
	UPROPERTY(EditAnywhere,  Category = "Tower Params")
	float ReloadInterval = 1.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int _HealthPoints;
	float _ReloadCountingDown;
	void DestoryProcess();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//	ゲッター関数
	UFUNCTION(BlueprintPure,Category = "Pangaea|Defense Tower",meta=(DisplayName="GetHP"))
	FORCEINLINE int GetHealthPoints();
	FORCEINLINE USphereComponent* GetSphereComponent() const { return _SphereComponent; }
	FORCEINLINE UStaticMeshComponent* GetMesh() const { return _MeshComponent; }

	//	メイン処理
	UFUNCTION(BlueprintPure,Category="Pangaea|Defense Tower")
	 bool IsDestroyed();
	UFUNCTION(BlueprintPure,Category="Pangaea|Defense Tower")
	 bool CanFire();
	UFUNCTION()	//	どの当たり判定が発火、対象Actor、他のアクターの当たり判定、他のアクターの体のインデックス
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex);
	UFUNCTION(Blueprintcallable)
	void OnMeshBeginOverlap(AActor* OtherActor);
	void Fire();
	void Hit(int damage);
	
};
