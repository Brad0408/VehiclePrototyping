// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandMine.generated.h"

class UBoxComponent;

UCLASS()
class VEHICLE_API ALandMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandMine();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FString Message = FString(TEXT("OVERLAPPED"));

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite);
	UBoxComponent* BoxCollision;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
