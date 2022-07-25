#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
class UComboBoxString;
class UVerticalBox;

/*
* Menu showing up on escape trigger
*/
UCLASS()
class GARDENTD_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* ReturnButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* RestartButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* OptionsButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UButton* QuitButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UComboBoxString* LangSelectBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UVerticalBox* LangBox;

	UFUNCTION()
		void Open();

	UFUNCTION()
		void Close();

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void ChangeLocale(FString SelectedItem, ESelectInfo::Type SelectType);

	UFUNCTION()
		void Options();
};
