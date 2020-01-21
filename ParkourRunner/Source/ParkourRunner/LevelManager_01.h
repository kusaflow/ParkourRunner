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
	float locationToDrawblock_X;

	UPROPERTY()
	int initVal=0, finalVal=0, midVal=0;

	UPROPERTY()
	bool ArrayToDrawIs_1 = true;

	UPROPERTY()
	int ListCount_LevelDecider = 0;

	/*UPROPERTY()
	int ListCount_1stBlockType = 0;

	UPROPERTY()
	int ListCount_2ndBlockType = 0;
	*/
	UPROPERTY()
	int ListCount_Total_actor_01 = 0;

	UPROPERTY()
	int ListCount_Total_actor_02 = 0;

	/*
	it stores data from the last block data so that when new block chaun is created
	we have referance it ot make it seemless
	*/
	UPROPERTY()
	int LastBlockTypeData = 0;




protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool DoDrawBlocks();
	void clearPrevBlocks();
	void createNewBlocksMngr();
	void createCheckBlocks();

	//functions for creation of blocks------------------------------------
	//creating random function
	int GenerateRandomLevelCreationTypes();
	int BlockCount(int type); 
	class LL_Actor_Node* createTheBlock(class LL_Actor_Node* head, int &counter,const int type, int Prevtype);

	//removelFunctions
	class LL_Actor_Node* removeActorsFromGame(class LL_Actor_Node* head);
	class LL_Actor_Node* RemoveDataForNew(class LL_Actor_Node* head,int &counter);
	

};
