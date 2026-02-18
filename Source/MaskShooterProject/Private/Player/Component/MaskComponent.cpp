// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/MaskComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include <Player/DataAsset/MaskDataAsset.h>
#include "Utility.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UMaskComponent::UMaskComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMaskComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	// ...
	
}


// Called every frame
void UMaskComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	redCooldown -= DeltaTime;
	greenCooldown -= DeltaTime;
	blueCooldown -= DeltaTime;
	FRAME_LOG_CUSTOM("Blue Cooldown : " + FString::SanitizeFloat(blueCooldown), FColor::Blue);
	FRAME_LOG_CUSTOM("Green Cooldown : " + FString::SanitizeFloat(greenCooldown), FColor::Green);
	FRAME_LOG_CUSTOM("Red Cooldown : " + FString::SanitizeFloat(redCooldown), FColor::Red);
	// ...
}

void UMaskComponent::Init()
{
	character = Cast<APlayerCharacter>(GetOwner());
	if (!character)
	{
		LOG_ERROR("No Character in MaskComponent");
		return;
	}

	movement = character->GetCharacterMovement();
	if (!movement)
	{
		LOG_ERROR("No Character Movement in MaskComponent");
		return;
	}

	redMaskData.SetDataAsset();
	greenMaskData.SetDataAsset();
	blueMaskData.SetDataAsset();

	SetRedMask();
	redCooldown = 0.0f;
}

void UMaskComponent::SwitchToRedMask()
{
	currentMaskColor = EMaskColor::MC_RED;
	currentMaskData = redMaskData;
	onCurrentMaskSwitch.Broadcast(currentMaskColor);
}

void UMaskComponent::SwitchToGreenMask()
{
	currentMaskColor = EMaskColor::MC_GREEN;
	currentMaskData = greenMaskData;
	onCurrentMaskSwitch.Broadcast(currentMaskColor);
}

void UMaskComponent::SwitchToBlueMask()
{
	currentMaskColor = EMaskColor::MC_BLUE;
	currentMaskData = blueMaskData;
	onCurrentMaskSwitch.Broadcast(currentMaskColor);

}

void UMaskComponent::UpdateValueSwitchMask()
{
	if (movement)
	{
		movement->MaxWalkSpeed = currentMaskData.maxWalkSpeed;
	}
}

// Red Mask
void UMaskComponent::SetRedMask()
{
	if (currentMaskColor == EMaskColor::MC_RED)return;
	if (redCooldown > 0.0f)return;
	SetSwitchCooldownMask();
	SwitchToRedMask();
	UpdateValueSwitchMask();
}

//Green Mask
void UMaskComponent::SetGreenMask()
{
	if (currentMaskColor == EMaskColor::MC_GREEN)return;
	if (greenCooldown > 0.0f)return;
	SetSwitchCooldownMask();
	SwitchToGreenMask();
	UpdateValueSwitchMask();
}

//Blue Mask
void UMaskComponent::SetBlueMask()
{
	if (currentMaskColor == EMaskColor::MC_BLUE)return;
	if (blueCooldown > 0.0f)return;
	SetSwitchCooldownMask();
	SwitchToBlueMask();
	UpdateValueSwitchMask();
}

void UMaskComponent::SetSwitchCooldownMask(bool _causeByDamage)
{
	float _cooldown = initCooldown;
	if (_causeByDamage)
		_cooldown += redCountCooldown * damageCooldownToAdd;

	switch (currentMaskColor)
	{
	case EMaskColor::MC_NONE:
		break;
	case EMaskColor::MC_RED:
		redCooldown = _cooldown;
		if (_causeByDamage)
			redCountCooldown++;
		break;
	case EMaskColor::MC_GREEN:
		greenCooldown = _cooldown;
		if (_causeByDamage)
			greenCountCooldown++;
		break;
	case EMaskColor::MC_BLUE:
		blueCooldown = _cooldown;
		if (_causeByDamage)
			blueCountCooldown++;
		break;
	case EMaskColor::MC_MAX:
		break;
	default:
		break;
	}
}

void UMaskComponent::TakeDamage()
{
	onTakeDamage.Broadcast();
	SetSwitchCooldownMask(true);
	if (greenCooldown > 0.0f && blueCooldown > 0.0f)
	{
		Death();
		return;
	}

	switch (currentMaskColor)
	{
	case EMaskColor::MC_NONE:
		break;
	case EMaskColor::MC_RED:
		if (greenCooldown <= 0.0f)
			SwitchToGreenMask();
		else if (blueCooldown <= 0.0f)
			SwitchToBlueMask();
		break;
	case EMaskColor::MC_GREEN:
		if (blueCooldown <= 0.0f)
			SwitchToBlueMask();
		else if (redCooldown <= 0.0f)
			SwitchToRedMask();
		break;
	case EMaskColor::MC_BLUE:
		if (redCooldown <= 0.0f)
			SwitchToRedMask();
		else if (greenCooldown <= 0.0f)
			SwitchToGreenMask();
		break;
	case EMaskColor::MC_MAX:
		break;
	default:
		break;
	}
}

void UMaskComponent::Death()
{
	onDeath.Broadcast();
	FName _nameLevel = FName(WORLD->GetName());
	UGameplayStatics::OpenLevel(WORLD, _nameLevel);
}

void UMaskComponent::DecreaseByDamage(EMaskColor _ennemyMaskColor)
{
	switch (_ennemyMaskColor)
	{
	case EMaskColor::MC_NONE:
		break;
	case EMaskColor::MC_RED:
		redCooldown--;
		break;
	case EMaskColor::MC_GREEN:
		greenCooldown--;
		break;
	case EMaskColor::MC_BLUE:
		blueCooldown--;
		break;
	case EMaskColor::MC_MAX:
		break;
	default:
		break;
	}
}

void FMaskDataStruct::SetDataAsset()
{
	if (!maskDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("FMaskDataStruct::SetDataAsset => No Mask DataAsset"));
		return;
	}
	maskColor = maskDataAsset->maskColor;

	//Movement
	maxWalkSpeed = maskDataAsset->maxWalkSpeed;

	//Dash
	canDash = maskDataAsset->canDash;
	canDashWithZ = maskDataAsset->canDashWithZ;
	dashPower = maskDataAsset->dashPower;
	dashCooldown = maskDataAsset->dashCooldown;
	maxDash = maskDataAsset->maxDash;
	maxDashWithZ = maskDataAsset->maxDashWithZ;
}
