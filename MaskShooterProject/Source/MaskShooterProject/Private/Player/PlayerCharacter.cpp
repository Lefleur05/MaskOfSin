// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Player/Component/MaskComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include "Camera/CameraComponent.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "Player/Component/DashComponent.h"
#include "Player/Component/ShootComponent.h"
#include "Utility.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	//springArm = CreateDefaultSubobject<USpringArmComponent>("springArm");
	camera = CreateDefaultSubobject<UCameraComponent>("camera");
	fpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("fpsMesh");

	maskComponent = CreateDefaultSubobject<UMaskComponent>("MaskComponent");
	dashComponent = CreateDefaultSubobject<UDashComponent>("DashComponent");
	shootComponent = CreateDefaultSubobject<UShootComponent>("ShootComponent");
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("weaponMesh");

	//springArm->SetupAttachment(GetMesh(),"HeadSocket");
	camera->SetupAttachment(GetMesh(), "HeadSocket");
	fpsMesh->SetupAttachment(camera);
	weaponMesh->SetupAttachment(fpsMesh,"R_WeaponSocket");

	AddOwnedComponent(maskComponent);
	AddOwnedComponent(dashComponent);
	AddOwnedComponent(shootComponent);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	LOG("BeginPlay");

	if (ULocalPlayer* _localPlayer = Cast<ULocalPlayer>(GetWorld()->GetFirstLocalPlayerFromController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* _inputSystem = _localPlayer->GetSubsystem< UEnhancedInputLocalPlayerSubsystem>())
		{
			_inputSystem->AddMappingContext(mapping, 0);
		}
	}
	
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	LOG("SetupPlayerInputComponent");

	UEnhancedInputComponent* _input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	_input->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Movement);
	_input->BindAction(moveAction, ETriggerEvent::Completed, this, &APlayerCharacter::CompletedMovement);
	_input->BindAction(rotateAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Rotate);
	_input->BindAction(jumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jumping);
	_input->BindAction(jumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJumping);
	_input->BindAction(shootAction, ETriggerEvent::Started, shootComponent.Get(), &UShootComponent::Shoot);
	_input->BindAction(dashAction, ETriggerEvent::Started, dashComponent.Get(), &UDashComponent::Dash);
	_input->BindAction(redMaskAction, ETriggerEvent::Started, maskComponent.Get(), &UMaskComponent::SetRedMask);
	_input->BindAction(greenMaskAction, ETriggerEvent::Started, maskComponent.Get(), &UMaskComponent::SetGreenMask);
	_input->BindAction(blueMaskAction, ETriggerEvent::Started, maskComponent.Get(), &UMaskComponent::SetBlueMask);


}

void APlayerCharacter::Jumping()
{
	if (!canJumping)return;
	Jump();
}

void APlayerCharacter::Movement(const FInputActionValue& _value)
{
	if (!canMove)return;
	lastMovementInput = _value.Get<FVector2D>();
	const FVector& _move = GetActorForwardVector() * lastMovementInput.Y + GetActorRightVector() * lastMovementInput.X;
	AddMovementInput(_move);
}

void APlayerCharacter::CompletedMovement(const FInputActionValue& _value)
{
	lastMovementInput = _value.Get<FVector2D>();
}

void APlayerCharacter::Rotate(const FInputActionValue& _value)
{
	if (!canRotate)return;

	const FVector2D& _input = _value.Get<FVector2D>();

	const FRotator& _controllerRotation = GetControlRotation();
	const FRotator& _newView = FRotator(GetControlRotation().Pitch + _input.Y, GetControlRotation().Yaw + _input.X, GetControlRotation().Roll);
	GetController()->SetControlRotation(_newView);

	//AddControllerYawInput(_input.Y);
	//AddControllerPitchInput(_input.X);
}

