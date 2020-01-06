// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelCreationBase.generated.h"

UCLASS()
class PARKOURRUNNER_API ALevelCreationBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelCreationBase();

	UPROPERTY(BlueprintReadOnly, category = "mesh")
	int type = 1;
	
	enum TypeOfObstacle 
	{
		GroundMoveVertically = 1
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void setType(const int t);

};
