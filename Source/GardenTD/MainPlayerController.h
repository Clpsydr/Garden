#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UPauseMenuWidget;

UCLASS()
class GARDENTD_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UPauseMenuWidget> MainMenuSample;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void CameraTurnX(float AxisValue);
	void CameraTurnY(float AxisValue);

	void Interact();
	void Jump();
	void StopJumping();

	//opens or closes widget depending on its state
	void CallMenu();

	//then, message tool
	//actor launches dialogue widget and auto (then within touch)
	//msg tool has table from dialogue struct, with all quotes, and plays the first one.

	//3d model + texture
	//2 sounds in distinct languages
	

protected:
	APawn* PlayerPawn;

	virtual void BeginPlay() override;

	//widget menu
	UPauseMenuWidget* MainMenu;
};
