#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Combatable.h"
#include "NiagaraComponent.h"
#include "GameStructs.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

class UCameraComponent;
class UPhysicsHandleComponent;
class UNiagaraComponent;

UCLASS()
class GARDENTD_API APlayerCharacter : public ACharacter, public ICombatable
{
	GENERATED_BODY()
public:
	APlayerCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UPhysicsHandleComponent* ContemporaryPhysicsComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USceneComponent* ItemGrabLocation;

	FOnPlayerDeath OnDeath;

	// scene component for grabbing location
private:
	UPrimitiveComponent* GrabbedItem;

	FVector FormerGrabItemLocation;

	bool bIsReadyToFire = true;

	bool bIsJetpackOn = false;

	FTimerHandle ReloadTimerHandle;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Battle")
		float MaxHp = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Battle")
		float CurrentHp = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Jetpack")
		float JetpackTopSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Jetpack")
		float JetpackCapacity = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Jetpack")
		float JetpackPower = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - JetPack")
		float MaxElevationPower = -0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Jetpack")
		float ReloadSpeed = 0.3f;
	
	float JetpackResource = 40.f;

	float JetpackDrain = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Physics")
		float TopStrength = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats - Physics")
		float StrengthLeft = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "spawn subclasses")
		TSubclassOf<AActor> BulletType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerVisuals")
		UNiagaraComponent* NightWeatherEffect;

	float CurrentWeight = 0.f;

	float EnergyOnHands = 0.f;

public:	
	bool bIsFiring = false;

	bool bIsDashing = false;

	virtual void Tick(float DeltaTime) override;

	void StrengthUpdate(float DeltaTime);

	void Fly();

	void StopFlying();

	void FireGun();

	void Reload();

	virtual void MoveSide(float AxisValue);

	virtual void MoveFront(float AxisValue);

	virtual void CameraTurnX(float AxisValue);

	virtual void CameraTurnY(float AxisValue);

	UFUNCTION(BlueprintCallable)
		void Respawn();

	UFUNCTION(BlueprintCallable)
		float GetFuelRatio();

	UFUNCTION(BlueprintCallable)
		float GetHPRatio();

	UFUNCTION(BlueprintCallable)
		void GrabAnItem(AActor* NewlyGrabbedItem);

	UFUNCTION(BlueprintCallable)
		void ReleaseAnItem(bool bWithForce);

	UFUNCTION(BlueprintCallable)
		float GetEnergy() { return EnergyOnHands; };

	UFUNCTION(BlueprintCallable)
		void AddEnergy(float EnergyBonus) { EnergyOnHands += EnergyBonus; };

	UFUNCTION()
	void EnableNightWeather();

	UFUNCTION()
	void DisableNightWeather();

	virtual void Hurt(const FCombatBlob& DamageInfo) override;

	UFUNCTION(BlueprintCallable)
	virtual void HazardDamage(const float AbsoluteDamage) override;

	virtual void GainEnergy(const float EnergyAmount) override;

	
};
