#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Combatable.h"
#include "Lair.generated.h"

class UNavigationComponent;
class USphereComponent;
class UBattleStatComponent;
class AHive;
class AUnit;
class UParticleSystemComponent;
/*
*  Stationary spot with parameters to spawn hives / units
*  needs navigational component to propel them
*  needs activation radius to manage separately spawned units
*  needs battlestat component to be damaged, return energy gain
*/
UCLASS()
class GARDENTD_API ALair : public AActor, public ICombatable
{
	GENERATED_BODY()
	
public:	
	ALair();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LairStats")
		int DifficultyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LairStats")
		float SpawningRate = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LairStats")
		float SelfResurrectRate = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LairStats")
		float Energy = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LairStats")
		bool Nocturnal = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseComponents")
		UNavigationComponent* InitialNavigationSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseComponents")
		UStaticMeshComponent* LairMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseComponents")
		UBattleStatComponent* LairParams;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseComponents")
		USphereComponent* AgroArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit type")
		TSubclassOf<AHive> SpawnHiveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit type")
		TSubclassOf<AUnit> SpawnUnitType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		UParticleSystemComponent* SplatterEffect;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Enable();

	UFUNCTION()
	void Disable();

	UFUNCTION()
		void DisableWithDamage(AActor* Receiver);

	UFUNCTION()
		virtual void Hurt(const FCombatBlob& DamageInfo) override;

	UFUNCTION()
		virtual void HazardDamage(const float AbsoluteDamage) override;

	UFUNCTION()
		virtual void GainEnergy(const float EnergyAmount) override;

	void LevelUp();

	void GenerateSpawn();

private:
	FTimerHandle RespawnTimerHandle;
	FTimerHandle ResurrectTimerHandle;
};
