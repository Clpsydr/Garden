#include "TalkingActor.h"
#include "DialogBoxWidget.h"
#include "HomeBase.h"
#include "Kismet/GameplayStatics.h"

ATalkingActor::ATalkingActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATalkingActor::BeginPlay()
{
	Super::BeginPlay();

	AHomeBase* Base = Cast<AHomeBase>(UGameplayStatics::GetActorOfClass(GetWorld(), AHomeBase::StaticClass()));
	if (Base)
	{
		Base->OnGameEvent.AddDynamic(this, &ThisClass::RememberNewMessage);
	}
}

//preparing message and keeping it in stack, calling it if message log is not busy
void ATalkingActor::RememberNewMessage(FString RowName)
{
	if (DialogueData)
	{
		UDialogBoxWidget* NewDialogueWindow = CreateWidget<UDialogBoxWidget>(GetWorld(), VNWindowType);
		FDialogueText* SomeText = DialogueData->FindRow<FDialogueText>(FName(RowName), "TextLabel");

		if (SomeText)
		{
			NewDialogueWindow->SyncData(*SomeText);
			MessageBackLog.Add(NewDialogueWindow);

			if (MessageBackLog.Num() == 1)
			{

				LaunchNextMessage();
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red,
				TEXT("Couldnt find this particular Row: " + RowName));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red,
			TEXT("DialogueData is not ready"));
	}
}

//calling first message in stack and binding to search again for the next one, once its detached
//TODO: two messages in a row will "hang", because stack size doesnt account for currently shown ui
//
void ATalkingActor::LaunchNextMessage()
{
	if (MessageBackLog.Num() > 0)
	{
		UDialogBoxWidget* ThisWidget = MessageBackLog[0];
		MessageBackLog.Remove(ThisWidget);
		ThisWidget->AddToViewport();

		GetWorldTimerManager().SetTimer(HandleToShutWidget, ThisWidget,
			&UDialogBoxWidget::ShutWithNotification, ShowDuration, false);

		ThisWidget->OnMsgShutdown.AddDynamic(this, &ThisClass::LaunchNextMessage);
	}
}

