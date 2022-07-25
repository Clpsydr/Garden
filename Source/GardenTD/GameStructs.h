#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "Engine/DataTable.h"
#include "GameStructs.generated.h"

//thatll need character struct to reduce text repetition

USTRUCT(BlueprintType)
struct FDialogueText : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "General")
		FString TextLabel;

	UPROPERTY(EditDefaultsOnly, Category = "Char")
		TSoftObjectPtr<UTexture2D> Portrait;

	UPROPERTY(EditDefaultsOnly, Category = "Char")
		FString CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "General")
		FString MainText;


};

