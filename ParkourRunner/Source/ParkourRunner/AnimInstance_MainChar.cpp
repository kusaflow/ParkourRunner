// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_MainChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "main.h"
#include "myGameInstance.h"


void UAnimInstance_MainChar :: NativeInitializeAnimation() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}


}


void UAnimInstance_MainChar :: update () {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
	}
	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		Speed.Z = 0;
		movementSpeed = Speed.Size();

		Speed = Pawn->GetVelocity();

		Velocity = Speed;
		
		gameInstance = Cast<UmyGameInstance>(Pawn->GetGameInstance());

		if (gameInstance) {
			bjumpAnticipation = gameInstance->anticipateForJump;
		}

		
	}
}