// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "EnhancedInputSubsystems.h" 
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputAction> ScrollIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputAction> ScrollOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TObjectPtr<UInputAction> ToggleCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	//Setup the player input component
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	//Call for Looking Around (Yaw)
	void LookAroundEvent(const FInputActionValue& Value);

	//Call for up and down looking (Pitch)
	void LookUpAndDownEvent(const FInputActionValue& Value);

	//Call for throttle
	void ThrottleEvent(const FInputActionValue& Value);

	//Call for breaking
	void BreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType);

	//Call for using Hand break
	void HandBreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType);

	//Call for shooting
	void ShootingEvent(const FInputActionValue& Value);

	//Call for steering
	void SteeringEvent(const FInputActionValue& Value);

	//Call for camera zooming in
	void CameraZoomInEvent(const FInputActionValue& Value);

	//Call for camera zooming out
	void CameraZoomOutEvent(const FInputActionValue& Value);

	//Call to toggle between 1st and 3rd person camera
	void CameraToggleEvent(const FInputActionValue& Value);

	//Declare the VehicleMoveComponent
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UChaosWheeledVehicleMovementComponent> VehicleMoveComponent;

	//Declare the tanks shooting state
	UPROPERTY(BlueprintReadWrite)
	bool bTankFired = true;

	//Declare which camera is active
	UPROPERTY(BlueprintReadWrite)
	bool bIsBackCameraActive = true;

	//Declare the tanks speed limit
	UPROPERTY(BlueprintReadWrite)
	float SpeedLimit = 50.0f;

	//Declare if the tank is reversing
	UPROPERTY(BlueprintReadWrite)
	bool bIsReversing = false;


	//Declare the screen shake
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase>TankShootCameraShake;

	//Declare the projectile to shoot
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATankShell> TankShellProjectile;

	//Declare the tank track decals 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> TrackDecals;


	//Declare the Tanks Skeleton Mesh
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> TankSkeletonMesh;

	//Declare the tanks spring arm
	UPROPERTY()
	TObjectPtr<USpringArmComponent> TankFrontSpringArm;

	//Declare the tanks spring arm
	UPROPERTY()
	TObjectPtr<USpringArmComponent> TankBackSpringArm;

	//Declare the 1st person front camera
	TObjectPtr<UCameraComponent> FrontCamera;

	//Declare the 3rd person back camera
	TObjectPtr<UCameraComponent> BackCamera;

	//Declare the left wheel dust component
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> LeftWheelDust;

	//Declare the right wheel dust component
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> RightWheelDust;

	//Declare the tank track audio component
	UPROPERTY()
	TObjectPtr<UAudioComponent> TankTrackAudio;

	//Declare the engine audio component
	UPROPERTY()
	TObjectPtr<UAudioComponent> TankEngineAudio;

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

	//Declare function to set the tanks max speed
	UFUNCTION()
	void SetMaxForwardSpeed();

	//Declare function to set the tanks max speed
	UFUNCTION()
	void SetMaxReverseSpeed();

	//Declare function to set the location of the tanks wheel dust
	UFUNCTION()
	void SetWheelDustLocation(bool bIsTankReversing);

	//Declare function that will spawn in track decals under the wheels
	UFUNCTION()
	void SpawningDecalTracks();

	//Declare function that will control the scale of dust size
	UFUNCTION()
	void DustScaleEffects();

	//Declare funciton to manipulate the engine sounds
	UFUNCTION()
	void EngineSounds(UChaosVehicleWheel* Wheel, bool bIsTankReversing);

	//Declare function to 'animate' the tank tracks
	UFUNCTION()
	void TrackAnimations(UChaosVehicleWheel* Wheel, bool bIsTankReversing);

};
