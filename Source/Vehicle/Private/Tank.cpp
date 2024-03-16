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
    
  
}


void ATank::BeginPlay()
{
	Super::BeginPlay();
    


    //Iterate through all attached child components of TankSkeletonMesh
    for (USceneComponent* ChildComponent : TankSkeletonMesh->GetAttachChildren())
    {

        //Names of the spring arm you want to find
        FName BackSpringArmNameToFind(TEXT("SpringArm"));
        FName FrontSpringArmNameToFind(TEXT("SpringArm1"));

        //Names of the niagara components
        FName LeftDustToFind(TEXT("BLeftWheelDust"));
        FName RightDustToFind(TEXT("BRightWheelDust"));

        //Names of the audio components
        FName TrackCueToFind(TEXT("Tank_Tracks_Cue"));
        FName EngineCueToFind(TEXT("Engine_Idle_Cue"));

        /////////////For find the back spring arm and the camera attached to that spring arm/////////////////
        //Check if the child component's name matches the spring arm name
        if (ChildComponent->GetName() == BackSpringArmNameToFind.ToString())
        {

            //Set found spring arm
            TankBackSpringArm = Cast<USpringArmComponent>(ChildComponent);
            if (TankBackSpringArm)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("back spring found")));
                //Found a spring arm component, now iterate through its attached children for the camera
                for (USceneComponent* SpringArmChild : TankBackSpringArm->GetAttachChildren())
                {
 
                    //Check if the child component is a camera component
                    if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(SpringArmChild))
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("back camera found")));
                        //Set the found camera to the back camera
                        BackCamera = CameraComponent;

                        break;
                    }
                }
            }
        }


        /////////////For find the front spring arm and the camera attached to that spring arm/////////////////
        if (ChildComponent->GetName() == FrontSpringArmNameToFind.ToString())
        {

            //Set found spring arm
            TankFrontSpringArm = Cast<USpringArmComponent>(ChildComponent);
            if (TankFrontSpringArm)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("front spring found")));
                //Found a spring arm component, now iterate through its attached children for the camera
                for (USceneComponent* SpringArmChild : TankFrontSpringArm->GetAttachChildren())
                {

                    //Check if the child component is a camera component
                    if (UCameraComponent* CameraComponent = Cast<UCameraComponent>(SpringArmChild))
                    {
                        //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("front camera found")));
                        //Set the found camera to the back camera
                        FrontCamera = CameraComponent;

                        break;
                    }
                }
            }
        }



        //Check for the dust components
        if (ChildComponent->GetName() == LeftDustToFind.ToString())
        {

            //Check if the child component is a Niagara Particle System Component
            LeftWheelDust = Cast<UNiagaraComponent>(ChildComponent);
            if (LeftWheelDust)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("left dust found")));
            }
        }


        if (ChildComponent->GetName() == RightDustToFind.ToString())
        {

            // Check if the child component is a Niagara Particle System Component
            RightWheelDust = Cast<UNiagaraComponent>(ChildComponent);
            if (RightWheelDust)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("right dust found")));
            }
        }

        if (ChildComponent->GetName() == TrackCueToFind.ToString())
        {

            //Found the spring audio components
            TankTrackAudio = Cast<UAudioComponent>(ChildComponent);
            if (TankTrackAudio)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("track audio found")));
            }
        }


        if (ChildComponent->GetName() == EngineCueToFind.ToString())
        {

            //Found the spring audio components
            TankEngineAudio = Cast<UAudioComponent>(ChildComponent);
            if (TankEngineAudio)
            {
                //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("engine audio found")));
            }
        }




        //Final check to see if all values on the tank in the blueprint have been found and are valid
        if (TankBackSpringArm && TankFrontSpringArm && RightWheelDust && LeftWheelDust && TankTrackAudio && TankEngineAudio)
        {   
            GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("ALL COMPONENTNS FOUND")));
            break;
        }
    }



}

