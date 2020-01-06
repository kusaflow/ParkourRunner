// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager_01.generated.h"

UCLASS()
class PARKOURRUNNER_API ALevelManager_01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager_01();

	//VUD = Verticle Up Down

	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> VUP_001;

	UPROPERTY(BlueprintReadOnly)
	class UStaticMeshComponent* viewer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
