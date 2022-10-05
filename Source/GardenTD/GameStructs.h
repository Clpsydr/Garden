#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "GameStructs.generated.h"

//also needs character struct to reduce text repetition
//also needs FTEXT/FNAMe for text due to localization

USTRUCT(BlueprintType)
struct FDialogueText : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "General")
		FName TextLabel;

	UPROPERTY(EditDefaultsOnly, Category = "Char")
		TSoftObjectPtr<UTexture2D> Portrait;

	UPROPERTY(EditDefaultsOnly, Category = "Char")
		FName CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "General")
		FText MainText;
};

USTRUCT(BlueprintType)
struct FGaugeWidgetStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Gauge")
		FName Description;

	UPROPERTY(EditDefaultsOnly, Category = "Gauge")
		float CurrentValue;

	UPROPERTY(EditDefaultsOnly, Category = "Gauge")
		float MaxValue;

	UPROPERTY(EditDefaultsOnly, Category = "Gauge")
		FLinearColor GaugeColor;

	float GetRatio() { return CurrentValue / MaxValue; };
};

USTRUCT(BlueprintType)
struct FCombatBlob
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, Category = "damage params")
		float Value;

	UPROPERTY(EditDefaultsOnly, Category = "damage params")
		AActor* Killer;

	//possibly to add damage type and so
};

