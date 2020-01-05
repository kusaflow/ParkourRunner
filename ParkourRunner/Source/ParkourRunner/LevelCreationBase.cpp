// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelCreationBase.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ALevelCreationBase::ALevelCreationBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEVEL_BLOCK"));
	block->SetupAttachment(RootComponent);

}

ALevelCreationBase::ALevelCreationBase(const int t):
	type(t)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	block = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEVEL_BLOCK"));
	block->SetupAttachment(RootComponent);	

	char* e = nullptr;

	if (t == 1) {
		e = "/Game/otherStuff/3dModels/LevelCreationBricks/001";
	}
	else if (t == 2) {
		e = "/Game/otherStuff/3dModels/LevelCreationBricks/002";
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> levelObj(ANSI_TO_TCHAR(e));

	e = nullptr;

	if (levelObj.Succeeded()) {
		block->SetStaticMesh(levelObj.Object);
		block->SetRelativeLocation(FVector(0.0f,0.0f,0.0f));
		block->SetWorldScale3D(FVector(1.0f));
	}

	

}

// Called when the game starts or when spawned
void ALevelCreationBase::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ALevelCreationBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

