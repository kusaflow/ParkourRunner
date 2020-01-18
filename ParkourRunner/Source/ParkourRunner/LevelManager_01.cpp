// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager_01.h"
#include "LevelCreationBase.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "myGameInstance.h"


//Linlked list 
template<typename T>
class LLNode {
public:
	T* actor;
	LLNode* next;

	LLNode(T* node_data) {
		this->actor = node_data;
		this->next = nullptr;
	}
};

template<typename T>
class LinkedList {
public:
	LLNode* head;

	LinkedList() {
		this->head = nullptr;
	}
};


UFUNCTION()
template<typename T>
LLNode<T>* insert(LLNode<T>* head, ALevelCreationBase* data,int &counter);

UFUNCTION()
template<typename T>
LLNode<T>* deleteFromLast(LLNode<T>* head, int &counter);

UFUNCTION()
template<typename T>
void DeleteAll(LLNode<T>* head, int &counter);

UPROPERTY()
int ListCount_LevelDecider = 0;
//Linked List end


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

	locationToDrawblock_X = 600.0f;

}

// Called when the game starts or when spawned
void ALevelManager_01::BeginPlay()
{
	Super::BeginPlay();

	/*if (Block_001) {
		UWorld* world = GetWorld();

		if (world) {

			FActorSpawnParameters spawnPara;
			spawnPara.Owner = this;
			for (int i = 0; i < 100; i++) {
				actor_01[i] =  world->SpawnActor<ALevelCreationBase>(Block_001, FVector(locationToDrawblock, 0, -580.0f), FRotator(0), spawnPara);
				locationToDrawblock += 200;
			}
		}
	}*/
	

}

// Called every frame
void ALevelManager_01::Tick(float DeltaTime)
{
	gameInstance = Cast<UmyGameInstance>(GetGameInstance());

}


//LinkedListOperations

template<typename T>
LLNode<T>* insert(LLNode<T>* head, T* data, int &counter) {
	if (head == nullptr) {
		//list is empty so add this at head
		LLNode<T>* newNode = new LLNode<T>(data);
		head = newNode;
		counter++;
		return head;
	}
	LLNode<T>* newNode = new LLNode<T>(data);
	LLNode<T>* h2 = head;
	for (int i = 0; i < counter - 1; i++) {
		h2 = h2->next;
	}
	h2->next = newNode;
	counter++;

	return head;
}

template<typename T>
LLNode<T>* deleteFromLast(LLNode<T>* head, int &counter) {
	if (head == nullptr) {
		return head;
	}
	if (counter == 0) return head;

	if (counter== 1) {
		head = nullptr;
		counter--;
		return head;
	}

	LLNode<T>* h2 = head;

	for (int i = 0; i < counter - 2; i++) {
		h2 = h2->next;
	}
	LLNode<T>* toBeDeleted = h2->next;
	h2->next = nullptr;
	delete toBeDeleted;

	*counter--;
	return head;
}

template<typename T>
void DeleteAll(LLNode<T>* head, int &counter) {
	LLNode<T>* h2 = head->next;
	while (h2 != nullptr) {
		delete head;
		head = h2;
		h2 = h2->next;
	}

	delete head;


	counter=0;
}

//Linked List operation End

