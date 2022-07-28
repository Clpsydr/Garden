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

