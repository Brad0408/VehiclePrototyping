// Fill out your copyright notice in the Description page of Project Settings.


#include "TankDecoy.h"

ATankDecoy::ATankDecoy()
{
	//Set low so you can just one shot the tank 
	Health = 1.0f;
}


void ATankDecoy::ModifyHealth(float Damage)
{
	if (Health > 0.0f)
	{
		Health = Health - Damage;

		if (Health <= 0.0f)
		{
			Destroy();

			//Afterwards SpawnActor is called to spawn BP_DestroyedTank (in BP)
		}
	}
}