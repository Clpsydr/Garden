#include "Bullet.h"
#include "PoolingSystem.h"
#include "Combatable.h"
#include "GameStructs.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	/*USceneComponent* RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;*/

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	RootComponent = Collider;
	//Collider->SetupAttachment(RootComponent);
	Collider->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(TTLHandle, this, &ThisClass::Stop, TTL, false);
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * Velocity * DeltaTime;
	SetActorLocation(NextPosition, true); 
}

void ABullet::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	//StartPosition = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetWorld()->GetTimerManager().SetTimer(TTLHandle, this, &ThisClass::Stop, TTL, false);
}

void ABullet::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetHiddenInGame(true);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorld()->GetTimerManager().ClearTimer(TTLHandle);

	UPoolingSystem* BulletPool = GetWorld()->GetSubsystem<UPoolingSystem>();
	BulletPool->ReturnBullet(this);
}

void ABullet::OnHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}
	else
	{
		//applying impulse to hit objects
		UStaticMeshComponent* HitMesh = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
		if (HitMesh && HitMesh->IsSimulatingPhysics())
		{
			FVector Impulse = 5.f * Velocity * GetActorForwardVector();
			OtherComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);
			/*FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
			ForceVector.Normalize();
			HitMesh->AddImpulse(ForceVector * 400.f, NAME_None, true);*/
		}

		ICombatable* DamageableActor = Cast<ICombatable>(OtherActor);
		if (DamageableActor)
		{
			//find out why this was with a * in front for DamageData
			FCombatBlob ThisAttack;
			ThisAttack.Value = DestructivePower;
			ThisAttack.Killer = GetOwner();
			DamageableActor->Hurt(ThisAttack);
		}

		Stop();
	}
	
}

