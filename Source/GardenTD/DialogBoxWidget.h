#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStructs.h"
#include "DialogBoxWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMsgShutdown);

class UImage;
class UTextBlock;

/*
* Launches a widget displaying text from the datatable
*/
UCLASS()
class GARDENTD_API UDialogBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
		UPROPERTY(BlueprintReadOnly, Meta = (Bindwidget))
			UImage* Portrait;

		UPROPERTY(BlueprintReadOnly, Meta = (Bindwidget))
			UTextBlock* SpeakerName;

		UPROPERTY(BlueprintReadOnly, Meta = (Bindwidget))
			UTextBlock* MainText;

		UPROPERTY()
			FDialogueText TextData;

		//good to pass by delegate as well
		UFUNCTION()
			void SyncData(const FDialogueText& NewData);

		//operation to display data

		//launch timer to autoshutdown
		UFUNCTION()
			void ShutWithNotification();

		FOnMsgShutdown OnMsgShutdown;
};