void ATank::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    //Create an array to store all the tank wheels
    TArray<UChaosVehicleWheel*> TankWheels;

    //Get the tank wheels and iterate through each one
    for (TObjectPtr<UChaosVehicleWheel>& WheelPtr : VehicleMoveComponent->Wheels)
    {
        //Add each returned wheel to the array
        TankWheels.Add(WheelPtr.Get());
    }

    //Just getting random wheel, in this case the 3 one - doesnt matter which wheel they are all rotating the same 
    TObjectPtr<UChaosVehicleWheel> FourthWheel = TankWheels[3];

    if (FourthWheel)
    {
        float WheelRotationSpeed = FourthWheel->GetRotationAngularVelocity();


        //Doing this to check if reversing rather than if throttle or break was inputted is 
        //because the tank can still roll after all inputs are stopped, so if the player lightly taps the breaks but is still moving forward doing this way
        //means any visual updates based on direction are actually accurate
        if (WheelRotationSpeed > 100)
        {
            bIsReversing = true;
        }
        else
        {
            bIsReversing = false;
        }
    }

    //Set the wheel dust based of if the tank is the reversing
    SetWheelDustLocation(bIsReversing);

    //Spawns Decal tracks on the ground
    SpawningDecalTracks();
    
    //Changes the engine pitch based of speed
    EngineSounds(FourthWheel, bIsReversing);

    //Changes tank audio based of pitch
    TrackSounds(bIsReversing);
    
    //'Animates' tank tracks
    TrackAnimations(FourthWheel, bIsReversing);
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

        if (ScrollOut)
        {
            //Call Camera Scroll In
            EnhancedInputComponent->BindAction(ScrollOut, ETriggerEvent::Triggered, this, &ATank::CameraZoomOutEvent);
        }

        if (ToggleCamera)
        {
            //Call Camera Scroll In
            EnhancedInputComponent->BindAction(ToggleCamera, ETriggerEvent::Triggered, this, &ATank::CameraToggleEvent);
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

    //Sets speed limit
    SetMaxForwardSpeed();

    //Sets dust scale
    DustScaleEffects();
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

    //Sets speed limit
    SetMaxReverseSpeed();

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

//Deals with Shooting TankShells
void ATank::ShootingEvent(const FInputActionValue& Value)
{

    if (bTankFired == true)
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
            //Spawn the tank projectile at the end of the tank barrel which is set in the editor
            World->SpawnActor<ATankShell>(TankShellProjectile, TankShootPointTransform);
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


//Zooms the camera in based on clamped values
void ATank::CameraZoomInEvent(const FInputActionValue& Value)
{
    //Get the current arm length
    float CurrentArmLength = TankBackSpringArm->TargetArmLength;

    float NewArmLength = CurrentArmLength - 100.0f;

    //Clamp length
    NewArmLength = FMath::Clamp(NewArmLength, 800.0f, 2500.0f);
    
    //Set the clamped arm length
    TankBackSpringArm->TargetArmLength = NewArmLength;
}


//Zooms the camera out based on clamped values
void ATank::CameraZoomOutEvent(const FInputActionValue& Value)
{
    //Get the current arm length
    float CurrentArmLength = TankBackSpringArm->TargetArmLength;

    float NewArmLength = CurrentArmLength + 100.0f;

    //Clamp length
    NewArmLength = FMath::Clamp(NewArmLength, 800.0f, 2500.0f);

    //Set the clamped arm length
    TankBackSpringArm->TargetArmLength = NewArmLength;
}


//Toggles the camera between 1st and 3rd person
void ATank::CameraToggleEvent(const FInputActionValue& Value)
{
    //Deactivate both cameras
    FrontCamera->Deactivate();
    BackCamera->Deactivate();


    //Replicating the flip flop node
    //Toggle the camera state
    bIsBackCameraActive = !bIsBackCameraActive;

    //Activate the appropriate camera based on the state
    if (bIsBackCameraActive)
    {
        BackCamera->Activate();
    }
    else
    {
        FrontCamera->Activate();
    }
}

//Stops the tank from going over 50 km/h
void ATank::SetMaxForwardSpeed()
{
    float TankForwardSpeed = VehicleMoveComponent->GetForwardSpeed();

    if(TankForwardSpeed * 0.036 > SpeedLimit)
    {
        VehicleMoveComponent->SetThrottleInput(0.0);
    }
}

//Stop the tank from going over 50 km/h when reversing
void ATank::SetMaxReverseSpeed()
{
    float TankForwardSpeed = VehicleMoveComponent->GetForwardSpeed();

    float TankBackSpeed = TankForwardSpeed * -1.0;
    
    if(TankBackSpeed * 0.036 > SpeedLimit)
    {
        VehicleMoveComponent->SetBrakeInput(0.0);
    }
}


//Sets the location of the wheel dust 
void ATank::SetWheelDustLocation(bool bIsTankReversing)
{
    //Create vectors to store the location of the niagara components
    FVector LeftWheelDustLocation;
    FVector RightWheelDustLocation;

    //If tank is reversing set the wheel dust to the front of the tank
    if (bIsTankReversing == true)
    {
        //Set the location of the wheel dust based of the wheel joint names
        LeftWheelDustLocation = TankSkeletonMesh->GetSocketLocation("lf_wheel_2_jnt");
        RightWheelDustLocation = TankSkeletonMesh->GetSocketLocation("rt_wheel_2_jnt");

        //Weird offset calculation to make the dust in the right location
        LeftWheelDustLocation.Y -= 20.0f;
        RightWheelDustLocation.Y -= 20.0f;

        //Set the tanks niagara component to this location in the world
        LeftWheelDust->SetWorldLocation(LeftWheelDustLocation);
        RightWheelDust->SetWorldLocation(RightWheelDustLocation);
    }
    else
    {
        //Set the location of the wheel dust based of the wheel joint names
        LeftWheelDustLocation = TankSkeletonMesh->GetSocketLocation("lf_wheel_7_jnt");
        RightWheelDustLocation = TankSkeletonMesh->GetSocketLocation("rt_wheel_7_jnt");

        //Set the tanks niagara component to this location in the world
        LeftWheelDust->SetWorldLocation(LeftWheelDustLocation);
        RightWheelDust->SetWorldLocation(RightWheelDustLocation);
    }

   
}


//Controls the scale of dust particle effects based on the tanks speed
void ATank::DustScaleEffects()
{
    FVector StartTraceLocation = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
    FVector EndTraceLocation = StartTraceLocation - FVector(0.0f, 0.0f, 100.0f);
    ETraceTypeQuery TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);
    FHitResult HitResult;

    bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartTraceLocation, EndTraceLocation, TraceChannel, false, TArray<AActor*>(), EDrawDebugTrace::None, HitResult, true);

    if (bHit)
    {
        float SpeedValue = VehicleMoveComponent->GetForwardSpeedMPH();

        //Clamp the values
        float DustParticleClampedValue = UKismetMathLibrary::MapRangeClamped(SpeedValue, 0.0f, 100.0f, 0.0f, 2000.0f);
        float WheelSpeedClampedValue = UKismetMathLibrary::MapRangeClamped(SpeedValue, 0.0f, 100.0f, 0.0f, 125.0f);

        //Set the scale based of the clamped values
        LeftWheelDust->SetFloatParameter("DustParticleScale", DustParticleClampedValue);
        RightWheelDust->SetFloatParameter("DustParticleScale", DustParticleClampedValue);

        LeftWheelDust->SetFloatParameter("WheelSpeed", WheelSpeedClampedValue);
        RightWheelDust->SetFloatParameter("WheelSpeed", WheelSpeedClampedValue);

    }
    else
    {
        //If the tank is in the air dont bother showing the dust particles
        LeftWheelDust->SetFloatParameter("DustParticleScale", 0.0f);
        RightWheelDust->SetFloatParameter("DustParticleScale", 0.0f);

        LeftWheelDust->SetFloatParameter("WheelSpeed", 0.0f);
        RightWheelDust->SetFloatParameter("WheelSpeed", 0.0f);
    }

}


