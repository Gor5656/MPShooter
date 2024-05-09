// Fill out your copyright notice in the Description page of Project Settings.


#include "MPSHWeaponBase.h"
#include <Components/SkeletalMeshComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "../MPSCharacter.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/DamageType.h>

// Sets default values
AMPSHWeaponBase::AMPSHWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMeshComponent->SetupAttachment(GetRootComponent());
}

void AMPSHWeaponBase::StartShooting_Implementation()
{
	ThrowLineTrace();
}

bool AMPSHWeaponBase::StartShooting_Validate()
{
	return true;
}

void AMPSHWeaponBase::StopShooting_Implementation()
{

}

bool AMPSHWeaponBase::StopShooting_Validate()
{
	return true;
}
void AMPSHWeaponBase::ThrowLineTrace_Implementation()
{	
	FHitResult HitResult;
	AMPSCharacter* Player = Cast<AMPSCharacter>(GetOwner());

	if (!IsValid(Player)) return;

	FVector StartLocation = WeaponMeshComponent->GetSocketLocation("Muzzle");
	FVector EndLocation = StartLocation + (Player->CameraComponent->GetForwardVector() * 10000.f);
	bool isHit;
	isHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECollisionChannel::ECC_PhysicsBody);
	DrawEffects(StartLocation, EndLocation, HitResult.Location);

	if (isHit)
	{
		ApplyShootDamage(HitResult);
	}
}

bool AMPSHWeaponBase::ThrowLineTrace_Validate()
{
	return true;
}

void AMPSHWeaponBase::DrawEffects_Implementation(FVector Startlocation, FVector EndLocation, FVector HitPoint)
{	
	UKismetSystemLibrary::DrawDebugSphere(GetWorld(), HitPoint, 10.f, 12.f, FLinearColor::Green, 2.f, 3.f);
	UKismetSystemLibrary::DrawDebugLine(GetWorld(), Startlocation, EndLocation, FLinearColor::Red, 2.f, 2.f);
}

void AMPSHWeaponBase::BeginPlay()
{
	Super::BeginPlay();


//	StartShooting();

}

void AMPSHWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMPSHWeaponBase::ApplyShootDamage(const FHitResult& HitResult)
{
	AMPSCharacter* DamagedCharacter = Cast<AMPSCharacter>(HitResult.GetActor());
	if (!IsValid(DamagedCharacter)) return;
	TSubclassOf<UDamageType> DamageTypeClass;
	UGameplayStatics::ApplyDamage(DamagedCharacter, 10.f, DamagedCharacter->GetController(), this, DamageTypeClass);
}

