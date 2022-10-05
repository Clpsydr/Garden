#include "KillWaterVolume.h"
#include "Combatable.h"

void AKillWaterVolume::ActorEnteredVolume(AActor* Other)
{
	if (Cast<ICombatable>(Other))
	{
		Cast<ICombatable>(Other)->HazardDamage(10000.f);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Something in the water, but we cannot hurt it"));
	}
}