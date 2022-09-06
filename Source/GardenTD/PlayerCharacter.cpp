#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalInteractable.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	ContemporaryPhysicsComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsComponent"));
	
	ItemGrabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GrabLocation"));
		USceneComponent* SkelMesh = Cast<USceneComponent>(GetComponentByClass(USkeletalMesh::StaticClass()));
	ItemGrabLocation->SetupAttachment(Camera);

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
	JetpackResource = JetpackCapacity;

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Flying status update
	UCharacterMovementComponent* ThisComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	if (bIsJetpackOn)
	{
		JetpackResource -= JetpackDrain;

		ThisComp->GravityScale = FMath::Clamp(ThisComp->GravityScale, MaxElevationPower, ThisComp->GravityScale - DeltaTime * JetpackTopSpeed * 2);

		if (JetpackResource <= 0.f)
		{
			StopFlying();
		}
	}
	else if (ThisComp->GravityScale < 1.f)
	{
		ThisComp->GravityScale = FMath::Clamp(ThisComp->GravityScale, ThisComp->GravityScale + DeltaTime * JetpackTopSpeed, 10.f);
	}

	//Firecheck
	if (bIsFiring && bIsReadyToFire)
	{
		FireGun();
	}

	//Physics update
	StrengthUpdate(DeltaTime);

	ContemporaryPhysicsComp->SetTargetLocationAndRotation(ItemGrabLocation->GetComponentLocation(),
		ItemGrabLocation->GetComponentRotation());
}

void APlayerCharacter::StrengthUpdate(float DeltaTime)
{
	if (GrabbedItem)
	{
		if (CurrentWeight > 0)
		{
			StrengthLeft -= GrabbedItem->GetMass() * DeltaTime;
			CurrentWeight -= GrabbedItem->GetMass() * DeltaTime;

			if (StrengthLeft <= 0)
			{
				ReleaseAnItem(false);
			}
		}
	}
	else
	{
		StrengthLeft = FMath::Clamp(StrengthLeft + 20 * DeltaTime, StrengthLeft + 20 * DeltaTime, TopStrength);
	}


}

void APlayerCharacter::MoveSide(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerCharacter::MoveFront(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::CameraTurnX(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void APlayerCharacter::CameraTurnY(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerCharacter::Fly()
{
	if (JetpackResource > 0.f)
	{
		bIsJetpackOn = true;
		Jump();
	}
}

void APlayerCharacter::StopFlying()
{
	bIsJetpackOn = false;
}

float APlayerCharacter::GetFuelRatio()
{
	return JetpackResource / JetpackCapacity;
}

float APlayerCharacter::GetHPRatio()
{
	return CurrentHp / MaxHp;
}

void APlayerCharacter::GrabAnItem(AActor* NewlyGrabbedItem)
{
	if (GrabbedItem)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Already holding something"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Starting to grab item"));

		GrabbedItem = Cast<UStaticMeshComponent>(NewlyGrabbedItem->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (!GrabbedItem)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red,
				TEXT("Improper Grabitem save"));
		}
		FormerGrabItemLocation = GrabbedItem->GetComponentLocation();
	
		//lerp from 000 to ItemGrabLocation in 1 second easing
		/* GrabbedItem->SetWorldLocation(FMath::Lerp(GrabbedItem->GetComponentLocation(),
									FVector(0.f,0.f,0.f), 
			exp2(-0.2f * DeltaTime));*/

		//might freak out because we dont know if its relative or absolute (in world space means okay?)
		GrabbedItem->SetWorldLocation(ItemGrabLocation->GetComponentLocation());

		ContemporaryPhysicsComp->GrabComponentAtLocation(GrabbedItem, "None", ItemGrabLocation->GetComponentLocation());
		ContemporaryPhysicsComp->SetAngularDamping(0.1f);
		CurrentWeight = GrabbedItem->GetMass();

	}
}

void APlayerCharacter::ReleaseAnItem(bool bWithForce)
{
	if (GrabbedItem)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Releasing Item"));

		ContemporaryPhysicsComp->ReleaseComponent();

		if (bWithForce)
		{
			GrabbedItem->AddImpulse(Camera->GetForwardVector() * 25000, NAME_None, false);

			///in worse case, work with cast results
			if (GrabbedItem->GetOwner()->Implements<UPhysicalInteractable>())
			{
				//dumb, detonation might not be actually implemented
				//Cast<IPhysicalInteractable>(GrabbedItem->GetOwner())->Execute_ContextualDetonate(GrabbedItem);
			}
		}

		GrabbedItem = nullptr;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Nothing to release"));
	}
}

void APlayerCharacter::Hurt(const float DamageAmount)
{
}

void APlayerCharacter::FireGun()
{
	bIsReadyToFire = false;

	if (BulletType)
	{
		AActor* NewBullet;
		NewBullet = GetWorld()->SpawnActor<AActor>(BulletType,
			ItemGrabLocation->GetComponentLocation(),
			Camera->GetComponentRotation());
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("No bullet blueprint selected"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ThisClass::Reload, ReloadSpeed, false);
}

void APlayerCharacter::Reload()
{
	bIsReadyToFire = true;
}
