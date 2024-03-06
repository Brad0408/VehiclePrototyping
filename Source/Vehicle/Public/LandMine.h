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

	//Damage of landmine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Create BoxCollision
	UPROPERTY(EditAnywhere);
	UBoxComponent* BoxCollision;

	//Create NiagaraEffect
	UPROPERTY(EditAnywhere, Category = "VFX")
	class UNiagaraSystem* NiagaraEffect;

	//Create ExplosionSound
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundBase* ExplosionSound;

	//Create ForceFeedback
	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	class UForceFeedbackEffect* ForceFeedbackEffect;

	//Overlap Event Function
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
