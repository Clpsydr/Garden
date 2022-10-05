#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "PhysicalInteractable.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "AtmosphereController.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "PoolingSystem.h"
#include "GameStructs.h"
#include "Bullet.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);

	ContemporaryPhysicsComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsComponent"));
	
	ItemGrabLocation = CreateDefaultSubobject<USceneComponent>(TEXT("GrabLocation"));
		USceneComponent* SkelMesh = Cast<USceneComponent>(GetComponentByClass(USkeletalMesh::StaticClass()));
	ItemGrabLocation->SetupAttachment(Camera);

	NightWeatherEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Weather effect"));
	NightWeatherEffect->SetupAttachment(RootComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
	JetpackResource = JetpackCapacity;

	//applying weather conditions
	if (NightWeatherEffect)
	{
		AActor* Atm = UGameplayStatics::GetActorOfClass(GetWorld(), AAtmosphereController::StaticClass());
		if (Atm)
		{
			Cast<AAtmosphereController>(Atm)->OnDusk.AddDynamic(this, &ThisClass::EnableNightWeather);
			Cast<AAtmosphereController>(Atm)->OnDawn.AddDynamic(this, &ThisClass::DisableNightWeather);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red,
				TEXT("Failure to detect any directional light - character"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5.f, FColor::Red,
			TEXT("quack"));
	}

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//speeding update
	if (bIsDashing)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2500.f;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}


	//Flying status update
	UCharacterMovementComponent* ThisComp = Cast<UCharacterMovementComponent>(GetComponentByClass(UCharacterMovementComponent::StaticClass()));

	//Managing fuel
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
		JetpackResource = FMath::Clamp(JetpackResource + JetpackDrain / 2, JetpackResource + JetpackDrain / 2, JetpackCapacity);
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

/// Grabbed item gets detached, possibly with added impulse
void APlayerCharacter::ReleaseAnItem(bool bWithForce)
{
	if (GrabbedItem)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("Releasing Item"));

		ContemporaryPhysicsComp->ReleaseComponent();

		if (bWithForce)
		{
			GrabbedItem->AddImpulse(Camera->GetForwardVector() * 40000, NAME_None, false);

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

void APlayerCharacter::Hurt(const FCombatBlob& DamageInfo)
{
	CurrentHp = FMath::Clamp(CurrentHp - DamageInfo.Value, 0.f, MaxHp);
	
	if (CurrentHp <= 0.f)
	{
		if (this == DamageInfo.Killer)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
				TEXT("Player killed himself"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
				TEXT("Player got kileld by " + DamageInfo.Killer->GetName()));
		}

		GetMesh()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast();
		}
	}
}

void APlayerCharacter::HazardDamage(const float AbsoluteDamage)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
		TEXT("Player is in dangerous area"));

	FCombatBlob NewDamage;
	NewDamage.Value = AbsoluteDamage;
	NewDamage.Killer = this; //ignoring safety instructions is a suicide
	Hurt(NewDamage);
}

void APlayerCharacter::FireGun()
{
	bIsReadyToFire = false;

	if (BulletType)
	{
		/*AActor* NewBullet;
		NewBullet = GetWorld()->SpawnActor<AActor>(BulletType,
			ItemGrabLocation->GetComponentLocation(),
			Camera->GetComponentRotation());*/

		UPoolingSystem* BulletPool = GetWorld()->GetSubsystem<UPoolingSystem>();
		FTransform SpawnTransform(Camera->GetComponentRotation(), 
								ItemGrabLocation->GetComponentLocation(), FVector::OneVector);
		ABullet* Bullet = Cast<ABullet>(BulletPool->RetrieveBullet(BulletType, SpawnTransform));
		if (Bullet)
		{
			Bullet->SetOwner(this);
			Bullet->Start();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.f, FColor::Yellow,
			TEXT("No bullet blueprint selected"));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ThisClass::Reload, ReloadSpeed, false);
}

void APlayerCharacter::Respawn()
{
	CurrentHp = MaxHp;
	EnergyOnHands = 0.f;
	CurrentWeight = 0.f;

	AActor* Start = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass());
	SetActorLocation(Start->GetActorLocation(), true);
	SetActorRotation(Start->GetActorRotation(), ETeleportType::ResetPhysics);

	GetMesh()->SetSimulatePhysics(false);
	bIsReadyToFire = true;
}

void APlayerCharacter::Reload()
{
	bIsReadyToFire = true;
}

void APlayerCharacter::EnableNightWeather()
{
	if (NightWeatherEffect)
	{
		NightWeatherEffect->Activate();
	}
}

void APlayerCharacter::DisableNightWeather()
{
	if (NightWeatherEffect)
	{
		NightWeatherEffect->Deactivate();
	}
}

void APlayerCharacter::GainEnergy(const float EnergyAmount)
{
	EnergyOnHands += EnergyAmount;
}