//Spawns Decal tracks on the ground
void ATank::SpawningDecalTracks()
{
    //The decal spawn location is based on where the Niagara components are located - which are moved around in SetWheelDustLocation()
    FVector LeftTrackLocation = LeftWheelDust->GetComponentLocation();
    FVector RightTrackLocation = RightWheelDust->GetComponentLocation();

    //Left Track Decals
    UGameplayStatics::SpawnDecalAtLocation(GetWorld(), TrackDecals, FVector(100.0f, 100.0f, 100.f), LeftTrackLocation, GetActorRotation(), 3.0f);


    //Right Track Decals
    UGameplayStatics::SpawnDecalAtLocation(GetWorld(), TrackDecals, FVector(100.0f, 100.0f, 100.f), RightTrackLocation, GetActorRotation(), 3.0f);
}


//Changes the pitch of the engine sound effect based of the speed
void ATank::EngineSounds(UChaosVehicleWheel* Wheel, bool bIsTankReversing)
{
    //Wheel velocity is used as a float value to modify the pitch value of the engine
    float WheelVelocity = Wheel->GetRotationAngularVelocity();

    if (bIsReversing == true)
    {
        TankEngineAudio->SetFloatParameter("RPM", WheelVelocity);
    }
    else
    {
        float ReverseWheelVelocity = WheelVelocity * -1.0f;
        TankEngineAudio->SetFloatParameter("RPM", ReverseWheelVelocity);
    }
}

