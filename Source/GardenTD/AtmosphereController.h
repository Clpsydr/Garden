#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "AtmosphereController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDusk);

class ADirectionalLight;

/**
* Atmosphere data aggregation and control
*/
UCLASS(Blueprintable)
class GARDENTD_API AAtmosphereController : public AInfo
{
	GENERATED_BODY()
public:
	AAtmosphereController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	float MemorizedSunPosition = 0.f;


public:
	UPROPERTY(BlueprintAssignable)
		FOnDawn OnDawn;

	UPROPERTY(BlueprintAssignable)
		FOnDusk OnDusk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Inputs")
		float DayCycleSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Inputs")
		AActor* NightSkyDome;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Inputs")
		ADirectionalLight* Sunlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Inputs")
		ADirectionalLight* Moonlight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Inputs")
		AActor* SkySphere;

	UPROPERTY(BlueprintReadWrite)
		float CurrentSunPosition = 0.f;

	float TotalTimeElapsed;

	UFUNCTION()
		void DayNightCycle();

	UFUNCTION(BlueprintCallable, Category = "Atmosphere", meta =
		(WorldContext = "WorldContextObject", DisplayName = "DayNightCycle"))
	bool DayNightCycle_bp(const UObject* WorldContextObject, float CycleSpeed,
		AActor* DirectionalLight, AActor* SkyStars);

	float GetDaytime();
	
};
