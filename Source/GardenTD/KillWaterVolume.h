#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PhysicsVolume.h"
#include "KillWaterVolume.generated.h"

/**
 * 
 */
UCLASS()
class GARDENTD_API AKillWaterVolume : public APhysicsVolume
{
	GENERATED_BODY()

	void ActorEnteredVolume(AActor* Other);
};