//Chanes volume of track audio based of speed
void ATank::TrackSounds(bool bIsTankReversing)
{
    //Wheel velocity is used as a float value to modify the pitch value of the engine
    float TankSpeed = VehicleMoveComponent->GetForwardSpeedMPH();

    if (bIsReversing == true)
    {
        float ReverseTankSpeed = TankSpeed * -1.0f;
        TankTrackAudio->SetFloatParameter("Speed", ReverseTankSpeed);

        float ReverseClampedTankSpeed = UKismetMathLibrary::MapRangeClamped(ReverseTankSpeed, 0.0f, 10.0f, 0.0f, 1.0f);
        TankTrackAudio->SetVolumeMultiplier(ReverseClampedTankSpeed);
    }
    else
    {
        TankTrackAudio->SetFloatParameter("Speed", TankSpeed);
        float ClampedTankSpeed = UKismetMathLibrary::MapRangeClamped(TankSpeed, 0.0f, 10.0f, 0.0f, 1.0f);
        TankTrackAudio->SetVolumeMultiplier(ClampedTankSpeed);
    }
}


//'Animates' tank tracks by offset the texture based on the wheel speed
void ATank::TrackAnimations(UChaosVehicleWheel* Wheel, bool bIsTankReversing)
{
    //Wheel velocity is used as float value to offset the tank texture
    float WheelVelocity = Wheel->GetRotationAngularVelocity();

    if (bIsReversing == true)
    {
        float ReverseWheelVelocity = WheelVelocity * -1.0f;
        TankSkeletonMesh->SetScalarParameterValueOnMaterials("OffsetV", ReverseWheelVelocity);
    }
    else
    {
        TankSkeletonMesh->SetScalarParameterValueOnMaterials("OffsetV", WheelVelocity);
    }
}

//Modify Tanks Health
void ATank::ModifyHealth(float Damage)
{
    //If > 0 
    if (Health > 0.0f)
    {
        //Actually take damage
        Health = Health - Damage;

        //If health is 0
        if (Health <= 0.0f)
        {
            //Spawn a blow up VFX
            if (TankExplosion)
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TankExplosion, GetActorLocation(), GetActorRotation());
            }

            //Get rid of the tank
            Destroy();

            //In BP a destroyed tank is spawned and UI is displayed letting the player restart the level or go back to main menu
        }
    }
    
}

