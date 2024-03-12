// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankShell.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class UForceFeedbackEffect;

//Potential implementation in c++ of FS_MasterField
//class UFieldSystemComponent;

UCLASS()
class VEHICLE_API ATankShell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankShell();

	//Create tank shell damage and radius values
	UPROPERTY(EditDefaultsOnly, Category = "Damage Values")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Values")
	float MinimumDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Values")
	float DamageInnerRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Values")
	float DamageOuterRadius;

	//Impulse strength when impulse is applied
	UPROPERTY(EditDefaultsOnly, Category = "Damage Values")
	float RadialStrength;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Declare Reference to Projectile Movement Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	//Declare NiagaraEffect to be set in editor
	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	//Declare ExplosionSound to be set in editor
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TObjectPtr<USoundBase> ExplosionSound;

	//Declare ForceFeedback to be set in editor
	UPROPERTY(EditAnywhere, Category = "ForceFeedback")
	TObjectPtr<UForceFeedbackEffect> ForceFeedbackEffect;

	//OnStop Function
	UFUNCTION()
	void OnProjectileStop(const FHitResult &ImpactResult);


};
