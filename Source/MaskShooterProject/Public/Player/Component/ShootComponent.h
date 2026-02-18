// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootComponent.generated.h"

class UCharacterMovementComponent;

class APlayerCharacter;
class UMaskComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASKSHOOTERPROJECT_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) TObjectPtr<APlayerCharacter> character = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UCharacterMovementComponent> movement = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UMaskComponent> maskComponent = nullptr;

	UPROPERTY(EditAnywhere) bool canShoot = true;
	UPROPERTY(EditAnywhere) float fireRate = 0.0f;

public:	
	UShootComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();

public:
	void Shoot();
	void bulletFX(FVector _start, FVector _end);
	void FireFX(FVector _pos);


		
};
