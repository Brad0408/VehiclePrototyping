// Fill out your copyright notice in the Description page of Project Settings.


#include "TankShell.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ATankShell::ATankShell()
{

	//Assign Default Values
	BaseDamage = 100.0f;
	MinimumDamage = 100.0f;
	DamageInnerRadius = 1000.0f;
	DamageOuterRadius = 600.0f;
	RadialStrength = 1500.0f;

}

void ATankShell::BeginPlay()
{   
    Super::BeginPlay();

	//Find the componenet that was already added to the blueprint
    ProjectileMovementComponent = FindComponentByClass<UProjectileMovementComponent>();

	//If the component is found
    if (ProjectileMovementComponent)
    {
		//Call stop function when it stops
        ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &ATankShell::OnProjectileStop);
    }
    else
    {
        //Projectile Movement Component is not found
        UE_LOG(LogTemp, Warning, TEXT("Projectile Movement Component not found in %s"), *GetName());
    }

}

//Do stuff when projectile stops
void ATankShell::OnProjectileStop(const FHitResult& ImpactResult)
{
	//Get Impact point of the impact
	FVector ImpactPoint = ImpactResult.ImpactPoint;

	//Get the normal of the impact
	FVector ImpactNormal = ImpactResult.ImpactNormal;

	//Rotate from the impact normal so VFX explosion spawns right way up
	FRotator RotationFromNormal = FRotationMatrix::MakeFromZ(ImpactNormal).Rotator();

	// Retrieve the hit component from the ImpactResults
	TObjectPtr<UPrimitiveComponent> HitComponent = ImpactResult.GetComponent();

	//Check sound is assigned
	if (ExplosionSound)
	{
		//Play sound cue at location
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, ImpactPoint);
	}


	//Check VFX is assigned
	if (ExplosionEffect)
	{
		//Play VFX at location
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, ImpactPoint, RotationFromNormal);
	}

	//Applies Radial Damage
	UGameplayStatics::ApplyRadialDamageWithFalloff(this, BaseDamage, MinimumDamage, ImpactPoint, DamageInnerRadius, DamageOuterRadius, 1.0f, nullptr, TArray<AActor*>(), nullptr, nullptr, ECollisionChannel::ECC_Visibility);


	//Check FF is assigned
	if (ForceFeedbackEffect)
	{
		//Get ref to player controller
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);



		//If PC is valid
		if (PlayerController)
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


	//Check if the hit componenet is valid
	if (HitComponent)
	{
		//On the hit component apply an impluse to it
		HitComponent->AddRadialImpulse(ImpactPoint, DamageOuterRadius, RadialStrength , ERadialImpulseFalloff::RIF_Constant, true);

		//Destroy after impulse
		Destroy();
	}
	else
	{
		//if false just destory itself
		Destroy();
	}
    
} 
