// Fill out your copyright notice in the Description page of Project Settings.


#include "LandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
ALandMine::ALandMine()
{

	//Actually Create BoxCol
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX COLLIDER"));

	//Set Base Damage
	BaseDamage = 25.0f;

}

// Called when the game starts or when spawned
void ALandMine::BeginPlay()
{
	Super::BeginPlay();

	//Call Overlap function of box collision
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::BeginOverlap);
	
}

//Overlap Event/Function
void ALandMine::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("overlapped%")));
	//}

	//GetActorLocation
	FVector SpawnLocation = GetActorLocation();

	//Apply Damage to Other Actor (Tank)
	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, GetInstigatorController(), this, nullptr);


	//Check sound is assigned
	if (ExplosionSound)
	{
		//Play sound cue at location
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, SpawnLocation);
	}


	//Check VFX is assigned
	if (ExplosionEffect)
	{
		//Play VFX at location
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, SpawnLocation);
	}

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


	//Get rid of the mine at the end
	Destroy();
}

