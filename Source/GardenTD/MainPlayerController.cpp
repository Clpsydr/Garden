#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayerController::Interact);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMainPlayerController::StopJumping);
	InputComponent->BindAction("EscapeMenu", IE_Pressed, this, &AMainPlayerController::CallMenu);

	InputComponent->BindAxis("CharacterMoveFront", this, &AMainPlayerController::MoveForward);
	InputComponent->BindAxis("CharacterMoveRight", this, &AMainPlayerController::MoveRight);

	InputComponent->BindAxis("CameraMoveX", this, &AMainPlayerController::CameraTurnX);
	InputComponent->BindAxis("CameraMoveY", this, &AMainPlayerController::CameraTurnY);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerCharacter>(GetPawn());
}

void AMainPlayerController::MoveForward(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorForwardVector(), AxisValue);
	}
}

void AMainPlayerController::MoveRight(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->AddMovementInput(PlayerPawn->GetActorRightVector(), AxisValue);
	}
}

void AMainPlayerController::CameraTurnX(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->AddControllerYawInput(AxisValue);
	}
}

void AMainPlayerController::CameraTurnY(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->AddControllerPitchInput(AxisValue);
	}
}

void AMainPlayerController::Interact()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Interact button pressed"));
}

void AMainPlayerController::Jump()
{
	if (PlayerPawn && Cast<ACharacter>(PlayerPawn))
	{
		Cast<ACharacter>(PlayerPawn)->Jump();
	}
}

void AMainPlayerController::StopJumping()
{
	if (PlayerPawn && Cast<ACharacter>(PlayerPawn))
	{
		Cast<ACharacter>(PlayerPawn)->StopJumping();
	}
}

void AMainPlayerController::CallMenu()
{
	/// still needs a pause
	if (MainMenu)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
			TEXT("Menu purged"));

		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		bShowMouseCursor = false;
		SetInputMode(Mode);

		MainMenu->RemoveFromParent();
		MainMenu = nullptr;

		//SetPause(false, FCanUnpause());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
			TEXT("Menu called"));

		MainMenu = CreateWidget<UPauseMenuWidget>(this, MainMenuSample);

		FInputModeGameAndUI Mode;
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		bShowMouseCursor = true;
		SetInputMode(Mode);


		MainMenu->AddToViewport(150); 

		//SetPause(true, FCanUnpause());
	}
}
