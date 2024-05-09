// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/HitResult.h>
#include "MPSHWeaponBase.generated.h"

UCLASS()
class MPSHOOTER_API AMPSHWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMPSHWeaponBase();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly);
	class USkeletalMeshComponent* WeaponMeshComponent;

	UFUNCTION(Server, WithValidation, Reliable)
	void StartShooting();

	UFUNCTION(Server, WithValidation, Reliable)
	void StopShooting();

	UFUNCTION(Server, WithValidation, Reliable)
	void ThrowLineTrace();

	UFUNCTION(Client, Unreliable)
	void DrawEffects(FVector Startlocation, FVector EndLocation, FVector HitPoint);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyShootDamage(const FHitResult& HitResult);
};
