// Fill out your copyright notice in the Description page of Project Settings.


#include "MPSHHealthComponent.h"
#include <Net/UnrealNetwork.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values for this component's properties
UMPSHHealthComponent::UMPSHHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


void UMPSHHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	isDead = false;
}


void UMPSHHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	DOREPLIFETIME(UMPSHHealthComponent, CurrentHealth);
	DOREPLIFETIME(UMPSHHealthComponent, isDead);
}

void UMPSHHealthComponent::SetHealth(float NewHealth)
{
	if (NewHealth < 0) return;
	
	CurrentHealth = UKismetMathLibrary::Clamp(NewHealth, 0.f, MaxHealth);

	if (CurrentHealth <= 0)
	{
		isDead = true;
		OnDeath.Broadcast();
	}
}

// Called every frame
void UMPSHHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

