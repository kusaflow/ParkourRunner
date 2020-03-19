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
	cameraBoom->RelativeRotation = (FRotator(-20, -90, 0.f));
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

	gameInstance->performingAction = PerformingAction;
	gameInstance->ActionIndex = ActionIndex;
	gameInstance->actionState = actionState;

	gameInstance->MainActorLocation = GetRootComponent()->GetRelativeLocation();

	//int x = gameInstance->tasks.front();

	
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
		gameInstance->waitingForNotify = false;
		GetCharacterMovement()->MaxWalkSpeed = 0;
		GetCharacterMovement()->Velocity = FVector(0);
	}
	//action 22================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 22 || gameInstance->sensorsClassQueue.front().task == 23) {
		gameInstance->waitingForNotify = false;
		//GetCharacterMovement()->MaxWalkSpeed = 700;
		GetCharacterMovement()->JumpZVelocity = 200;
		Jump();
		//GetCharacterMovement()->Velocity = FVector(500,0,0);
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
		GetCharacterMovement()->Velocity.X = 500;
		GetCharacterMovement()->JumpZVelocity = 120;
		Jump();
	}
	//
	//
	//===================================For 4===============================================================
	//
	//
	//action 41================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 41) {
		GetCharacterMovement()->MaxWalkSpeed = 600;
		LocToDoMoves = GetRootComponent()->GetRelativeLocation();
		GetCharacterMovement()->Velocity.X = 0;
		gameInstance->waitingForNotify = false;
	}
	//
	//
	//===================================For 5===============================================================
	//
	//
	//action 51================================================================
	else if (gameInstance->sensorsClassQueue.front().task == 51) {
		GetCharacterMovement()->MaxWalkSpeed = 600;
		LocToDoMoves = GetRootComponent()->GetRelativeLocation();
		GetCharacterMovement()->Velocity.X = 0;
		gameInstance->waitingForNotify = false;
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
	//21----------------------------------------------------------------
	if (ActionIndex == 21) {
		if(actionState == 1) {
			if (gameInstance->waitingForNotify) {
				actionState = 2;
			}
		}
		else if (actionState == 2) {
			GetCharacterMovement()->Velocity.X = 800;
			GetCharacterMovement()->JumpZVelocity = 570;
			Jump();
			actionState = 3;
			LocToDoMoves = GetRootComponent()->GetRelativeLocation();
			LocToDoMoves.X += 1200;
		}
		else if (actionState == 3) {
			//UE_LOG(LogTemp, Warning, TEXT("%f"), GetRootComponent()->GetRelativeLocation().X - LocToDoMoves.X);
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				actionState = 4;
				gameInstance->waitingForNotify = false;
			}
		}
		else if (actionState == 4) {
			//GetCharacterMovement()->Launch(FVector(0,0,3000));
			if (gameInstance->waitingForNotify) {
				resetRunningState(800);
			}
		}
	}
	//22----------------------------------------------------------------
	else if (ActionIndex == 22) {
		if (actionState == 1) {
			if (gameInstance->waitingForNotify) {
				actionState = 2;
				GetCharacterMovement()->Velocity.X = 820;
				GetCharacterMovement()->JumpZVelocity = 450;
				Jump();
				LocToDoMoves = GetRootComponent()->GetRelativeLocation();
				LocToDoMoves.X += 1000;
			}
		}
		else if (actionState == 2) {
			//UE_LOG(LogTemp, Warning, TEXT("%f"), GetRootComponent()->GetRelativeLocation().X - LocToDoMoves.X);
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				actionState = 3;
				//GetCharacterMovement()->Velocity = FVector(0,0,2000);
				gameInstance->waitingForNotify = false;
			}
		}
		else if (actionState == 3) {
			if (gameInstance->waitingForNotify) {
				GetCharacterMovement()->Velocity = FVector(300, 0, 0);
				GetCharacterMovement()->JumpZVelocity = 400;
				gameInstance->waitingForNotify = false;
				Jump();
				actionState = 4;
			}
		}
		else if (actionState == 4) {
			// GetCharacterMovement()->Velocity = FVector(50, 0, 0);
			if (gameInstance->waitingForNotify) {
				actionState = 5;
				gameInstance->waitingForNotify = false;
				resetRunningState(800);
			}
		}


	}
	//23----------------------------------------------------------------
	else if (ActionIndex == 23) {
		if (actionState == 1) {
			if (gameInstance->waitingForNotify) {
				actionState = 2;
				GetCharacterMovement()->Velocity.X = 750;
				GetCharacterMovement()->JumpZVelocity = 500;
				Jump();
				LocToDoMoves = GetRootComponent()->GetRelativeLocation();
				LocToDoMoves.X += 1000;
			}
		}
		else if (actionState == 2) {
			//UE_LOG(LogTemp, Warning, TEXT("%f"), GetRootComponent()->GetRelativeLocation().X - LocToDoMoves.X);
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				actionState = 3;
				//GetCharacterMovement()->Velocity = FVector(0,0,2000);
				gameInstance->waitingForNotify = false;
			}
		}
		else if (actionState == 3) {
			if (gameInstance->waitingForNotify) {
				GetCharacterMovement()->Velocity = FVector(300, 0, 0);
				GetCharacterMovement()->JumpZVelocity = 400;
				gameInstance->waitingForNotify = false;
				Jump();
				actionState = 4;
			}
		}
		else if (actionState == 4) {
			//GetCharacterMovement()->Velocity = FVector(700, 0, 0);
			if (gameInstance->waitingForNotify) {
				actionState = 5;
				gameInstance->waitingForNotify = false;
			}
		}
		else if (actionState == 5) {
			GetCharacterMovement()->Velocity = FVector(700, 0, 0);
			if (gameInstance->waitingForNotify) {
				actionState = 6;
				gameInstance->waitingForNotify = false;
				resetRunningState(800);
			}
		}
		


	}
	//GetCharacterMovement()->GravityScale = 0;
	//GetCharacterMovement()->Velocity = FVector(0);
	//
	//
	// for 3
	//
	//
	else if (ActionIndex == 31) {
		if (actionState == 1) {
			//small jump
			if (GetCharacterMovement()->Velocity.Z == 0) {
				TimeCounter = 0;
				actionState = 2;
			}  
		}
		//wait a bit
		else if (actionState == 2) {
			TimeCounter += 60 * dt;
			if (TimeCounter >= 20) {
				LocToDoMoves.X += 390;
				actionState = 3;
				GetCharacterMovement()->JumpZVelocity = 600;
				GetCharacterMovement()->Velocity.X = 500;
				Jump();
			}
			//GetCharacterMovement()->Launch(FVector(0, 0, 2000));
		}
		//jump and to the clif
		else if (actionState == 3) {
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				LocToDoMoves.Z= GetRootComponent()->GetRelativeLocation().Z;
				LocToDoMoves.Z -= 50;
				actionState = 4;
			}
			//resetRunningState();
		}
		//
		else if (actionState == 4) {
			GetCharacterMovement()->Velocity.Z = -200;
			if (GetRootComponent()->GetRelativeLocation().Z <= LocToDoMoves.Z) {
				LocToDoMoves.Z +=150;
				actionState = 5;
			}
			//resetRunningState();
		}
		//
		else if (actionState == 5) {
			GetCharacterMovement()->Velocity.Z = 220;
			if (GetRootComponent()->GetRelativeLocation().Z >= LocToDoMoves.Z) {
				actionState = 6;
				LocToDoMoves.X = GetRootComponent()->GetRelativeLocation().X;
				LocToDoMoves.X += 100;
			}
			//resetRunningState();
		}
		else if (actionState == 6) {
			GetCharacterMovement()->Velocity = FVector(300, 0, 0);
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				resetRunningState(800);
			}
			//resetRunningState();
		}
	}
	//
	//
	// for 4
	//
	//
	//41----------------------------------------------------------------
	else if (ActionIndex == 41) {
		if (actionState == 1) {
			//jumping anticipation
			if (gameInstance->waitingForNotify) {
				//jump anticipation done now jumping
				GetCharacterMovement()->MaxWalkSpeed = 600;
				LocToDoMoves = GetRootComponent()->GetRelativeLocation();
				GetCharacterMovement()->Velocity.X = 500;
				GetCharacterMovement()->JumpZVelocity = 630;
				LocToDoMoves.X += 475-30;
				Jump();
				actionState = 2;
			}
		}
		else if (actionState == 2) {
			//is air
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				//-70 unit away from cliff
				gameInstance->waitingForNotify = false;
				GetCharacterMovement()->GravityScale = 0;
				actionState = 3;
			}
		}
		else if (actionState == 3) {
			//movin in x
			GetCharacterMovement()->Velocity = FVector(100,0,0);
			if (gameInstance->waitingForNotify) {//braced hang 1
				//touching the clif
				gameInstance->waitingForNotify = false;
				actionState = 4;
			}
		}
		else if (actionState == 4) {
			//on cliff
			//GetCharacterMovement()->Velocity.X = 100;
			if (gameInstance->waitingForNotify) {//braced hang 2
				//touching the clif
				//GetCharacterMovement()->Velocity = FVector(0);
				gameInstance->waitingForNotify = false;
				actionState = 5;
			}
		}
		else if (actionState == 5) {
			//going up
			GetCharacterMovement()->Velocity = FVector(0, 0, 150);
			if (gameInstance->waitingForNotify) {//braced hang to crouch 1
				//on the cliff now go in X
				GetCharacterMovement()->GravityScale = 1;
				gameInstance->waitingForNotify = false;
				actionState = 6;
			}
		}
		else if (actionState == 6) {
			//going in X
			GetCharacterMovement()->Velocity = FVector(100, 0, 0);
			if (gameInstance->waitingForNotify) {//braced hang to crouch 2
				//now in stiing pose  
				gameInstance->waitingForNotify = false;
				actionState = 7;
			}
		}
		else if (actionState == 7) {
			GetCharacterMovement()->Velocity = FVector(50, 0, 0);
			if (gameInstance->waitingForNotify) {
				//actionState = 5;
				//GetCharacterMovement()->GravityScale = 0;
				resetRunningState(800);
			}
		}

	}

