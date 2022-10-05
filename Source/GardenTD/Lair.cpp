#include "Lair.h"
#include "Unit.h"
#include "Hive.h"
#include "NavigationComponent.h"
#include "AtmosphereController.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystemComponent.h"
#include "BattleStatComponent.h"

ALair::ALair()
{
	PrimaryActorTick.bCanEverTick = true;

	LairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lair Mesh"));
	LairMesh->SetupAttachment(RootComponent);

	InitialNavigationSet = CreateDefaultSubobject<UNavigationComponent>(TEXT("NavSet"));

	AgroArea = CreateDefaultSubobject<USphereComponent>(TEXT("Agro Radius"));
	AgroArea->SetupAttachment(LairMesh);

	LairParams = CreateDefaultSubobject<UBattleStatComponent>(TEXT("Battle Stats"));
	LairParams->OnDeath.AddDynamic(this, &ThisClass::DisableWithDamage);

	SplatterEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Hurt Visuals"));
	SplatterEffect->SetupAttachment(LairMesh);
}

void ALair::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Atmosphere;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),  AAtmosphereController::StaticClass(), Atmosphere);

	//TODO: binding levelup to collectible pickup

	//binding a wake up on certain timezones
	if (Atmosphere.Num() > 0)
	{
		AAtmosphereController* TempAtmosphere = Cast<AAtmosphereController>(Atmosphere[0]);
		if (Nocturnal)
		{
			TempAtmosphere->OnDusk.AddDynamic(this, &ThisClass::Enable);
			TempAtmosphere->OnDawn.AddDynamic(this, &ThisClass::Disable);
		}
		else
		{
			TempAtmosphere->OnDusk.AddDynamic(this, &ThisClass::Disable);
			TempAtmosphere->OnDawn.AddDynamic(this, &ThisClass::Enable);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red,
			TEXT("Lair failed to find a weather manager"));
	}

	//setting up initial activity state
	if (Cast<AAtmosphereController>(Atmosphere[0])->GetDaytime() >= 0 && Nocturnal ||
		Cast<AAtmosphereController>(Atmosphere[0])->GetDaytime() < 0 && !Nocturnal)
	{
		Enable();
	}
	else
	{
		Disable();
	}
}

void ALair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALair::Enable()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple,
		TEXT("Waking up!"));
	
	//LairMesh->SetVisibility(true, true);
	//LairMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	PrimaryActorTick.bCanEverTick = true;

	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ThisClass::GenerateSpawn, SpawningRate, true);
}

void ALair::Disable()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Purple,
		TEXT("Going to sleep!"));

	//LairMesh->SetVisibility(false, true);
	//LairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PrimaryActorTick.bCanEverTick = false;

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
}

void ALair::LevelUp()
{
	DifficultyLevel++;
}

void ALair::GenerateSpawn()
{
	if (SpawnHiveType)
	{
		//should trace down to check available locations??? Doesnt matter for a hive probably
		APawn* NewHive;
		NewHive = GetWorld()->SpawnActor<APawn>(SpawnHiveType,
			LairMesh->GetComponentLocation() + FVector(FMath::RandRange(-200.f, 200.f), FMath::RandRange(-200.f, 200.f), LairMesh->GetComponentLocation().Z),
			FRotator());

		//setting up default hive params
		if (NewHive)
		{
			Cast<AHive>(NewHive)->SetDifficulty(DifficultyLevel);
			Cast<AHive>(NewHive)->AssignUnitType(SpawnUnitType);
			Cast<AHive>(NewHive)->SetNewDestination(InitialNavigationSet->GiveDirection(true));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("No lair blueprint selected"));
	}
}

void ALair::Hurt(const FCombatBlob& DamageInfo)
{
	LairParams->TakeDamage(DamageInfo);
	SplatterEffect->ActivateSystem();
}

void ALair::HazardDamage(const float AbsoluteDamage)
{}

void ALair::GainEnergy(const float EnergyAmount)
{}

void ALair::DisableWithDamage(AActor* Receiver)
{
	LairMesh->SetVisibility(false, true);
	LairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Disable();
	
	if (Cast<ICombatable>(Receiver))
	{
		Cast<ICombatable>(Receiver)->GainEnergy(Energy);
	}
	
	//actually turning off the lair, without triggering anything 
	//remains a problem with already bound operations
}

//TODO revival , timer restarts, score pertains
//TODO merge lair and hive as unit spawners, hive inherits lair with moving methods. AI control is the same for either class
