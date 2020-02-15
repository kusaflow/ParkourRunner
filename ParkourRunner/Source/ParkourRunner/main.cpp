// Fill out your copyright notice in the Description page of Project Settings.


#include "main.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_MainChar.h"
#include "Animation/AnimInstance.h"
#include "myGameInstance.h"

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


	GetCharacterMovement()->GroundFriction = 2.0f;
	GetCharacterMovement()->JumpZVelocity = 820.0f;

	timmerForjump = 0.0f;

}

// Called when the game starts or when spawned
void Amain::BeginPlay()
{
	Super::BeginPlay();
	speed = 900.f;

	
	//Crouch();

	//GetRootComponent()->SetRelativeLocation()
	
	gameInstance = Cast<UmyGameInstance>(GetGameInstance());
}

// Called every frame
void Amain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetCharacterMovement()->MaxWalkSpeed = speed;

	gameInstance = Cast<UmyGameInstance>(GetGameInstance());

	gameInstance->MainActorLocation = GetRootComponent()->GetRelativeLocation();

	//int x = gameInstance->tasks.front();
	//UE_LOG(LogTemp, Warning, TEXT("%d"),x);


	//here we check for action trigger and if its true theh the perform action 
	//is true and then action should be called
	if (actionTrigger) {
		if (
			(float)(gameInstance->sensorsClassQueue.front().x - gameInstance->sensorsClassQueue.front().sizeX) <= GetRootComponent()->GetRelativeLocation().X &&
			(float)(gameInstance->sensorsClassQueue.front().x + gameInstance->sensorsClassQueue.front().sizeX) >= GetRootComponent()->GetRelativeLocation().X
			) {
			///oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
		}
		else {
			actionTrigger = false;
			PerformingAction = true;
			speed = 0;
			GetCharacterMovement()->MaxWalkSpeed = speed;
			GetCharacterMovement()->GravityScale = 0;
			actionState = 1;

			if (gameInstance->sensorsClassQueue.front().task == 21) {
				LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 200,
					GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
			}
		}
	}

	//run constantly
	if ((Controller != nullptr) && !PerformingAction) {
		GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, 1);
	}
	else {
		ManageAction();
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d"), gameInstance->sensorsClassQueue.front().x);
	


}



void Amain :: ManageAction() {
	//----------------------------------------------------------------
	if (ActionIndex == 21) {
		if (actionState == 1) {
			GetCharacterMovement()->Launch(FVector(900, 0, 0));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 900,
					GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
				actionState = 2;
				//GetCharacterMovement()->GravityScale = 1;
			}
		}
		if (actionState == 2) {
			GetCharacterMovement()->Launch(FVector(900, 0, -350));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 100,
					GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
				actionState = 3;
			}
		}
		if (actionState == 3) {
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
			resetRunningState();
		}
		//resetRunningState();
	}
}


// Called to bind functionality to input
void Amain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &Amain::ActionPerformed);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &Amain::NormalJump);

	//PlayerInputComponent->BindAction("jumping", IE_Released, this, &Amain::StartJump);

	//PlayerInputComponent->BindAxis("MoveForward", this, &Amain::moveForward);
	PlayerInputComponent->BindAxis("Walk", this, &Amain::Walk);
	

}

/*
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
*/

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

void Amain::ActionPerformed() {
	//PerformingAction = !PerformingAction;
	//speed = 0;
	//GetCharacterMovement()->MaxWalkSpeed = speed;
	//GetCharacterMovement()->GravityScale = 0;	
	//GetCharacterMovement()->Launch(FVector(0, 0, 220));
	//checking for the collision

	if (!gameInstance->sensorsClassQueue.empty()) {
		if (
			(float)(gameInstance->sensorsClassQueue.front().x - gameInstance->sensorsClassQueue.front().sizeX) <= GetRootComponent()->GetRelativeLocation().X &&
			(float)(gameInstance->sensorsClassQueue.front().x + gameInstance->sensorsClassQueue.front().sizeX) >= GetRootComponent()->GetRelativeLocation().X
			) {
			//======================== long if for tasks===========================
			actionTrigger = true;
			ActionIndex = gameInstance->sensorsClassQueue.front().task;
			
			if (gameInstance->sensorsClassQueue.front().task == 21) {
				//GetCharacterMovement()->Launch(FVector(0, 0, 220));
				LocToDoMoves = FVector();
			
			}

			///==================================long if end here======================================
		}
	}
	/*
	task encyclopedia 
		block		|		tasks
		=====		|		=====
		  1			|		 11
		  2			|		 21
		  2

	*/


	//UE_LOG(LogTemp, Warning, TEXT("==========================Chal raha h================================"));


}


void Amain::NormalJump() {
	Jump();
}

void Amain :: resetRunningState() {
	PerformingAction = false;
	actionTrigger = false;
	speed = 700;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->GravityScale = 1;
	ActionIndex = 0;
	actionState = 0;
}

////onoverlap begin
//void Amain :: OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//
//}

