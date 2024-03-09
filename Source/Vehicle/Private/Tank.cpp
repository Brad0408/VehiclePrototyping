// Fill out your copyright notice in the Description page of Project Settings.



#include "Tank.h"

#include "ChaosVehicleMovementComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Kismet/GameplayStatics.h"



ATank::ATank()
{
    
   //VehicleMoveComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetMovementComponent());
    VehicleMoveComponent = FindComponentByClass<UChaosWheeledVehicleMovementComponent>();

    TankSkeletonMesh = FindComponentByClass<USkeletalMeshComponent>();
    

}


void ATank::BeginPlay()
{
	Super::BeginPlay();


    if (TankSkeletonMesh)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("Skeleton Component: %s")));
        }
    }

}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (EnhancedInputComponent)
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
            //Call HandBreak
            EnhancedInputComponent->BindAction(Shooting, ETriggerEvent::Triggered, this, &ATank::ShootingEvent);
        }
    }
}

//Adds Yaw 
void ATank::LookAroundEvent(const FInputActionValue &Value)
{	
	float val = Value.Get<float>();

	AddControllerYawInput(val);
}



//Adds pitch
void ATank::LookUpAndDownEvent(const FInputActionValue& Value)
{
    //Inverse to work properly
    float val = -1.0f * Value.Get<float>();

    AddControllerPitchInput(val);
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
    if (TriggerEventType == ETriggerEvent::Triggered)
    {
        float BreakValue = Value.Get<float>();

        VehicleMoveComponent->SetBrakeInput(BreakValue);
    }
    else
    {
        VehicleMoveComponent->SetBrakeInput(0.0f);
    }
}


//Applies Break to VehicleMovementComponent
void ATank::HandBreakEvent(const FInputActionValue& Value, ETriggerEvent TriggerEventType)
{
    if (TriggerEventType == ETriggerEvent::Triggered)
    {
        VehicleMoveComponent->SetHandbrakeInput(true);
    }
    else
    {
        VehicleMoveComponent->SetHandbrakeInput(false);
    }
}


void ATank::ShootingEvent(const FInputActionValue& Value)
{
    bool Fired = true;

    if (Fired)
    {   
        GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT(" Fired ")));
        
        UWorld* World = GetWorld();

        if (TankShootCameraShake)
        {
            FVector EpicenterValue = GetActorLocation();

            UGameplayStatics::PlayWorldCameraShake(GetWorld(), TankShootCameraShake, EpicenterValue, 0.0f, 10000.0f, 1.0f, false);
        }
 
        //TankShellProjectile = World->SpawnActor<ATankShell>();
    }
}
