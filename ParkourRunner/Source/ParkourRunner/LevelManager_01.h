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


	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_001;

	UPROPERTY(BlueprintReadOnly)
	class UStaticMeshComponent* viewer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UmyGameInstance* gameInstance;


	//default size of the 001 block is 200 units

	UPROPERTY()
	class ALevelCreationBase* actor_01[100];

	UPROPERTY()
	float locationToDrawblock_X;

	UPROPERTY()
	int initVal=0, finalVal=0, midVal=0;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool DoDrawBlocks();
	void clearPrevBlocks();
	void clearNewBlocks();
	void clearCheckBlocks();
};
