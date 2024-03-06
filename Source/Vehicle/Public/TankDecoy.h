// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "TankDecoy.generated.h"

/**
 * 
 */
UCLASS()
class VEHICLE_API ATankDecoy : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankDecoy();

	//Create tank shell damage and radius values
	UPROPERTY(EditAnywhere, Category = "Tank Values")
	float Health;
	
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Declate EventTakeDamage
	void ReceiveAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	//Declare a way to modify health
	UFUNCTION()
	void ModifyHealth(float Damage);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTankDestroyed();
};
