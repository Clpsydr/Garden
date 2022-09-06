#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogBoxWidget.h"
#include "GameStructs.h"
#include "TalkingActor.generated.h"

class UDataTable; 
class UDialogBoxWidget;

UCLASS()
class GARDENTD_API ATalkingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATalkingActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* DialogueData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UDialogBoxWidget> VNWindowType;

protected:
	virtual void BeginPlay() override;

	float ShowDuration = 5.f;

private:
	FTimerHandle HandleToShutWidget;
	FTimerDelegate WidgetTTL;

	//keeps a stack of called messages to show them in a row
	TArray<UDialogBoxWidget*> MessageBackLog;

public:	
	virtual void Tick(float DeltaTime) override;

	void SayDialogueLine(FString RowName, FString ContextString);  //
};
