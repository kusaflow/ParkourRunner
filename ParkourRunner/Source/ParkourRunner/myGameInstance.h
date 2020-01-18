// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY()
	FVector MainActorLocation;

	UPROPERTY()
	int ActorsToDrawAtATime = 100;
	
};
