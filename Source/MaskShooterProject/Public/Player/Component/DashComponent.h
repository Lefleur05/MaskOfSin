// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DashComponent.generated.h"

class UCharacterMovementComponent;

class APlayerCharacter;
class UMaskComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASKSHOOTERPROJECT_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TObjectPtr<APlayerCharacter> character = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UCharacterMovementComponent> movement = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UMaskComponent> maskComponent = nullptr;

	UPROPERTY(VisibleAnywhere) FTimerHandle timerDashCooldown;

	UPROPERTY(VisibleAnywhere) int dashCount = 0.0f;

public:	
	UDashComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();

	void DashBase();
	void DashWithZ();
	void DashNoSpeed(bool _zLaunch);
	void DashRefill();

public:
	void Dash();

		
};
