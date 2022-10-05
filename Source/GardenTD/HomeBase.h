#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Combatable.h"
#include "HomeBase.generated.h"

class UBoxComponent;
class UBattleStatComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEvent, FString, EventID);

/*
* Base object, is an immobile goal for enemies, and contains various game goals, 
* as well as access to upgrade interface
*/
UCLASS()
class GARDENTD_API AHomeBase : public AActor, public ICombatable
{
	GENERATED_BODY()
public:	
	AHomeBase();

private:
	TMap<int, UStaticMeshComponent*> GoalBits;

	float BaseHp;

	int CollectedPieces = 0;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HomeArea;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBattleStatComponent* BaseParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
		float EnergyVictoryCondition = 80000.f;

public:	
	virtual void Tick(float DeltaTime) override;

	FOnGameEvent OnGameEvent;
	
	UFUNCTION(BlueprintCallable)
		void UpdateGoalsWith(int NewID);

	UFUNCTION(BlueprintCallable)
		void CheckVictoryConditions();

	UFUNCTION()
		void AnnounceGameOver(AActor* Address);

	UFUNCTION(BlueprintCallable)
		virtual void Hurt(const FCombatBlob& DamageInfo) override;

	UFUNCTION()
		virtual void HazardDamage(const float AbsoluteDamage) override {};

	UFUNCTION()
		virtual void GainEnergy(const float EnergyAmount) override;

	UFUNCTION()
		void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
