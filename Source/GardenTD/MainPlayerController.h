#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UPauseMenuWidget;
class APlayerCharacter;

UCLASS()
class GARDENTD_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

private:


protected:

	bool bIsReadyToInteract = true;

	float ActionCooldown = 0.5f;

	FTimerHandle ActionTimerHandle;

public:
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UPauseMenuWidget> MainMenuSample;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void CameraTurnX(float AxisValue);
	void CameraTurnY(float AxisValue);

	void Fly(float AxisValue);

	void Interact();
	void Jump();
	void Fire();
	void StopJumping();
	void StopFiring();

	UFUNCTION(BlueprintCallable)
		void ChangePawnTo(APlayerCharacter* NewPawn);

	//opens or closes widget depending on its state
	void CallMenu();

protected:
	UPROPERTY(BlueprintReadWrite)
		APlayerCharacter* PlayerPawn;

	UPROPERTY(BlueprintReadWrite)
		APlayerCharacter* OriginalPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control params")
		int InteractRange = 5000;

	virtual void BeginPlay() override;

	void ResetAction() { bIsReadyToInteract = true;  };

	//widget menu
	UPauseMenuWidget* MainMenu;
};
