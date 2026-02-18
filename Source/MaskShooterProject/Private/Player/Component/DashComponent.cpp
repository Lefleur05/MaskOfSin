// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/DashComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Component/MaskComponent.h"
#include "Camera/CameraComponent.h"
#include "Utility.h"

// Sets default values for this component's properties
UDashComponent::UDashComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	Init();
}


// Called every frame
void UDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDashComponent::Init()
{
	character = Cast<APlayerCharacter>(GetOwner());
	if (!character)
	{
		LOG_ERROR("No Character in DashComponent");
		return;
	}

	movement = character->GetCharacterMovement();
	if (!movement)
	{
		LOG_ERROR("No Character Movement in DashComponent");
		return;
	}
	maskComponent = character->GetMaskComponent();
	if (!maskComponent)
	{
		LOG_ERROR("No Character maskComponent in DashComponent");
		return;
	}

}

void UDashComponent::DashBase()
{
	if (dashCount >= maskComponent->GetCurrentMaskData().maxDash)return;
	dashCount++;
	FVector2D _lastMovementInput = character->GetLastMovementInput();

	if (_lastMovementInput.Length() <= 0.0f)
	{
		DashNoSpeed(false);
		return;
	}

	FVector _launch = (character->GetActorForwardVector() * _lastMovementInput.Y) + (character->GetActorRightVector() * _lastMovementInput.X);
	_launch *= maskComponent->GetCurrentMaskData().dashPower;
	_launch = FVector(_launch.X, _launch.Y, 0.0f);
	character->LaunchCharacter(_launch, true, true);
	TIMER_LOOP(WORLD, timerDashCooldown, UDashComponent::DashRefill(), maskComponent->GetCurrentMaskData().dashCooldown, 0.01f);
	LOG("DashBase");
}

void UDashComponent::DashWithZ()
{
	if (dashCount >= maskComponent->GetCurrentMaskData().maxDashWithZ)return;
	dashCount++;
	FVector2D _lastMovementInput = character->GetLastMovementInput();

	if (_lastMovementInput.Length() <= 0.0f)
	{
		DashNoSpeed(true);
		return;
	}

	FVector _launch = (character->GetCamera()->GetForwardVector() * _lastMovementInput.Y) + (character->GetCamera()->GetRightVector() * _lastMovementInput.X);
	_launch *= maskComponent->GetCurrentMaskData().dashPower;
	character->LaunchCharacter(_launch, true, true);
	if (!GetWorld()->GetTimerManager().IsTimerActive(timerDashCooldown))
		TIMER_LOOP(WORLD, timerDashCooldown, UDashComponent::DashRefill(), maskComponent->GetCurrentMaskData().dashCooldown, 0.01f);
	LOG("DashWithZ");
}

void UDashComponent::DashNoSpeed(bool _zLaunch)
{
	FVector _launch = character->GetCamera()->GetForwardVector() * maskComponent->GetCurrentMaskData().dashPower;
	if (!_zLaunch)
		_launch = FVector(_launch.X, _launch.Y, 0.0f);
	character->LaunchCharacter(_launch, true, true);
	TIMER_LOOP(WORLD, timerDashCooldown, UDashComponent::DashRefill(), maskComponent->GetCurrentMaskData().dashCooldown, 0.01f);
	//LOG("Dash No Speed");
	return;
}

void UDashComponent::DashRefill()
{
	if (!movement || !movement->IsMovingOnGround())return;
	dashCount= 0.0f;
	CLEAR_TIMER(timerDashCooldown);
}

void UDashComponent::Dash()
{
	if (!maskComponent ||!maskComponent->GetCurrentMaskData().canDash || !character)return;
	if (maskComponent->GetCurrentMaskData().canDashWithZ)
	{
		DashWithZ();
		return;
	}
	DashBase();
}


