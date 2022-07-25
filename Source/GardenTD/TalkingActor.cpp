#include "TalkingActor.h"
#include "DialogBoxWidget.h"

ATalkingActor::ATalkingActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATalkingActor::BeginPlay()
{
	Super::BeginPlay();

	if (DialogueData)
	{
		FDialogueText* SomeText = DialogueData->FindRow<FDialogueText>(FName(TEXT("intro")), "TextLabel");
		SayDialogueLine(*SomeText); //
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red,
			TEXT("DataTable unavailable"));
	}
}

// Called every frame
void ATalkingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATalkingActor::SayDialogueLine(const FDialogueText& NewLineToSay)   //
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Called dialogue box"));

	UDialogBoxWidget* NewDialogueWindow = CreateWidget<UDialogBoxWidget>(GetWorld(), VNWindowType);
	NewDialogueWindow->AddToViewport(50);
	NewDialogueWindow->SyncData(NewLineToSay);  
	
	GetWorldTimerManager().SetTimer(HandleToShutWidget, NewDialogueWindow, &UDialogBoxWidget::RemoveFromParent, ShowDuration, false);
}


