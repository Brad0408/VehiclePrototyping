// Fill out your copyright notice in the Description page of Project Settings.



#include "Tank.h"
#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TankShell.h"
#include "NiagaraFunctionLibrary.h"



ATank::ATank()
{
    
   //VehicleMoveComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetMovementComponent());
    VehicleMoveComponent = FindComponentByClass<UChaosWheeledVehicleMovementComponent>();

    TankSkeletonMesh = FindComponentByClass<USkeletalMeshComponent>();
    
    TankBackSpringArm = FindComponentByClass<USpringArmComponent>();
}


void ATank::BeginPlay()
{
	Super::BeginPlay();

    //FOnTimelineFloat ShootingProgressUpdate;
    //ShootingProgressUpdate.BindUFunction(this, FName("TankShootTimeLineUpdate"));

    //FOnTimelineEvent ShootingFinishedEvent;
    //ShootingFinishedEvent.BindUFunction(this, FName("TankShootTimeLineFinished"));

    //TankShootingTimeLine.AddInterpFloat(TankShootingCurvefloat, ShootingProgressUpdate);
    //TankShootingTimeLine.SetTimelineFinishedFunc(ShootingFinishedEvent);



    if (TankBackSpringArm)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("Skeleton Component: %s")));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("NO CAMERA %s")));
    }



}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    
    if(TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {   
        //Set in all these IA in the editor
        if (LookAround)
        {
            //Call LookAroundEvent
			EnhancedInputComponent->BindAction(LookAround, ETriggerEvent::Triggered, this, &ATank::LookAroundEvent);
        }


        if (LookUpAndDown)
        {
            //Call LookUpAndDownEvent
            EnhancedInputComponent->BindAction(LookUpAndDown, ETriggerEvent::Triggered, this, &ATank::LookUpAndDownEvent);
        }

        if (Throttle)
        {
            //Call Throttle
            EnhancedInputComponent->BindAction(Throttle, ETriggerEvent::Triggered, this, &ATank::ThrottleEvent);
            EnhancedInputComponent->BindAction(Throttle, ETriggerEvent::Completed, this, &ATank::ThrottleEvent);
        }

        if (Break)
        {
            //Call Break
            EnhancedInputComponent->BindAction(Break, ETriggerEvent::Triggered, this, &ATank::BreakEvent, ETriggerEvent::Triggered);
            EnhancedInputComponent->BindAction(Break, ETriggerEvent::Completed, this, &ATank::BreakEvent, ETriggerEvent::Completed);
        }

        if (HandBreak)
        {
            //Call HandBreak
            EnhancedInputComponent->BindAction(HandBreak, ETriggerEvent::Triggered, this, &ATank::HandBreakEvent, ETriggerEvent::Triggered);
            EnhancedInputComponent->BindAction(HandBreak, ETriggerEvent::Completed, this, &ATank::HandBreakEvent, ETriggerEvent::Completed);
        }

        if (Shooting)
        {
            //Call Shooting
            EnhancedInputComponent->BindAction(Shooting, ETriggerEvent::Triggered, this, &ATank::ShootingEvent);
        }

        if (Steering)
        {
            //Call Steering
            EnhancedInputComponent->BindAction(Steering, ETriggerEvent::Triggered, this, &ATank::SteeringEvent);
            EnhancedInputComponent->BindAction(Steering, ETriggerEvent::Completed, this, &ATank::SteeringEvent);
        }

        if (ScrollIn)
        {
            //Call Camera Scroll In
            EnhancedInputComponent->BindAction(ScrollIn, ETriggerEvent::Triggered, this, &ATank::CameraZoomInEvent);
        }
    }
}

//Adds Yaw 
void ATank::LookAroundEvent(const FInputActionValue &Value)
{	
	float LookAroundValue = Value.Get<float>();

	AddControllerYawInput(LookAroundValue);
}



//Adds pitch
void ATank::LookUpAndDownEvent(const FInputActionValue& Value)
{
    //Inverse to work properly
    float LookUpAndDownValue = -1.0f * Value.Get<float>();

    AddControllerPitchInput(LookUpAndDownValue);
}


//Adds Throttle to VehicleMovementComponent
void ATank::ThrottleEvent(const FInputActionValue& Value)
{
    float ThrottleValue = Value.Get<float>();

    VehicleMoveComponent->SetThrottleInput(ThrottleValue);
}


//Applies Break to VehicleMovementComponent
void ATank::BreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType)
{
    //If its being triggered set to the actual value of it
    if (TriggerEventType == ETriggerEvent::Triggered)
    {
        float BreakValue = Value.Get<float>();

        VehicleMoveComponent->SetBrakeInput(BreakValue);
    }
    else
    {
        //Basically for EventCompleted, just fully break the tank
        VehicleMoveComponent->SetBrakeInput(0.0f);
    }
}


