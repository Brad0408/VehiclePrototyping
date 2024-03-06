// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandMine.generated.h"


class UBoxComponent;
class UNiagaraSystem;
class USoundBase;
class UForceFeedbackEffect;

UCLASS()
class VEHICLE_API ALandMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandMine();

	//Define Damage of landmine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Declare BoxCollision to be set in editor
	UPROPERTY(EditAnywhere);
	UBoxComponent* BoxCollision;

	//Declare NiagaraEffect to be set in editor
	UPROPERTY(EditAnywhere, Category = "VFX")
	class UNiagaraSystem* NiagaraEffect;

	//Declare ExplosionSound to be set in editor
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* ExplosionSound;

	//Declare ForceFeedback to be set in editor
	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	class UForceFeedbackEffect* ForceFeedbackEffect;

	//Overlap Event Function
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
