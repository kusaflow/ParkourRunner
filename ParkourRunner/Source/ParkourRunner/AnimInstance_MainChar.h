// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AnimInstance_MainChar.generated.h"

/**
 * 
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


private:



	
};
