// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "main.generated.h"

UCLASS()
class PARKOURRUNNER_API Amain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Amain();
	
	//actor of Sensor
	UPROPERTY(EditAnywhere, category = "meshes")
	TSubclassOf<class AActor> sensorFromBP;

	/*camera behind the player*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* cameraBoom;

	/* main camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera; 
	
	UPROPERTY()
	float speed;

	UPROPERTY()
	bool Initiatejump = false;

	UPROPERTY()
	float timmerForjump = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UmyGameInstance* gameInstance;


	//Action For Parkour========================================================
	UPROPERTY()
	bool PerformingAction = false,actionTrigger = false;
	//action trigger will be true whwn space bar is pressed a the sensoes so that
	//when the collition ends the block will set action perform true and the action is called
	UPROPERTY()
	unsigned int ActionIndex = 0,actionState = 0;
	//fVector to the location of the actor to perform all the moves
	UPROPERTY()
	FVector LocToDoMoves;

	UPROPERTY(VisibleAnyWhere, BluePrintReadOnly, Category="KusaVar")
	float TimeCounter = 0;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**moving forwars and backword*/
	void moveForward(float value);
	void Walk(float value);

	//action
	void PerformAction();
	// action init state
	void ActionInitState(float dt);


	void NormalJump();

	//reset all values to running state
	void resetRunningState();

	//action manager
	void ManageAction(float dt);

	//running essential
	void runCharacter();

	

	//overlapping events
	//UFUNCTION()
	//void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		//int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
