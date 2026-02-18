// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/ShootComponent.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/Component/MaskComponent.h"
#include "Camera/CameraComponent.h"
#include "Utility.h"
// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	Init();
	// ...
	
}


// Called every frame
void UShootComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShootComponent::Init()
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

void UShootComponent::Shoot()
{
	LOG("Shoot");
	if (!canShoot || !character || !character->GetCamera())return;
	const FVector& _start = character->GetCamera()->GetComponentLocation();
	const FVector& _end = character->GetCamera()->GetForwardVector() * 10000 + _start;
	FHitResult _result;
	bool _hit = UKismetSystemLibrary::SphereTraceSingle(WORLD, _start, _end, 10.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, TArray<AActor*>(), EDrawDebugTrace::ForOneFrame, _result, true);

	if (!_hit)
	{
		bulletFX(_start, _end);
		return;
	}
	bulletFX(_start, _result.ImpactPoint);


	//TODO do damage and effect

}

void UShootComponent::bulletFX(FVector _start, FVector _end)
{



	//UNiagaraFunctionLibrary::SpawnSystemAttached(slideFX, character->GetRootComponent(), "M_Spine_01_jnt", FVector(0.0f), FRotator(0.0f), EAttachLocation::KeepRelativeOffset, true)




}

void UShootComponent::FireFX(FVector _pos)
{

	//UNiagaraFunctionLibrary::SpawnSystemAttached(slideFX, character->GetRootComponent(), "M_Spine_01_jnt", FVector(0.0f), FRotator(0.0f), EAttachLocation::KeepRelativeOffset, true)


}

