#include "HomeBase.h"
#include "BattleStatComponent.h"
#include "Hive.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"

AHomeBase::AHomeBase()
{
	PrimaryActorTick.TickInterval = 0.3f;
	PrimaryActorTick.bCanEverTick = true;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	RootComponent = HitCollider;

	HomeArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Home zone"));
	HomeArea->SetupAttachment(RootComponent);
	HomeArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerOverlapBegin);

	BaseParams = CreateDefaultSubobject<UBattleStatComponent>(TEXT("Base stats"));
	BaseParams->OnDeath.AddDynamic(this, &ThisClass::AnnounceGameOver);
}

void AHomeBase::BeginPlay()
{
	Super::BeginPlay();
	
//scouring for available meshes
	//TODO: this should be reliant on special actor class that retains ID and therefor has its own position
	int i = 0;
	for (UActorComponent* ThisMesh : GetComponentsByClass(UStaticMeshComponent::StaticClass()))
	{
		i++;
		GoalBits.Add(i, Cast<UStaticMeshComponent>(ThisMesh));
	}

	for (auto& Bit : GoalBits)
	{
		CollectedPieces += Bit.Value->IsVisible();
	}

}

void AHomeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	BaseParams->RegenerateHP();
}

void AHomeBase::UpdateGoalsWith(int NewID)
{
	UStaticMeshComponent* CurrentBit = GoalBits.FindRef(NewID);
	
	if (CurrentBit)
	{
		CurrentBit->SetVisibility(true);

		CollectedPieces++;

		if (OnGameEvent.IsBound())
		{
			OnGameEvent.Broadcast("KeyPickup");
		}

		CheckVictoryConditions();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Red,
			TEXT("Didn't find any portal bits with that ID somehow"));
	}
}

void AHomeBase::Hurt(const FCombatBlob& DamageInfo)
{
	BaseParams->TakeDamage(DamageInfo);
}

//for now, just a message from Narrator
void AHomeBase::AnnounceGameOver(AActor* Address)
{
	
	if (OnGameEvent.IsBound())
	{
		OnGameEvent.Broadcast("GameWin");
	}
}

void AHomeBase::CheckVictoryConditions()
{
	if (GoalBits.Num() == CollectedPieces && BaseParams->EnergyReward >= EnergyVictoryCondition)
	{
		if (OnGameEvent.IsBound())
		{
			OnGameEvent.Broadcast("GameOver");
		}
	}
}

void AHomeBase::GainEnergy(const float EnergyAmount)
{
	BaseParams->EnergyReward += EnergyAmount;
	CheckVictoryConditions();
}

void AHomeBase::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (OtherActor == PlayerPawn)
	{
		float NrgTransaction = Cast<APlayerCharacter>(PlayerPawn)->GetEnergy();
		GainEnergy(NrgTransaction);
		Cast<APlayerCharacter>(PlayerPawn)->AddEnergy(-NrgTransaction);

		if (OnGameEvent.IsBound())
		{
			OnGameEvent.Broadcast("WelcomeBack");
		}
	}
	else
		if (Cast<AHive>(OtherActor))
	{
			if (OnGameEvent.IsBound())
			{
				OnGameEvent.Broadcast("EnemyClose");
			}
	}

}
