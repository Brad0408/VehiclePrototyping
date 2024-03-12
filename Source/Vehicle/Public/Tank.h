// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "Components/TimelineComponent.h"
#include "Tank.generated.h"


class UChaosWheeledVehicleMovementComponent;
class ATankShell;
class UNiagaraSystem;
class UForceFeedbackEffect;
class UCurveFloat;

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
	TObjectPtr<UInputAction> LookAround;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookUpAndDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> Throttle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> Break;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> HandBreak;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> Shooting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> Steering;


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

	//Call for steering
	void SteeringEvent(const FInputActionValue& Value);



	//Declare the VehcileMoveComponent
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UChaosWheeledVehicleMovementComponent> VehicleMoveComponent;

	UPROPERTY(BlueprintReadWrite)
	bool TankFired = true;

	//Declare the screen shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase>TankShootCameraShake;

	//Declare the projectile to shoot
	UPROPERTY(EditAnywhere)
	//ATankShell* TankShellProjectile;
	TSubclassOf<ATankShell> TankShellProjectile;

	//Declare the Tanks Skeleton Mesh
	UPROPERTY();
	TObjectPtr<USkeletalMeshComponent> TankSkeletonMesh;

	//Declare NiagaraEffect to be set in editor
	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ShootingVFX;

	//Declare ExplosionSound to be set in editor
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> TankShootSound;

	//Declare ExplosionSound to be set in editor
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundAttenuation> TankShootAttenuation;

	//Declare ForceFeedback to be set in editor
	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;


	//Declare the Tanks shooting timeline
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Tank Shoot Curve")
	UCurveFloat* TankShootingCurvefloat;

	FTimeline TankShootingTimeLine;

	UFUNCTION()
	void TankShootTimeLineUpdate(float Alpha);

	UFUNCTION()
	void TankShootTimeLineFinished();

};
