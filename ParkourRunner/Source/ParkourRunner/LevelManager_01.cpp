// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager_01.h"
#include "LevelCreationBase.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "myGameInstance.h"

//UE_LOG(LogTemp, Warning, TEXT("%d"), h2->data);


//Linlked list 
class LL_INT_Node {
public:
	int data;
	LL_INT_Node* next;

	LL_INT_Node(int node_data) {
		this->data = node_data;
		this->next = nullptr;
	}
};

class LL_Actor_Node {
public:
	ALevelCreationBase* actor;
	LL_Actor_Node* next;

	LL_Actor_Node(ALevelCreationBase* node_data) {
		this->actor = node_data;
		this->next = nullptr;
	}
};


class LinkedList_INT {
public:
	LL_INT_Node* head;

	LinkedList_INT() {
		this->head = nullptr;
	}
};


class LinkedList_Actor {
public:
	LL_Actor_Node* head;

	LinkedList_Actor() {
		this->head = nullptr;
	}
};
//end of class definaton

UFUNCTION()
LL_INT_Node* insertInt(LL_INT_Node* head, int data, int& counter);

UFUNCTION()
LL_Actor_Node* insertActor(LL_Actor_Node* head, ALevelCreationBase* data, int& counter);

UFUNCTION()
template<typename T>
T* deleteFromLast(T* head, int& counter);

UFUNCTION()
template<typename T>
void DeleteAll(T* head, int& counter);

//Linked List end

//-------------------------------------------------
//important Linked list for the level
UPROPERTY()//list of number that shows the type of blocks to be created
LinkedList_INT* head_LevelDesigner = new LinkedList_INT();

//UPROPERTY()//
//LinkedList<ALevelCreationBase> *head_1stBlocksType = new LinkedList<ALevelCreationBase>();
//
//UPROPERTY()//
//LinkedList<ALevelCreationBase> *head_2ndBlocksType = new LinkedList<ALevelCreationBase>();

UPROPERTY()//total individual block to be drawn at a time on screen
LinkedList_Actor* head_Total_actor_01 = new LinkedList_Actor();

UPROPERTY()//total individual block to be drawn at a time on screen
LinkedList_Actor* head_Total_actor_02 = new LinkedList_Actor();
//------------------------------------------------


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
	ArrayToDrawIs_1 = false;

	//UE_LOG(LogTemp, Warning, TEXT("==========================================kusaflow=========================================="));
	gameInstance = Cast<UmyGameInstance>(GetGameInstance());


	try {
		//init level creation of level
		createNewBlocks();
	}
	catch (...) {

	}

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

	/*if (DoDrawBlocks()) {
		if (ArrayToDrawIs_1) {
			RemoveDataForNew(head_Total_actor_01->head,ListCount_Total_actor_01);
		}
		else {
			RemoveDataForNew(head_Total_actor_02->head,ListCount_Total_actor_02);
		}

		//creating new blocks
		createNewBlocks();

	}*/


	UE_LOG(LogTemp, Warning, TEXT("%d"), midVal);




}

// runs on every tick to check do we have to create new files or not
bool ALevelManager_01::DoDrawBlocks() {
	if (gameInstance->MainActorLocation.X >= midVal)
		return true;
	return false;
}

//create new blocks=======================================================================

void ALevelManager_01::createNewBlocks() {
	int counterOfActorsOnScreen = 0;
	if (head_LevelDesigner->head != nullptr) {
		DeleteAll<LL_INT_Node>(head_LevelDesigner->head, ListCount_LevelDecider);
	}
	//return;
	while (counterOfActorsOnScreen <= gameInstance->ActorsToDrawAtATime) {
		int val = GenerateRandomLevelCreationTypes();
		counterOfActorsOnScreen += BlockCount(val);
		if (counterOfActorsOnScreen <= gameInstance->ActorsToDrawAtATime) {
			//its a valid number to we can add it to the list
			LL_INT_Node* head = insertInt(head_LevelDesigner->head, val, ListCount_LevelDecider);
			head_LevelDesigner->head = head;
		}
		else {
			continue;
		}
	}

	initVal = locationToDrawblock_X;

	//now generating the levels
	LL_INT_Node* h2 = head_LevelDesigner->head;
	int prevData = LastBlockTypeData;
	while (h2 != nullptr) {
		if (ArrayToDrawIs_1) {
			createTheBlock(head_Total_actor_01->head, ListCount_Total_actor_01,h2->data, prevData);
		}
		else {
			createTheBlock(head_Total_actor_02->head, ListCount_Total_actor_02, h2->data, prevData);
		}

		//UE_LOG(LogTemp, Warning, TEXT("%d"), h2->data);
		prevData = h2->data;
		h2 = h2->next;
	}

	finalVal = locationToDrawblock_X;

	midVal = (finalVal - initVal)/2;
	midVal += initVal;


	ArrayToDrawIs_1 = !ArrayToDrawIs_1;
}

