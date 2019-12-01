// Fill out your copyright notice in the Description page of Project Settings.


#include "main.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

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



}

// Called when the game starts or when spawned
void Amain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Amain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void Amain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

