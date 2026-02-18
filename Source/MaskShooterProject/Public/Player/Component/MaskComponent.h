// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MaskComponent.generated.h"

class UCharacterMovementComponent;

class APlayerCharacter;
class UMaskDataAsset;


UENUM(BlueprintType)
enum class EMaskColor : uint8
{
	MC_NONE UMETA(DisplayName = "MC_NONE"),
	MC_RED UMETA(DisplayName = "MC_RED"),
	MC_GREEN UMETA(DisplayName = "MC_GREEN"),
	MC_BLUE UMETA(DisplayName = "MC_BLUE"),
	MC_MAX UMETA(DisplayName = "MC_MAX")

};

USTRUCT(BlueprintType)
struct FMaskDataStruct
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere) TObjectPtr<UMaskDataAsset> maskDataAsset = nullptr;

	UPROPERTY(EditAnywhere) EMaskColor maskColor;
	//Movement
	UPROPERTY(EditAnywhere) float maxWalkSpeed = 1000.0f;


	//Dash
	UPROPERTY(EditAnywhere) bool canDash = true;
	UPROPERTY(EditAnywhere) bool canDashWithZ = true;
	UPROPERTY(EditAnywhere) float dashPower = 2500.0f;
	UPROPERTY(EditAnywhere) float dashCooldown = 2.0f; // No data Asset
	UPROPERTY(EditAnywhere) int maxDash = 1.0f;
	UPROPERTY(EditAnywhere) int maxDashWithZ = 2.0f;


public:
	void SetDataAsset();
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASKSHOOTERPROJECT_API UMaskComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamage);
	UPROPERTY(BlueprintAssignable, BlueprintCallable) FOnTakeDamage onTakeDamage;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
	UPROPERTY(BlueprintAssignable, BlueprintCallable) FOnDeath onDeath;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentMaskSwitch, EMaskColor, _newMaskColor);
	UPROPERTY(BlueprintAssignable, BlueprintCallable) FOnCurrentMaskSwitch onCurrentMaskSwitch;


private:
	UPROPERTY(EditAnywhere) TObjectPtr<APlayerCharacter> character= nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UCharacterMovementComponent> movement = nullptr;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EMaskColor currentMaskColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FMaskDataStruct currentMaskData;
private:
	UPROPERTY(EditAnywhere) FMaskDataStruct redMaskData;
	UPROPERTY(EditAnywhere) FMaskDataStruct greenMaskData;
	UPROPERTY(EditAnywhere) FMaskDataStruct blueMaskData;

	UPROPERTY(VisibleAnywhere) float redCooldown = 0.0f;
	UPROPERTY(VisibleAnywhere) float greenCooldown = 0.0f;
	UPROPERTY(VisibleAnywhere) float blueCooldown = 0.0f;

	UPROPERTY(VisibleAnywhere) int redCountCooldown = 0.0f;
	UPROPERTY(VisibleAnywhere) int greenCountCooldown = 0.0f;
	UPROPERTY(VisibleAnywhere) int blueCountCooldown = 0.0f;

	UPROPERTY(VisibleAnywhere) float initCooldown = 20.0f;
	UPROPERTY(VisibleAnywhere) float damageCooldownToAdd = 5.0f;


	UPROPERTY(VisibleAnywhere) FTimerHandle timerRedCooldown;
	UPROPERTY(VisibleAnywhere) FTimerHandle timerGreenCooldown;
	UPROPERTY(VisibleAnywhere) FTimerHandle timerBlueCooldown;

public:
	EMaskColor GetCurrentMask() { return currentMaskColor; }
	FMaskDataStruct GetCurrentMaskData() { return currentMaskData; }
	FMaskDataStruct GetRedMask() { return redMaskData; }
	FMaskDataStruct GetGreenMask() { return greenMaskData; }
	FMaskDataStruct GetBlueMask() { return blueMaskData; }

public:	
	UMaskComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();

	//Red Mask
	void SwitchToRedMask();

	//Green Mask
	void SwitchToGreenMask();

	//Blue Mask
	void SwitchToBlueMask();

	void UpdateValueSwitchMask();

public:
	//Red Mask
	void SetRedMask();

	//Green Mask
	void SetGreenMask();

	//Blue Mask
	void SetBlueMask();

	void SetSwitchCooldownMask(bool _causeByDamage = false);

	void TakeDamage();
	void Death();
	void DecreaseByDamage(EMaskColor _ennemyMaskColor);


};
