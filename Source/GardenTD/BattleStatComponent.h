#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "BattleStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AActor*, Receiver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, HPModifier);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GARDENTD_API UBattleStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBattleStatComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
		FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
		FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStats")
		float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxStats")
		float MaxHP = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxStats")
		float HPRecovery = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStats")
		float EnergyReward = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentStats")
		float AttackPower = 10.f;

	UFUNCTION(BlueprintCallable)
		float GetHPRatio() { return HP / MaxHP; };

	UFUNCTION(BlueprintCallable)
		void TakeDamage(FCombatBlob DamageInfo);

	UFUNCTION()
		void RegenerateHP() { HP = FMath::Clamp(HP += HPRecovery, 0.f, MaxHP); }
};
