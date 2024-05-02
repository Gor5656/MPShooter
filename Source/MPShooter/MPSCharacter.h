// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "MPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

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

	UFUNCTION(BlueprintPure)
	float GetMovementDirection();

};
