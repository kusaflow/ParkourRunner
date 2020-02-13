// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager_01.h"
#include "LevelCreationBase.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "myGameInstance.h"
#include "Math/UnrealMathUtility.h"

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
LinkedList_Actor* head_Total_Blocks = new LinkedList_Actor();
//------------------------------------------------


//-----------Testing============================
void kusa(LL_Actor_Node* head) {
	int x = 0;
	while (head != nullptr) {
		if (x % 2 == 0) {
			head->actor->SetActorScale3D(FVector(1.0f, 2.0f, 3.0f));
		}
		
		x++;
		head = head->next;
	}
}
//==============================================


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

	locationToDrawblock_X = 500.0f;
}

ALevelManager_01 :: ~ALevelManager_01() {
	LL_Actor_Node* h2;
	while (head_Total_Blocks->head != nullptr) {
		h2 = head_Total_Blocks->head;
		head_Total_Blocks->head = head_Total_Blocks->head->next;
		delete h2;
	}

	DeleteAllInts();
}


// Called when the game starts or when spawned
void ALevelManager_01::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("==========================================kusaflow=========================================="));
	gameInstance = Cast<UmyGameInstance>(GetGameInstance());

	//init level creation of level
	createNewBlocksMngr();
	FirstRun = true;

	//kusa(head_Total_actor_01->head);

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

int CountList() {
	int x=0;
	LL_Actor_Node* h2 = head_Total_Blocks->head;
	while (h2 != nullptr) {
		x++;
		h2 = h2->next;
	}

	return x;
}

// Called every frame
void ALevelManager_01::Tick(float DeltaTime)
{
	gameInstance = Cast<UmyGameInstance>(GetGameInstance());
	//return;

	if (DoDrawBlocks()) {

		UE_LOG(LogTemp, Warning, TEXT("%d"), CountList())
		RemoveDataForNew();

		UE_LOG(LogTemp, Warning, TEXT("%d"), CountList())

		//creating new blocks
		createNewBlocksMngr();

		UE_LOG(LogTemp, Warning, TEXT("%d"), CountList())

		//clear the integer list
		DeleteAllInts();		
	}

	//removing actor that has no need in the scene
	if ((float)(gameInstance->sensorsClassQueue.front().x + gameInstance->sensorsClassQueue.front().sizeX) + 10 <=
		gameInstance->MainActorLocation.X) {
		//--------------------------------------
		if (!gameInstance->sensorsClassQueue.empty() && !taskSensorsActor.empty()) {
			gameInstance->sensorsClassQueue.pop();
			taskSensorsActor.front()->Destroy();
			taskSensorsActor.pop();
		}
	}


	
	//UE_LOG(LogTemp, Warning, TEXT("%d ---------------- %d" ), *gameInstance->MainActorLocation.X,midVal);




}

void ALevelManager_01::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	//DeleteAll<int>
}

// runs on every tick to check do we have to create new files or not
bool ALevelManager_01::DoDrawBlocks() {
	if (gameInstance->MainActorLocation.X >= midVal)
		return true;
	return false;
}

//create new blocks=======================================================================

void ALevelManager_01::createNewBlocksMngr() {
	int counterOfActorsOnScreen = 0;
	if (head_LevelDesigner->head != nullptr) {
		DeleteAllInts();
	}
	
	int val = 0;
	while (counterOfActorsOnScreen < gameInstance->ActorsToDrawAtATime) {
		val = GenerateRandomLevelCreationTypes();
		counterOfActorsOnScreen += BlockCount(val);
		insertInt(val);
		
	}

	if (FirstRun) {
		BlocksOnFront = counterOfActorsOnScreen;
		goto ForFirstRunCheckPoint;
	}

	BlocksOnBack = BlocksOnFront;
	BlocksOnFront = counterOfActorsOnScreen;


	//UE_LOG(LogTemp, Warning, TEXT("==========================Clear 1ithar tk ================================"));

	ForFirstRunCheckPoint:

	initVal = locationToDrawblock_X;

	//now generating the levels
	LL_INT_Node* h2 = head_LevelDesigner->head;
	while (h2 != nullptr) {
		createTheBlock(h2->data);

		//UE_LOG(LogTemp, Warning, TEXT("%d"), x);

		h2 = h2->next;
	}

	finalVal = locationToDrawblock_X;				

	midVal = (finalVal - initVal)/2;
	midVal += initVal;

}

