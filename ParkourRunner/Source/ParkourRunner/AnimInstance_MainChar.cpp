// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance_MainChar.h"
#include "GameFramework/CharacterMovementComponent.h"


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


	}
}