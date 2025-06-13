// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType,meta =(PrimaryAssetType="Weapon"))
class PANGAEA_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,meta =(MetaClass="Weapon",AssetBundles="Weapon")) 
	TSoftClassPtr<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float Strength =10.0f;

};