//Applies Break to VehicleMovementComponent
void ATank::HandBreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType)
{
    //If triggered set hand break true
    if (TriggerEventType == ETriggerEvent::Triggered)
    {
        VehicleMoveComponent->SetHandbrakeInput(true);
    }
    else
    {
        //Any other input Enum set it to false
        VehicleMoveComponent->SetHandbrakeInput(false);
    }
}

void ATank::TankShootTimeLineUpdate(float Alpha)
{
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("UPDATE IS CALLED SET TO FALSE")));
    TankFired = false;

}

void ATank::TankShootTimeLineFinished()
{
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("FINISHED WAS CALLED")));
    FLatentActionInfo LatentInfo;
    UKismetSystemLibrary::Delay(GetWorld(), 1.2f, LatentInfo);
    TankFired = true;
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("FINISHED CALLED SET TO TRUE")));
}


//Deals with Shooting TankShells
void ATank::ShootingEvent(const FInputActionValue& Value)
{

    if (TankFired == true)
    {   
        //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT(" Fired ")));
        
        //Get World for spawning actors
        UWorld* World = GetWorld();

        if (TankShootCameraShake)
        {
            FVector EpicenterValue = GetActorLocation();

            //Makes the camera shake when tank fires
            UGameplayStatics::PlayWorldCameraShake(GetWorld(), TankShootCameraShake, EpicenterValue, 0.0f, 10000.0f, 1.0f, false);
        }
        
        //Get the transform point of main_cannon, which is located at the end of the tanks main turret barrel
        FTransform TankShootPointTransform = TankSkeletonMesh->GetSocketTransform("main_cannon", RTS_World);

        FVector TankShootPointLocation = TankShootPointTransform.GetLocation();
        FRotator TankShootPointRotation = TankShootPointTransform.Rotator();

        //Check if TankShellProjectile was set in the editor
        if (TankShellProjectile)
        {
            //Spawn the tank projectile at the end of the tank barrel which is set in the editor and store this here
            AActor* SpawnedTankShellProjectile = World->SpawnActor<ATankShell>(TankShellProjectile, TankShootPointTransform);
        }


        //Check VFX is assigned
        if (ShootingVFX)
        {
            //Play VFX at location
            UNiagaraFunctionLibrary::SpawnSystemAttached(ShootingVFX, TankSkeletonMesh, "main_cannon", TankShootPointLocation, TankShootPointRotation, EAttachLocation::KeepWorldPosition, false, true, ENCPoolMethod::None, true);;
        }


        //Check sound is assigned
        if (TankShootSound)
        {
            //Play sound cue at location
            UGameplayStatics::PlaySoundAtLocation(this, TankShootSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, TankShootAttenuation);
        }

        //Check FF is assigned
        if (ForceFeedbackEffect)
        {
            //If PC is valid
            if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
            {
                //Create FFbackparameter struct ref
                FForceFeedbackParameters ForceFeedbackParams;

                //Set the variables in struct
                ForceFeedbackParams.Tag = FName(TEXT(""));
                ForceFeedbackParams.bLooping = false;

                //Play force feedback using struct data and effect set in editor
                PlayerController->ClientPlayForceFeedback(ForceFeedbackEffect, ForceFeedbackParams);

            }

        }

        //Get Transform of the tanks turret
        FTransform TankTurretTransform = TankSkeletonMesh->GetSocketTransform("turret_jnt", RTS_World);

        //Set the the direction recoil of the turret - Give the effect of the turret going back into itself when it fires
        FVector TurretRecoilDirection(0.0f, -50.0f, 0.0f);

        //Calculate a torque
        FVector TankTurretRecoilTorque = UKismetMathLibrary::TransformDirection(TankTurretTransform, TurretRecoilDirection);

        //Actually update the visuals of the tank with the recoil effect
        TankSkeletonMesh->AddTorqueInRadians(TankTurretRecoilTorque, "cog_jnt", true);


        //TankShootingTimeLine.PlayFromStart();
    }
}


//Steers the Tank
void ATank::SteeringEvent(const FInputActionValue& Value)
{
    float SteeringValue = Value.Get<float>();

    VehicleMoveComponent->SetYawInput(SteeringValue);
}

void ATank::CameraZoomInEvent(const FInputActionValue& Value)
{
    //Get the arm length
    float TargetArmLength = TankBackSpringArm->TargetArmLength;

    // Decrease the arm length by 100 units
    TargetArmLength -= 100.0f;

    // Clamp the arm length between 800 and 2500
    TargetArmLength = FMath::Clamp(TargetArmLength, 800.0f, 2500.0f);

    // Set the new arm length
    TankBackSpringArm->TargetArmLength;
}