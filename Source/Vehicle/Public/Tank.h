// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "Tank.generated.h"


class UChaosWheeledVehicleMovementComponent;
class ATankShell;
class USkeletalMeshComponent;

/**
 * 
 */
UCLASS()
class VEHICLE_API ATank : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	//Several IA to be set in editor - idk accessing it from IMC, which is set in the player controller blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAround;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookUpAndDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Throttle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Break;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* HandBreak;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* Shooting;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Setup the player input componenet
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	//Call for Lookingaround (Yaw)
	void LookAroundEvent(const FInputActionValue& Value);

	//Call for up and down looking (Pitch)
	void LookUpAndDownEvent(const FInputActionValue& Value);

	//Call for throttle
	void ThrottleEvent(const FInputActionValue& Value);

	//Call for breaking
	void BreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType);

	//Call for using handbreak
	void HandBreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType);

	//Call for shooting
	void ShootingEvent(const FInputActionValue& Value);




	UPROPERTY(BlueprintReadWrite)
	UChaosWheeledVehicleMovementComponent* VehicleMoveComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase>TankShootCameraShake;

	UPROPERTY(EditAnywhere)
	//ATankShell* TankShellProjectile;
	TSubclassOf<ATankShell> TankShellProjectile;

	UPROPERTY()
	USkeletalMeshComponent* TankSkeletonMesh;

};
