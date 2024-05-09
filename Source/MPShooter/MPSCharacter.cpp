// Fill out your copyright notice in the Description page of Project Settings.


#include "MPSCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <GameFramework/SpringArmComponent.h>
#include "Weapons/MPSHWeaponBase.h"
#include <Engine/EngineTypes.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "Components/MPSHHealthComponent.h"

// Sets default values
AMPSCharacter::AMPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UMPSHHealthComponent>("HealthComponent");
	HealthComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AMPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SpawnandAttachWeapon();

	HealthComponent->OnDeath.AddDynamic(this, &AMPSCharacter::OnDeath);
}

// Called every frame
void AMPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMPSCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMPSCharacter::Look);

		EnhancedInputComponent->BindAction(SlowingDownAction, ETriggerEvent::Triggered, this, &AMPSCharacter::StartSlowingDown);
		EnhancedInputComponent->BindAction(SlowingDownAction, ETriggerEvent::Completed, this, &AMPSCharacter::StopSlowingDown);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AMPSCharacter::StartShooting);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &AMPSCharacter::StopShooting);
	}

}

void AMPSCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMPSCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMPSCharacter::StartSlowingDown(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SlowingDownSpeed;
		SlowingDown_Server(SlowingDownSpeed);
	}
}

void AMPSCharacter::StopSlowingDown(const FInputActionValue& Value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		SlowingDown_Server(NormalSpeed);
	}
}

void AMPSCharacter::StartShooting(const FInputActionValue& Value)
{
	Shoot_Server(true);
}

void AMPSCharacter::StopShooting(const FInputActionValue& Value)
{
	Shoot_Server(false);
}

void AMPSCharacter::SpawnandAttachWeapon()
{
	if (IsValid(WeaponClass))
	{
		FTransform SpawnTransform = GetMesh()->GetSocketTransform(WeaponSocketName, ERelativeTransformSpace::RTS_Actor);
		CurrentWeapon = GetWorld()->SpawnActor<AMPSHWeaponBase>(WeaponClass, SpawnTransform);
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	}
}

float AMPSCharacter::GetMovementDirection()
{
	if (GetVelocity().IsZero()) return 0.f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AMPSCharacter::SlowingDown_Server_Implementation(float NewSpeed)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
	}
}

bool AMPSCharacter::SlowingDown_Server_Validate(float NewSpeed)
{
	return true;
}

void AMPSCharacter::Shoot_Server_Implementation(bool isStart)
{
	if (IsValid(CurrentWeapon))
	{
		if (isStart)
		{
			CurrentWeapon->StartShooting();
		}
		else
		{
			CurrentWeapon->StopShooting();
		}
	}
}

bool AMPSCharacter::Shoot_Server_Validate(bool isStart)
{
	return true;
}

float AMPSCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	FString Name = FString::SanitizeFloat(DamageAmount);
	UKismetSystemLibrary::PrintString(this, Name, true, true, FLinearColor::Red, 2.f);
	HealthComponent->SetHealth(HealthComponent->CurrentHealth - DamageAmount);
	return 0.f;
}

void AMPSCharacter::OnDeath()
{
	UKismetSystemLibrary::PrintString(this, "Meeeerrrraaavvvv", true, true, FLinearColor::Black);

}

