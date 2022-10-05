#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationComponent.generated.h"

/*
* Actor that provides a set of other actor references 
* Not very good solution for openworld, as they cannot hold cross-level references and therefore all chunked into one level instance
* Should be replaced with just fvector once again
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GARDENTD_API UNavigationComponent : public UActorComponent
{
	GENERATED_BODY()



public:	
	UNavigationComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Possible directions to go")
	TArray<AActor*> Directions;

protected:
	virtual void BeginPlay() override;

public:
	//returns first or any direction, or none
	AActor* GiveDirection(bool bIsRandom);

	//interface that allows other actors to refer to the actor and figure out if it has this component
	//therefore calling methods from it
};
