#include "NavigationComponent.h"

UNavigationComponent::UNavigationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNavigationComponent::BeginPlay()
{
	Super::BeginPlay();
}

AActor* UNavigationComponent::GiveDirection(bool bIsRandom)
{
	if (Directions.Num() == 0)
	{
		return nullptr;
	}

	if (bIsRandom)
	{
		return Directions[FMath::RandRange(0, Directions.Num() - 1)];
	}
	else
	{
		return Directions[0];
	}
}