void ALevelManager_01::createTheBlock(LL_Actor_Node* head,int &counter, const int type, int Prevtype) {
	//here we create the block type
	ALevelCreationBase* actor;
	FActorSpawnParameters spawnPara;
	spawnPara.Owner = this;
	//UE_LOG(LogTemp, Warning, TEXT("==========================================Block is clear=========================================="));

	if (Block_001) {

		UWorld* world = GetWorld();
		if (world) {
			//----------------selection--------------
			if (type == 1) {
				actor = world->SpawnActor<ALevelCreationBase>(Block_001, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				locationToDrawblock_X += 200;
				LL_Actor_Node* headOut = insertActor(head, actor, counter);
				head = headOut;
			}
		}
	}
}

//---------------------------

//======================================================================================================================
//remove the actors from the screen ........clearing memory


void ALevelManager_01 :: RemoveDataForNew(LL_Actor_Node* head,int &counter) {
	removeActorsFromGame(head);
	DeleteAll<LL_Actor_Node>(head, counter);
}

LL_Actor_Node* ALevelManager_01 :: removeActorsFromGame(LL_Actor_Node* head) {
	if (head == nullptr) {
		return head;
	}

	LL_Actor_Node* h2 = head;

	while (h2) {
		h2->actor->Destroy();
		h2 = h2->next;
	}

	return head;

}
//-----------------------------------------


int ALevelManager_01::GenerateRandomLevelCreationTypes() {
	//creating this as seperate for future if i have to create a logic to make complex randomness
	return 1;
}

int ALevelManager_01::BlockCount(int type) {
	if (type == 1) {
		return 2;
	}
	return 0;
}
//----------------------------------------------------------------------------------------



//LinkedListOperations-----------------------------------------------------------

LL_INT_Node* insertInt(LL_INT_Node* head, int data, int& counter) {
	if (head == nullptr) {
		//list is empty so add this at head
		LL_INT_Node* newNode = new LL_INT_Node(data);
		head = newNode;
		counter++;
		return head;
	}
	LL_INT_Node* newNode = new LL_INT_Node(data);
	LL_INT_Node* h2 = head;
	for (int i = 0; i < counter - 1; i++) {
		h2 = h2->next;
	}
	h2->next = newNode;
	counter++;

	return head;
}

LL_Actor_Node* insertActor(LL_Actor_Node* head, ALevelCreationBase* data, int& counter) {
	if (head == nullptr) {
		//list is empty so add this at head
		LL_Actor_Node* newNode = new LL_Actor_Node(data);
		head = newNode;
		counter++;
		return head;
	}
	LL_Actor_Node* newNode = new LL_Actor_Node(data);
	LL_Actor_Node* h2 = head;
	for (int i = 0; i < counter - 1; i++) {
		h2 = h2->next;
	}
	h2->next = newNode;
	counter++;

	return head;
}


template<typename T>
T* deleteFromLast(T* head, int& counter) {
	if (head == nullptr) {
		return head;
	}
	if (counter == 0) return head;

	if (counter == 1) {
		head = nullptr;
		counter--;
		return head;
	}

	T* h2 = head;

	for (int i = 0; i < counter - 2; i++) {
		h2 = h2->next;
	}
	T* toBeDeleted = h2->next;
	h2->next = nullptr;
	delete toBeDeleted;

	counter--;
	return head;
}

template<typename T>
void DeleteAll(T* head, int& counter) {
	T* h2 = head->next;
	while (h2 != nullptr) {
		delete head;
		head = h2;
		h2 = h2->next;
	}

	delete head;

	counter = 0;
}

//Linked List operation End

