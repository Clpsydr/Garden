#include "TalkingActor.h"
#include "DialogBoxWidget.h"

ATalkingActor::ATalkingActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATalkingActor::BeginPlay()
{
	Super::BeginPlay();

	//FDialogueText* SomeText = DialogueData->FindRow<FDialogueText>(FName(TEXT("intro")), "TextLabel");
	//SayDialogueLine(*SomeText); //
	SayDialogueLine("intro", "TextLabel");
}

void ATalkingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//however instead of adding to viewport keeps a list of those
//sends other after timer, which should have the same TTL on change as one sent to dialogue window for closure

//const FDialogueText& NewLineToSay
void ATalkingActor::SayDialogueLine(FString RowName, FString ContextString)   //
{
	if (DialogueData)
	{
		FDialogueText* SomeText = DialogueData->FindRow<FDialogueText>(FName(RowName), ContextString);
		if (SomeText)
		{

			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
				TEXT("Called dialogue box"));

			UDialogBoxWidget* NewDialogueWindow = CreateWidget<UDialogBoxWidget>(GetWorld(), VNWindowType);
			NewDialogueWindow->AddToViewport(50);
			NewDialogueWindow->SyncData(*SomeText);

			GetWorldTimerManager().SetTimer(HandleToShutWidget, NewDialogueWindow, &UDialogBoxWidget::RemoveFromParent, ShowDuration, false);
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
			TEXT("DataTable unavailable"));
	}
}


