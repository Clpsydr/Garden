// Fill out your copyright notice in the Description page of Project Settings.
#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton)
	{
		ReturnButton->OnPressed.AddDynamic(this, &ThisClass::Close);
	}

	if (RestartButton)
	{
		RestartButton->OnPressed.AddDynamic(this, &ThisClass::Open);
	}

	if (OptionsButton)
	{
		OptionsButton->OnPressed.AddDynamic(this, &ThisClass::Options);
	}

	if (QuitButton)
	{
		QuitButton->OnPressed.AddDynamic(this, &ThisClass::QuitGame);
	}

	if (LangSelectBox)
	{
		LangSelectBox->OnSelectionChanged.AddDynamic(this, &ThisClass::ChangeLocale);
	}

	if (LangBox)
	{
		LangBox->SetVisibility(ESlateVisibility::Hidden);

		
	}
}


void UPauseMenuWidget::Open()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Reopening level"));

	UGameplayStatics::OpenLevel(GetWorld(), "BaseMap");
}

void UPauseMenuWidget::Close()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Closing Menu"));

	AMainPlayerController* ThisCtrlr = Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (ThisCtrlr)
	{
		ThisCtrlr->CallMenu(); 
	}

}

void UPauseMenuWidget::QuitGame()
{
	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Quitting game"));

	FGenericPlatformMisc::RequestExit(false);
}
					   
void UPauseMenuWidget::ChangeLocale(FString SelectedItem, ESelectInfo::Type SelectType) 
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Changing Locale to " + SelectedItem));

	if (SelectedItem == "Russian")
	{
		FInternationalization::Get().SetCurrentCulture(TEXT("ru"));
	}
	else if (SelectedItem == "English")
	{
		FInternationalization::Get().SetCurrentCulture(TEXT("en"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("unsupported locale"));
	}
}

void UPauseMenuWidget::Options()
{
	if (LangBox)
	{
		if (LangBox->IsVisible())
		{
			LangBox->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			LangBox->SetVisibility(ESlateVisibility::Visible);

			FCulturePtr CuCulture = FInternationalization::Get().GetCurrentCulture();
			if (CuCulture.IsValid())
			{
				GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
					TEXT("current culture: " + CuCulture->GetEnglishName()));
				
				LangSelectBox->SetSelectedOption(CuCulture->GetEnglishName());
			}
		}
	}
}