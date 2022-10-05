#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerCharacter.h"
#include "PhysicalInteractable.generated.h"

UINTERFACE(MinimalAPI)
class UPhysicalInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface to apply various effects of players' E (interact) button
 * also includes physical interaction
 */
class GARDENTD_API IPhysicalInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PhysicalInterface")
		void ContextualPhysicalEffect(const APlayerCharacter* PChar, 
												const FVector& HitImpulse, 
												const FVector& HitLocation );

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PhysicalInterface")
		void ContextualOperate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PhysicalInterface")
		void ContextualDetonate();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "PhysicalInterface")
		void ContextualGrab(const APlayerCharacter* ThisPlayer);
};
