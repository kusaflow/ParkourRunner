// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager_01.h"
#include "LevelCreationBase.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"


// Sets default values
ALevelManager_01::ALevelManager_01()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	viewer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LEVEL_BLOCK"));
	viewer->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> levelObj(TEXT("/Game/otherStuff/3dModels/cube.cube"));

	if (levelObj.Succeeded()) {
		viewer->SetStaticMesh(levelObj.Object);
		viewer->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		viewer->SetWorldScale3D(FVector(1.0f));
	}



}

// Called when the game starts or when spawned
void ALevelManager_01::BeginPlay()
{
	Super::BeginPlay();

	ALevelCreationBase e(1);

	if (mainLandObstacles) {
		UWorld* world = GetWorld();

		if (world) {

			FActorSpawnParameters spawnPara;
			spawnPara.Owner = this;

			world->SpawnActor<ALevelCreationBase>(e.GetClass(),FVector(5000,0,0),FRotator(0),spawnPara);
		}
	}
	

}

// Called every frame
void ALevelManager_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