void ALevelManager_01::createTheBlock(const int type) {
	//here we create the block type
	ALevelCreationBase* actor;
	FActorSpawnParameters spawnPara;
	spawnPara.Owner = this;
	//UE_LOG(LogTemp, Warning, TEXT("==========================================Block is clear=========================================="));

	if (Block_001 && Block_002 && Block_003 && Block_004 && Block_005) {
		UWorld* world = GetWorld();
		if (world) {
			//AddSensor
			AddSensors(type, locationToDrawblock_X, world);
			//----------------selection--------------
			if (type == 1) {
				//block 1
				RandomBlock();
				locationToDrawblock_X += 400;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(4, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 400;
			}
			else if (type == 2) {
				//0:17
				//block 1
				RandomBlock();
				locationToDrawblock_X += 800;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(8, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 800;
				//block2
				RandomBlock();
				locationToDrawblock_X += 1030;
				locationToDrawblock_X += 50;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -980.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(0.5f, 1.4f, 1.15f));
				insertActor(actor);
				locationToDrawblock_X += 50;
				//block3
				RandomBlock();
				locationToDrawblock_X += 400;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -980.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(4, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 400;
				//block 4
				RandomBlock();
				locationToDrawblock_X += 1210;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -980.0f), FRotator(10, 0, 0), spawnPara);
				actor->SetActorScale3D(FVector(12, 1.4f, 1.34f));
				insertActor(actor);
				locationToDrawblock_X += 1000;
			}
			else if (type == 3) {
				//block 1
				RandomBlock();
				locationToDrawblock_X += 300;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(3, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 300;
				//block2
				RandomBlock();
				locationToDrawblock_X += 150;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(1.5f, 1.4f, 1.45f));
				insertActor(actor);
				locationToDrawblock_X += 150;
				//block3
				RandomBlock();
				locationToDrawblock_X += 640;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(-10, 0, 0), spawnPara);
				actor->SetActorScale3D(FVector(8.0f, 1.4f, 1.235f));
				insertActor(actor);
				locationToDrawblock_X += 640;
			}
			else if (type == 4) {
				//block 1
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block2
				RandomBlock();
				locationToDrawblock_X += 470;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1.5f));
				insertActor(actor);
				locationToDrawblock_X += 200;
			}
			else if (type == 5) {
				//block 1
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block2
				RandomBlock();
				locationToDrawblock_X += 470;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1.5f));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block3
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
			}
			else if (type == 6){
				unsigned int orgPos = locationToDrawblock_X;
				//block 1
				RandomBlock();
				locationToDrawblock_X += 400;
				locationToDrawblock_X += 100;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, 80.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(1, 0.5f, 0.1f));
				insertActor(actor);
				locationToDrawblock_X += 400;
				//block2
				RandomBlock();
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, 80.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(1, 0.5f, 0.1f));
				insertActor(actor);
				locationToDrawblock_X += 400;
				//block 3
				RandomBlock();
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, 80.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(1, 0.5f, 0.1f));
				insertActor(actor);
				locationToDrawblock_X += 400;
				//block 4
				RandomBlock();
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, 80.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(1, 0.5f, 0.1f));
				insertActor(actor);

				//main block
				RandomBlock();
				locationToDrawblock_X = orgPos;
				locationToDrawblock_X += 1200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(12, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 1200;
			}
			else if (type == 7) {
				//one gap only block
				//block 1
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block2
				RandomBlock();
				locationToDrawblock_X += 500;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
			}
			else if (type == 8) {
				//3 gap block
				//total 4 blocks
				//block 1
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block2
				RandomBlock();
				locationToDrawblock_X += 500;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block3
				RandomBlock();
				locationToDrawblock_X += 500;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block4
				RandomBlock();
				locationToDrawblock_X += 500;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
			}
			else if (type == 9) {
				//3 blocks where the middle one is a bit lower than others
				//block 1
				RandomBlock();
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block2
				RandomBlock();
				locationToDrawblock_X += 400;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 0.8f));
				insertActor(actor);
				locationToDrawblock_X += 200;
				//block3
				RandomBlock();
				locationToDrawblock_X += 400;
				locationToDrawblock_X += 200;
				actor = world->SpawnActor<ALevelCreationBase>(Block, FVector(locationToDrawblock_X, 0, -580.0f), FRotator(0), spawnPara);
				actor->SetActorScale3D(FVector(2, 1.4f, 1));
				insertActor(actor);
				locationToDrawblock_X += 200;

			}
			


		}

	}
}

//---------------------------

//======================================================================================================================
//remove the actors from the screen ........clearing memory
 

void ALevelManager_01 :: RemoveDataForNew() {
	if (FirstRun) {
		FirstRun = false;
		return;
	}
	removeActorsFromGame();
	//DeleteLast50Actors();
}

void ALevelManager_01 :: removeActorsFromGame() {
	if (head_Total_Blocks->head == nullptr) {
		return;
	}

	LL_Actor_Node* h2 = head_Total_Blocks->head;
	LL_Actor_Node* h3 = h2;

	for (int i = 0; i < BlocksOnBack; i++) {
		if (h2 == nullptr) {
			return;
		}
		h3 = h2;
		if (h2) {
			h2->actor->Destroy();
			h2 = h2->next;
			head_Total_Blocks->head = h2;
			delete h3;
		}
	}

}
//-----------------------------------------


