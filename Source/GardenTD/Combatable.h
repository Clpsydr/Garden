#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStructs.h"
#include "Combatable.generated.h"

UINTERFACE(MinimalAPI)
class UCombatable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for reacting to damaging gameplay elements (harmful zones, bullets, melee zones)
 */
class GARDENTD_API ICombatable
{
	GENERATED_BODY()

public:
	virtual void Hurt(const FCombatBlob& DamageInfo) = 0;

	virtual void HazardDamage(const float AbsoluteDamage) = 0;

	virtual void GainEnergy(const float EnergyAmount) = 0;
};
