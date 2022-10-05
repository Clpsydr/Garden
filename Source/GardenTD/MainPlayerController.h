#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPawnIssued, AActor*, NewPlayerPawn);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> GameoverUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<UUserWidget> HudTypeUI;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void CameraTurnX(float AxisValue);
	void CameraTurnY(float AxisValue);

	void Interact();
	void Jump();
	void Fire();
	void StopJumping();
	void StopFiring();
	void Respawn();
	void EnableDashing();
	void DisableDashing();

	UFUNCTION(BlueprintCallable)
		void ChangePawnTo(APlayerCharacter* NewPawn);

	UFUNCTION(BlueprintCallable)
		void InitHUD();

	UFUNCTION(BlueprintCallable)
		UUserWidget* GetPlayerStatusWindow() { return HudUI; }

	//opens or closes widget depending on its state
	void CallMenu();

	FOnNewPawnIssued OnNewPawnIssued;

protected:
	UPROPERTY(BlueprintReadWrite)
		APlayerCharacter* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Defaults")
		TSubclassOf<APlayerCharacter> OriginalPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Rules")
		bool bIsGameOver = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Control params")
		int InteractRange = 5000;

	virtual void BeginPlay() override;

	void ResetAction() { bIsReadyToInteract = true;  };

	UFUNCTION()
		void GameoverState();

	UFUNCTION()
		void ActiveState();

	//widgets
	UPauseMenuWidget* MainMenu;
	UUserWidget* GameoverMenu;
	UUserWidget* HudUI;
};
