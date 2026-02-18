// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Player/Component/MaskComponent.h"
#include "MaskDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MASKSHOOTERPROJECT_API UMaskDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere) EMaskColor maskColor;

	UPROPERTY(EditAnywhere, Category = "Movement") float maxWalkSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Dash") bool canDash = true;
	UPROPERTY(EditAnywhere, Category = "Dash") float dashPower = 2500.0f;
	UPROPERTY(EditAnywhere, Category = "Dash") float dashCooldown = 2.0f;
	UPROPERTY(EditAnywhere, Category = "Dash") bool canDashWithZ = true;
	UPROPERTY(EditAnywhere, Category = "Dash| Base", meta = (EditCondition = "!canDashWithZ", EditConditionHides)) int maxDash = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Dash| WithZ", meta = (EditCondition = "canDashWithZ", EditConditionHides)) int maxDashWithZ = 2.0f;


};
