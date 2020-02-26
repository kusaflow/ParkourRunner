// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <iostream>
#include <queue>
#include "Engine/GameInstance.h"
#include "myGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PARKOURRUNNER_API UmyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
	bool anticipateForJump;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
	FVector MainActorLocation;

	UPROPERTY()
	int ActorsToDrawAtATime = 40;

	UPROPERTY()
	bool performingAction = false;
	UPROPERTY()
	unsigned int ActionIndex = 0, actionState = 0;



	//classes
	class sensorClass {
	public:
		unsigned int x, y, sizeX, sizeY, task;
	};

	//UPROPERTY()
	std::queue<class sensorClass> sensorsClassQueue;

	
	
};
