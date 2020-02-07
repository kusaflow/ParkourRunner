// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <queue>
#include "LevelManager_01.generated.h"

UCLASS()
class PARKOURRUNNER_API ALevelManager_01 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager_01();
	~ALevelManager_01();

	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_001;
	
	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_002;
	
	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_003;

	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_004;

	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<class ALevelCreationBase> Block_005;

	UPROPERTY(EditDefaultsOnly, category = "meshes")
	TSubclassOf<AActor> sensorMesh;

	TSubclassOf<class ALevelCreationBase> Block;

	UPROPERTY(BlueprintReadOnly)
	class UStaticMeshComponent* viewer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UmyGameInstance* gameInstance;

	//default size of the 001 block is 200 units

	//===Variable to clear the previous blocks
	short BlocksOnBack = 0, BlocksOnFront = 0;


	UPROPERTY()
	unsigned int locationToDrawblock_X;

	UPROPERTY()
	int initVal=0, finalVal=0, midVal=0;

	UPROPERTY()
	int ListCount_LevelDecider = 0;

	UPROPERTY()
	bool FirstRun = true;

	std::queue<AActor*> taskSensors;



	//--------------------------
	/*UPROPERTY()
	int ListCount_1stBlockType = 0;

	UPROPERTY()
	int ListCount_2ndBlockType = 0;
	*/
	

	/*
	it stores data from the last block data so that when new block chaun is created
	we have referance it ot make it seemless
	*/


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool DoDrawBlocks();
	void clearPrevBlocks();
	void createCheckBlocks();

	//functions for creation of blocks------------------------------------
	//creating random function
	int GenerateRandomLevelCreationTypes();
	int BlockCount(int type);
	void createNewBlocksMngr();
	void createTheBlock(const int type);

	//removelFunctions
	void removeActorsFromGame();
	void RemoveDataForNew();


	//random block for different color
	void RandomBlock();

	//----------------Linked List------------------------

	UFUNCTION()
	void insertInt(int data);

	UFUNCTION()
	void insertActor(class ALevelCreationBase* data);

	UFUNCTION()
	void DeleteLast50Actors();

	UFUNCTION()
	void DeleteAllInts();


	//===================================================
	
	UFUNCTION()
	void AddSensors(int type, unsigned int posX, UWorld* world);

};
