// Fill out your copyright notice in the Description page of Project Settings.


#include "main.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_MainChar.h"
#include "Animation/AnimInstance.h"

// Sets default values
Amain::Amain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create camera bomm
	//pull towards player when coloosion happens
	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(GetRootComponent());
	cameraBoom->bDoCollisionTest = false;
	cameraBoom->TargetArmLength = 450.f;
	cameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	cameraBoom->RelativeRotation = (FRotator(-15.f, 300.f, 0.f));
	//cameraBoom->RelativeRotation = FRotator(0.f, 0.f, 0.f);
	cameraBoom->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	GetCharacterMovement()->MaxWalkSpeed = 900.0f;
	GetCharacterMovement()->GroundFriction = 2.0f;

	anim = Cast<UAnimInstance_MainChar>(GetMesh()->GetAnimInstance()); //(GetMesh()->GetAnimInstance());
	if (!anim) return;

}

// Called when the game starts or when spawned
void Amain::BeginPlay()
{
	Super::BeginPlay();
	speed = 900.f;
	//Crouch();
	
}

// Called every frame
void Amain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = speed;


}

// Called to bind functionality to input
void Amain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("jumping", IE_Pressed, this, &Amain::anticipateForJump);
	PlayerInputComponent->BindAction("jumping", IE_Released, this, &Amain::StartJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &Amain::moveForward);
	PlayerInputComponent->BindAxis("Walk", this, &Amain::Walk);
	

}

void Amain :: moveForward(float value) {
	if ((Controller != nullptr) && (value != 0.0f)) {
		if (value < 0) {
			GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, 90, 0));
		}
		else {
			GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));
		}
		
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);
	}
}

void Amain::Walk(float value) {
	if (value == 1) {
		//setting val to 150
		if (speed > 150.f) {
			speed -= 5.f;
		}
		else {
			speed = 150.f;
		}
	} else {
		// setting val to 900
		if (speed < 900.f) {
			speed += 5.f;
		}
		else {
			speed = 900.f;
		}
	}
}

void Amain::StartJump() {
	bJumpAnticipate = false;
	//anim->setbJumpAnticipation(false);
	if(anim)
		anim->setbJumpAnticipation(false);
	Jump();
}

void Amain::anticipateForJump() {
	bJumpAnticipate = true;
	if(anim)
		anim->setbJumpAnticipation(true);
}



