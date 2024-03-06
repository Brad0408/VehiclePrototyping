// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "GameFramework/Actor.h"
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


	//Cannot override in Actor.h - not virtual function 
	// Instead just used EventAnyDamage in the BP itself
	// 
	//void ReceiveAnyDamage(float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



	//Works with pawns but tank shell is UGameplayStatics::ApplyRadialDamageWithFalloff which only effects actors
	// Could work if tags were used in TankShell.cpp , not sure just sticking with EventAnyDamage
	// 
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
protected:

	//Declare a way to modify health - Call after EventAnyDamage
	UFUNCTION(BlueprintCallable)
	void ModifyHealth(float Damage);

};
