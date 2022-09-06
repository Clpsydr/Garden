#include "MainPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "PauseMenuWidget.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "PhysicalInteractable.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMainPlayerController::Interact);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainPlayerController::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMainPlayerController::StopJumping);
	InputComponent->BindAction("EscapeMenu", IE_Pressed, this, &AMainPlayerController::CallMenu);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &AMainPlayerController::Fire);
	InputComponent->BindAction("Shoot", IE_Released, this, &AMainPlayerController::StopFiring);

	InputComponent->BindAxis("CharacterMoveFront", this, &AMainPlayerController::MoveForward);
	InputComponent->BindAxis("CharacterMoveRight", this, &AMainPlayerController::MoveRight);

	InputComponent->BindAxis("CameraMoveX", this, &AMainPlayerController::CameraTurnX);
	InputComponent->BindAxis("CameraMoveY", this, &AMainPlayerController::CameraTurnY);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerCharacter>(GetPawn());
	OriginalPawn = Cast<APlayerCharacter>(GetPawn());
}

// Pawns utilize OnPossess or OnUnpossess events in BP to remove or change themselves accordingly
void AMainPlayerController::ChangePawnTo(APlayerCharacter* NewPawn)
{
	if (NewPawn)
	{
		UnPossess();
		PlayerPawn = NewPawn;
		Possess(NewPawn);
	}
}

void AMainPlayerController::MoveForward(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->MoveFront(AxisValue);
	}
}

void AMainPlayerController::MoveRight(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->MoveSide(AxisValue);

	}
}

void AMainPlayerController::CameraTurnX(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->CameraTurnX(AxisValue);
	}
}

void AMainPlayerController::CameraTurnY(float AxisValue)
{
	if (PlayerPawn && AxisValue != 0.0f)
	{
		PlayerPawn->CameraTurnY(AxisValue);
	}
}

// Tracing and checking for item grab possibility and vehicle boarding
void AMainPlayerController::Interact()
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Yellow,
		TEXT("Interact button pressed"));
	
	if (bIsReadyToInteract)
	{
		bIsReadyToInteract = false;

		FHitResult HitResult;
		FVector TraceStart = 
			Cast<USceneComponent>(PlayerPawn->GetComponentByClass(UCameraComponent::StaticClass()))
				->GetComponentLocation();
		
		FRotator InRotEnd =
			Cast<USceneComponent>(PlayerPawn->GetComponentByClass(UCameraComponent::StaticClass()))
				->GetComponentRotation();

		FVector TraceEnd = TraceStart + InRotEnd.RotateVector(FVector(1.f, 0.f, 0.f)) * InteractRange;

		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("PlayerInteractTrace")), true, this);
		TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
			ECollisionChannel::ECC_Visibility, TraceParams);
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Blue, false, 2.f, 0, 2);
			
		if (HitResult.IsValidBlockingHit() && HitResult.Actor->Implements<UPhysicalInteractable>())
		{
			Cast<IPhysicalInteractable>(HitResult.Actor)->Execute_ContextualGrab(HitResult.Actor.Get(), PlayerPawn);
		}
		else
		{
			PlayerPawn->ReleaseAnItem(true);
		}

		GetWorld()->GetTimerManager().SetTimer(ActionTimerHandle, this, &ThisClass::ResetAction, ActionCooldown, false);
	}

	//TODO: also boating procedure 
}

void AMainPlayerController::Jump()
{
	if (PlayerPawn)
	{
		PlayerPawn->Fly();
	}
}

void AMainPlayerController::StopJumping()
{
	if (PlayerPawn)
	{
		PlayerPawn->StopFlying();
	}
}

void AMainPlayerController::Fire()
{
	if (PlayerPawn)
	{
		PlayerPawn->bIsFiring = true;
	}
}

void AMainPlayerController::StopFiring()
{
	if (PlayerPawn)
	{
		PlayerPawn->bIsFiring = false;
	}
}

void AMainPlayerController::Fly(float AxisValue)
{
	
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
