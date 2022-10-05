#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GARDENTD_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();
	virtual void Start();
	virtual void Stop();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USphereComponent* Collider;

	UFUNCTION()
		virtual void OnHit(class UPrimitiveComponent* HitComp, 
			class AActor* OtherActor, 
			class UPrimitiveComponent* OtherComp, 
			FVector NormalImpulse, 
			const FHitResult& HitResult);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		float TTL = 3.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		float DestructivePower = 2.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		float Velocity = 6000.f;
private:
	FTimerHandle TTLHandle;
};
