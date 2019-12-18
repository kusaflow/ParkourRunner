// Fill out your copyright notice in the Description page of Project Settings.


#include "main.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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


	//init values
	speed = 1300.0f;

	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->GroundFriction = 2.0f;
}

// Called when the game starts or when spawned
void Amain::BeginPlay()
{
	Super::BeginPlay();
	//Crouch();
	
}

// Called every frame
void Amain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	delta = DeltaTime;

}

// Called to bind functionality to input
void Amain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &Amain::moveForward);

}

void Amain :: moveForward(float value) {
	if ((Controller != nullptr) && (value != 0.0f)) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, value);

		
	}
}

