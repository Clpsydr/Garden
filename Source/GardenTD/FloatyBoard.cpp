#include "FloatyBoard.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "Camera/CameraComponent.h"
#include "../../Plugins/Experimental/Water/Source/Runtime/Public/BuoyancyComponent.h"

/*
* controlled pawn with default buoyancy
* pontoons will need to be properly set in blueprints 
*/
AFloatyBoard::AFloatyBoard()
{
	PrimaryActorTick.bCanEverTick = true;

	FloatingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floating Body Mesh"));
	RootComponent = FloatingMesh;
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));

	Camera->SetupAttachment(RootComponent);
}

void AFloatyBoard::BeginPlay()
{
	Super::BeginPlay();
}

void AFloatyBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloatyBoard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// Unused: 
// system can create pontoons, but would crash with Spring related errors on buoyancy activation
// if more than 2 pontoons were added.
void AFloatyBoard::FloatMode()
{
	Buoyancy->AddCustomPontoon(15.f, FVector(50.f, 0.f, 1.f));
	Buoyancy->AddCustomPontoon(15.f, FVector(-50.f, -25.f, 1.f));
	Buoyancy->Activate();
}

//Unused:
// while its possible to delete pontoons in deactivated state, reactivating is not possible 
// without Spring related crash
// perhaps needs a component update or something similar.
void AFloatyBoard::SinkMode()
{
	for (auto BuoyData : Buoyancy->BuoyancyData.Pontoons)
	{
		//TArray<FSphericalPontoon>
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple,
			TEXT("Pontoon: " + BuoyData.CenterLocation.ToString()));
		
	}

	Buoyancy->Deactivate();
	Buoyancy->BuoyancyData.Pontoons.Empty();
	//Buoyancy->Activate();
	//Buoyancy->Update()
	/*FSphericalPontoon Pontoon;
	Pontoon.Radius = Radius;
	Pontoon.CenterSocket = CenterSocketName;
	BuoyancyData.Pontoons.Add(Pontoon);*/
}

void AFloatyBoard::DriveMode(ACharacter* PreviousChar)
{
	APlayerController* ThisController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	Cast<AMainPlayerController>(ThisController)->ChangePawnTo(this);
}

void AFloatyBoard::MoveSide(float AxisValue)
{
	//this seems to rely too much on collider and general pawn shape
	if (AxisValue != 0.f)
	{
		FloatingMesh->AddAngularImpulse(AxisValue * GetActorRightVector(), NAME_None, true);
	}
	
}

void AFloatyBoard::MoveFront(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		
		FloatingMesh->AddForce(AxisValue * GetActorForwardVector() * BoatVelocity, NAME_None,true);
	}
}