//
//
//=============5
//
//
	else if (ActionIndex == 51) {
		if (actionState == 1) {
			//jumping anticipation
			if (gameInstance->waitingForNotify) {
				//jump anticipation done now jumping
				GetCharacterMovement()->MaxWalkSpeed = 600;
				LocToDoMoves = GetRootComponent()->GetRelativeLocation();
				GetCharacterMovement()->Velocity.X = 500;
				GetCharacterMovement()->JumpZVelocity = 630;
				LocToDoMoves.X += 475 - 30;
				Jump();
				actionState = 2;
			}
		}
		else if (actionState == 2) {
			//is air
			if (GetRootComponent()->GetRelativeLocation().X >= LocToDoMoves.X) {
				//-70 unit away from cliff
				gameInstance->waitingForNotify = false;
				GetCharacterMovement()->GravityScale = 0;
				actionState = 3;
			}
			//50 units

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

void Amain :: resetRunningState(float s) {
	PerformingAction = false;
	actionTrigger = false;
	speed = s;
	GetCharacterMovement()->MaxWalkSpeed = speed;
	GetCharacterMovement()->GravityScale = 1;
	//GetCharacterMovement()->JumpZVelocity = 820.0f;
	ActionIndex = 0;
	actionState = 0;

	gameInstance->performingAction = PerformingAction;
	gameInstance->ActionIndex = ActionIndex;
	gameInstance->actionState = actionState;
}

////onoverlap begin
//void Amain :: OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//
//}

