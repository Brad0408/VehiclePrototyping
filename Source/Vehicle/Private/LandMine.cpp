// Fill out your copyright notice in the Description page of Project Settings.


#include "LandMine.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ALandMine::ALandMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX COLLIDER"));
	BaseDamage = 25.0f;



}

// Called when the game starts or when spawned
void ALandMine::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALandMine::BeginOverlap);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::White, FString::Printf(TEXT("ACTUAL TEXT?%"), *Message));
	}
	
}

void ALandMine::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AController* PlayerC = GetInstigator()->GetController();

	UGameplayStatics::ApplyDamage(OtherActor, BaseDamage, PlayerC, this, DamageType);
	//Destroy();
}

// Called every frame
void ALandMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

