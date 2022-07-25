#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MsgManager.generated.h"

UCLASS()
class GARDENTD_API AMsgManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMsgManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
