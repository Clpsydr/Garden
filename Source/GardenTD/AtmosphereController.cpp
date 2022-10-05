#include "AtmosphereController.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h" 

AAtmosphereController::AAtmosphereController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.8f;
}

void AAtmosphereController::BeginPlay()
{
	Super::BeginPlay();

	if (Sunlight)
	{
		MemorizedSunPosition = Sunlight->GetActorRotation().Pitch;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red,
			TEXT("Failure to detect any directional light"));
	}

	if (Moonlight)
	{
		Moonlight->SetBrightness(0.f);
	}
}

void AAtmosphereController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DayNightCycle();

	//TODO: interpolating light source positions
	
	//FRotator rotation = FRotator(DayCycleSpeed, 0, 0);
	//Door->SetRelativeRotation(FMath::Lerp(FQuat(DoorRotation), FQuat(FRotator(0.0f, RotateValue, 0.0f)), 0.01f));
	//Sunlight->SetActorRotation(FMath::Lerp(rotation, exp2(-0.2f * DeltaTime)), ETeleportType::None);
	
	///refresh material and interp light
}

void AAtmosphereController::DayNightCycle()
{
	FRotator rotation = FRotator(DayCycleSpeed, 0, 0);
	Sunlight->AddActorWorldRotation(rotation, false, 0, ETeleportType::None);
	CurrentSunPosition = Sunlight->GetActorRotation().Pitch;
	
	if (NightSkyDome)
	{
		NightSkyDome->AddActorWorldRotation(rotation, false, 0, ETeleportType::None);
	}

	/// pitch goes through zero twice a cycle, and what matters is the direction of the passage
	/// towards -30 to -20 night, towards -5 to +5 day
	/// better way would be using acos?  acos of dot product between zenith and sun position would be the angle
	if (CurrentSunPosition > -20.f && MemorizedSunPosition < -20.f)
	{
		if (OnDusk.IsBound())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue,
				TEXT("Dusk arrives"));

			if (Moonlight)
			{
				/// Moonlight->SetBrightness(FMath::Clamp(FMath::Abs(CurrentSunPosition - 5), 10.f, 0.f ));
				Moonlight->SetBrightness(10.f);
			}

			OnDusk.Broadcast();
		}
	}

	if (CurrentSunPosition < 20.f && MemorizedSunPosition > 20.f)
	{
		if (OnDawn.IsBound())
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Blue,
				TEXT("Dawn arrives"));

			if (Moonlight)
			{
				Moonlight->SetBrightness(0.f);
			}

			OnDawn.Broadcast();
		}
	}

	MemorizedSunPosition = CurrentSunPosition;
}

bool AAtmosphereController::DayNightCycle_bp(const UObject* WorldContextObject, float CycleSpeed,
	AActor* DirectionalLight, AActor* SkyStars)
{
	FRotator rotation = FRotator(CycleSpeed, 0, 0);
	DirectionalLight->AddActorWorldRotation(rotation, false, 0, ETeleportType::None);
	SkyStars->AddActorWorldRotation(rotation, false, 0, ETeleportType::None);

	return false;
}

float AAtmosphereController::GetDaytime()
{
	return Sunlight->GetActorRotation().Pitch;
}