// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UMaskComponent;
class UDashComponent;
class UShootComponent;

UCLASS()
class MASKSHOOTERPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	//UPROPERTY(EditAnywhere) TObjectPtr<USpringArmComponent> springArm = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UCameraComponent> camera = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<USkeletalMeshComponent> fpsMesh = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UStaticMeshComponent> weaponMesh = nullptr;

	UPROPERTY(EditAnywhere) TObjectPtr<UInputMappingContext> mapping = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> moveAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> jumpAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> rotateAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> shootAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> dashAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> redMaskAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> greenMaskAction = nullptr;
	UPROPERTY(EditAnywhere) TObjectPtr<UInputAction> blueMaskAction = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)TObjectPtr<UMaskComponent> maskComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)TObjectPtr<UDashComponent> dashComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)TObjectPtr<UShootComponent> shootComponent = nullptr;

private:
	UPROPERTY(EditAnywhere) bool canMove = true;
	UPROPERTY(EditAnywhere) bool canRotate = true;
	UPROPERTY(EditAnywhere) bool canJumping = true;


	UPROPERTY(EditAnywhere) FVector2D lastMovementInput = FVector2D(0.0f);

public:
	FVector2D GetLastMovementInput() { return lastMovementInput; }
	TObjectPtr<UCameraComponent> GetCamera() { return camera; }
	TObjectPtr<UMaskComponent> GetMaskComponent() { return maskComponent; }
	TObjectPtr<UDashComponent> GetDashComponent() { return dashComponent; }
	TObjectPtr<UShootComponent> GetShootComponent() { return shootComponent; }

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Jumping();

	void Movement(const FInputActionValue& _value);
	void CompletedMovement(const FInputActionValue& _value);

	void Rotate(const FInputActionValue& _value);

};
