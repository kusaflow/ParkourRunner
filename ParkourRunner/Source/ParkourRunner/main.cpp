// Fill out your copyright notice in the Description page of Project Settings.


#include "main.h"	
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AnimInstance_MainChar.h"
#include "Animation/AnimInstance.h"
#include "Math/UnrealMathUtility.h"
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
	cameraBoom->TargetArmLength = 1500;
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
	if (!PerformingAction)
		GetCharacterMovement()->MaxWalkSpeed = speed;

	gameInstance = Cast<UmyGameInstance>(GetGameInstance());

	gameInstance->MainActorLocation = GetRootComponent()->GetRelativeLocation();

	//int x = gameInstance->tasks.front();

	
	/*if (TimeCounter >= 60) {
		UE_LOG(LogTemp, Warning, TEXT("Nice"));
		TimeCounter = 0;
	}
	else {
		TimeCounter += 60*DeltaTime;
	}*/
	
	//UE_LOG(LogTemp, Warning, TEXT("%f"),GetCharacterMovement()->MaxWalkSpeed);

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
			ActionInitState(DeltaTime);
		}
	}

	//run constantly
	if (!PerformingAction) {
		runCharacter();
	}
	else {
		ManageAction(DeltaTime);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%f"), GetCharacterMovement()->Velocity.Z);
	


}

void Amain :: ActionInitState(float dt) {
	actionTrigger = false;
	PerformingAction = true;
	actionState = 1;

	//action 21================================================================
	if (gameInstance->sensorsClassQueue.front().task == 21) {
		speed = 0;
		GetCharacterMovement()->MaxWalkSpeed = speed;
		GetCharacterMovement()->GravityScale = 0;

		LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 200,
			GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
	}
	//action 22================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 22) {
		speed = 0;
		GetCharacterMovement()->MaxWalkSpeed = speed;
		GetCharacterMovement()->GravityScale = 0;

		LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 300,
			GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
	}
	//action 23 && 24================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 23 ||
		gameInstance->sensorsClassQueue.front().task == 24) {
		speed = 0;
		GetCharacterMovement()->MaxWalkSpeed = speed;
		GetCharacterMovement()->GravityScale = 0;

		GetCharacterMovement()->GravityScale = 1;
		GetCharacterMovement()->JumpZVelocity = 300;
		Jump();
		LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 1030,
			GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
	}

	//
	//
	//===================================For 3===============================================================
	//
	//
	//action 31================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 31) {
		GetCharacterMovement()->MaxWalkSpeed = 600;		
		LocToDoMoves = GetRootComponent()->GetRelativeLocation();
		GetCharacterMovement()->Velocity.X = 300;
		GetCharacterMovement()->JumpZVelocity = 120;
		Jump();
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
}

void Amain :: runCharacter() {
	if (Controller != nullptr) {
		GetRootComponent()->GetChildComponent(1)->SetWorldRotation(FRotator(0, -90, 0));

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, 1);
	}
}



void Amain :: ManageAction(float dt) {
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
		else if (actionState == 2) {
			GetCharacterMovement()->Launch(FVector(900, 0, -320));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves = FVector(GetRootComponent()->GetRelativeLocation().X + 100,
					GetRootComponent()->GetRelativeLocation().Y, GetRootComponent()->GetRelativeLocation().Z);
				actionState = 3;
			}
		}
		else if (actionState == 3) {
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
			resetRunningState();
		}
		//resetRunningState();
	}
	//action 22==============================================================================
    else if (ActionIndex == 22) {
		if (actionState == 1) {
			GetCharacterMovement()->Launch(FVector(900, 0, 500));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves.X += 130;
				actionState = 2;
			}
		}
		else if (actionState == 2) {
			GetCharacterMovement()->Launch(FVector(800, 0, 0));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves.X += 600;
				actionState = 3;
			}
		}
		else if (actionState == 3) {
			GetCharacterMovement()->Launch(FVector(800, 0, -500));
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				actionState = 4;
			}
		}
		else if (actionState == 4) {
			resetRunningState();
		}


	}
	//action 23==============================================================================
	else if (ActionIndex == 23) {
		if (actionState == 1) {
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X && GetCharacterMovement()->Velocity.Z == 0) {
				//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
				actionState = 2;
				LocToDoMoves.X += 600;
				GetCharacterMovement()->GravityScale = 1;
				GetCharacterMovement()->MaxWalkSpeed = 1000;
				GetCharacterMovement()->JumpZVelocity = 300.0f;
				Jump();
			}
		}
		else if (actionState == 2) {
			runCharacter();
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				//resetRunningState();
				//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
				actionState = 3;
			}
		}
		else if (actionState == 3) {
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
			resetRunningState();
		}
	}
	//action 24==============================================================================
	else if (ActionIndex == 24) {
		if (actionState == 1) {
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X && GetCharacterMovement()->Velocity.Z == 0) {
				//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
				actionState = 2;
				LocToDoMoves.X += 600;
				GetCharacterMovement()->GravityScale = 1;
				GetCharacterMovement()->Velocity.X = 200;
				GetCharacterMovement()->MaxWalkSpeed = 900;
				GetCharacterMovement()->JumpZVelocity = 600;
				Jump();
			}
		}
		else if (actionState == 2) {
			runCharacter();
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				//LocToDoMoves.X += 0;
				//resetRunningState();
				//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
				//Jump();
				actionState = 3;
			}
		}
		else if (actionState == 3) {
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
			resetRunningState();
		}
	}
	//
	//
	// for 3
	//
	//
	else if (ActionIndex == 31) {
		if (actionState == 1) {
			if (GetCharacterMovement()->Velocity.Z == 0) {
				TimeCounter = 0;
				actionState = 2;
			}  
		}
		else if (actionState == 2) {
			TimeCounter += 60 * dt;
			if (TimeCounter >= 20) {
				LocToDoMoves.X += 380;
				actionState = 3;
				GetCharacterMovement()->JumpZVelocity = 600;
				GetCharacterMovement()->Velocity.X = 500;
				Jump();
			}
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
		}
		else if (actionState == 3) {
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				GetCharacterMovement()->Velocity = FVector(0);
			}
			//resetRunningState();
		}
	}






}


// Called to bind functionality to input
void Amain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent); 
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Action", IE_Pressed, this, &Amain::PerformAction);
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

void Amain::PerformAction() {
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
			///==================================long if end here======================================
		}
	}
	


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
	//GetCharacterMovement()->JumpZVelocity = 820.0f;
	ActionIndex = 0;
	actionState = 0;
}

////onoverlap begin
//void Amain :: OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//
//}