int ALevelManager_01::GenerateRandomLevelCreationTypes() {
	//creating this as seperate for future if i have to create a logic to make complex randomness
	//type 2
	//return 2;
	//return 3;
	//return 4;

	return 1;
	
	int x = (int)FMath::FRandRange(2, 9);

	return x;
	//return 1;
}

int ALevelManager_01::BlockCount(int type) {
	if (type == 1) {
		return 1;
	}
	if (type == 2) {
		return 4;
	}
	if (type == 3) {
		return 3;
	}
	if (type == 4) {
		return 2;
	}
	if (type == 5) {
		return 3;
	}
	if (type == 6) {
		return 5;
	}
	if (type == 7) {
		return 2;
	}
	if (type == 8) {
		return 4;
	}
	if (type == 9) {
		return 3;
	}

	return 0;
}


void ALevelManager_01 :: RandomBlock() {
	int r = (int)FMath::FRandRange(0, 5);
	if (r == 0) {
		Block = Block_001;
	}
	else if (r == 1) {
		Block = Block_002;
	}
	else if (r == 2) {
		Block = Block_003;
	}
	else if (r == 3) {
		Block = Block_004;
	}
	else if (r == 4) {
		Block = Block_005;
	}
}


//----------------------------------------------------------------------------------------



//LinkedListOperations-----------------------------------------------------------

void ALevelManager_01 :: insertInt(int data) {
	if (head_LevelDesigner->head == nullptr) {
		//list is empty so add this at head
		LL_INT_Node* newNode = new LL_INT_Node(data);
		head_LevelDesigner->head = newNode;
		return;
	}

	LL_INT_Node* newNode = new LL_INT_Node(data);
	LL_INT_Node* h2 = head_LevelDesigner->head;
	while (h2->next != nullptr) {
		h2 = h2->next;
	}
	h2->next = newNode;
}

void ALevelManager_01 :: insertActor(ALevelCreationBase* data) {
	if (head_Total_Blocks->head == nullptr) {
		//list is empty so add this at head
		LL_Actor_Node* newNode = new LL_Actor_Node(data);
		head_Total_Blocks->head = newNode;
		return;
	}

	LL_Actor_Node* newNode = new LL_Actor_Node(data);
	LL_Actor_Node* h2 = head_Total_Blocks->head;
	while (h2->next !=  nullptr) {
		h2 = h2->next;
	}
	h2->next = newNode;
}

void ALevelManager_01 :: DeleteLast50Actors() {
	if (head_Total_Blocks->head == nullptr) {
		return;
	}

	LL_Actor_Node* h2 = head_Total_Blocks->head;

	for (int i = 0; i < BlocksOnBack; i++) {
		if (h2 == nullptr) {
			head_Total_Blocks->head = nullptr;
			return;
		}

		delete head_LevelDesigner->head;
		head_Total_Blocks->head = h2;
		h2 = h2->next;
	}
}

void ALevelManager_01 :: DeleteAllInts() {
	if (head_LevelDesigner->head == nullptr)
		return;

	LL_INT_Node* h2 = head_LevelDesigner->head->next;
	while (h2 != nullptr) {
		delete head_LevelDesigner->head;
		head_LevelDesigner->head = h2;
		h2 = h2->next;
	}

	delete head_LevelDesigner->head;

	head_LevelDesigner->head = nullptr;
}

//Linked List operation End


void ALevelManager_01 :: AddSensors(int type, unsigned int posX, UWorld* world) {
	AActor* actor;
	FActorSpawnParameters spawnPara;
	spawnPara.Owner = this;

	//SIZE of sensor for now is 200x200x200
	UmyGameInstance::sensorClass sensorClassObj;

	if (world && sensorMesh) {
		if (type == 1) {
			actor = world->SpawnActor<AActor>(sensorMesh, FVector(posX+400, 0, 90), FRotator(0), spawnPara);	
			taskSensorsActor.push(actor);
			sensorClassObj.x = posX + 400;
			sensorClassObj.y = 90;
			sensorClassObj.sizeX = 100;
			sensorClassObj.sizeY = 100;
		}
		else if (type == 2) {
			actor = world->SpawnActor<AActor>(sensorMesh, FVector(posX + 1500, 0, 70), FRotator(0), spawnPara);
			taskSensorsActor.push(actor);
		}


	}

	//pushing the task or action here--------------
	if (type == 1) {
		sensorClassObj.task = 1;
	}
	//=============================================
	
	gameInstance->sensorsClassQueue.push(sensorClassObj);

}

