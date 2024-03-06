// Fill out your copyright notice in the Description page of Project Settings.


#include "TankDecoy.h"

ATankDecoy::ATankDecoy()
{
	Health = 1.0f;
}

void ATankDecoy::BeginPlay()
{	
	Super::BeginPlay();

	// Bind the HandleAnyDamageReceived function to the ReceiveAnyDamage event
	OnTakeAnyDamage.AddDynamic(this, &ATankDecoy::ReceiveAnyDamage);
}

void ATankDecoy::ReceiveAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("damaged%")));
	}

	ModifyHealth(Damage);
}

void ATankDecoy::ModifyHealth(float Damage)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("funcalled%")));
	}

	if (Health > 0.0f)
	{
		Health = Health - Damage;

		if (Health <= 0.0f)
		{
			Destroy();

			//OnTankDestroyed();

		}
	}
}

