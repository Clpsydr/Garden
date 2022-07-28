#include "DialogBoxWidget.h"
#include "GameStructs.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UDialogBoxWidget::SyncData(const FDialogueText& NewData)
{
	Portrait->SetBrushFromTexture(NewData.Portrait.LoadSynchronous());

	SpeakerName->SetText(FText::FromName(NewData.CharacterName));

	MainText->SetText(NewData.MainText);
}
