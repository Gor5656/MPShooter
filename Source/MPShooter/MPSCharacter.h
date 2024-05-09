// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include <UObject/NameTypes.h>
#include "MPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMPSHHealthComponent;

UCLASS()
class MPSHOOTER_API AMPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPSCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UMPSHHealthComponent* HealthComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputAction* SlowingDownAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowProtectedAccess = "true"))
	class UInputAction* ShootAction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartSlowingDown(const FInputActionValue& Value);
	void StopSlowingDown(const FInputActionValue& Value);
	void StartShooting(const FInputActionValue& Value);
	void StopShooting(const FInputActionValue& Value);

	void SpawnandAttachWeapon();

	UFUNCTION(BlueprintPure)
	float GetMovementDirection();

	UFUNCTION(Server, Reliable, WithValidation)
	void SlowingDown_Server(float NewSpeed);

	UFUNCTION(Server, Reliable, WithValidation)
	void Shoot_Server(bool isStart);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon");
	TSubclassOf<class AMPSHWeaponBase> WeaponClass;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon");
	class AMPSHWeaponBase* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon");
	FName WeaponSocketName = "RightHandSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	float SlowingDownSpeed = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	float NormalSpeed = 600;

	virtual float TakeDamage
	(
		float DamageAmount,						//
		struct FDamageEvent const& DamageEvent,	//
		AController* EventInstigator,			//
		AActor* DamageCauser					//
	) override;

	UFUNCTION()
	virtual void OnDeath();

};
