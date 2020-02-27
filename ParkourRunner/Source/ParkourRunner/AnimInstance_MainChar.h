// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_MainChar.generated.h"

/**
 * char anim mul by 0.4f
 */
UCLASS()
class PARKOURRUNNER_API UAnimInstance_MainChar : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimProperty")
	void update();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float movementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bjumpAnticipation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameInstance")
	class UmyGameInstance* gameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	bool performingAction = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	int ActionIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	int actionState = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	bool waitingForNotify = false;


private:



	
};
