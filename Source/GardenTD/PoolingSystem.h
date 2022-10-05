#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolingSystem.generated.h"

/*
* Ghoulish development of the initial one pool subsystem
* TODO: pool class with its own cleanup methods and data structure below that would prevent WET in this .cpp
*/
USTRUCT()
struct FPool
{
	GENERATED_BODY()

	TArray<class AActor*> ActiveActors;

	TMap<UClass*, TArray<class AActor*>> InactiveActors;

	/*FORCEINLINE FPool& operator==(const FPool rhs)
	{
		return this->ActiveActors == rhs->ActiveActors;
	}*/
};

/**
 * Separate pooling for effects, units and projectiles
 */
UCLASS()
class GARDENTD_API UPoolingSystem : public UWorldSubsystem
{
	GENERATED_BODY()

//not the best way to separate pools, I guess
public:
	AActor* RetrieveBullet(UClass* Class, const FTransform& Transform);

	AActor* RetrieveUnit(UClass* Class, const FTransform& Transform);

	void ReturnBullet(AActor* Actor);

	void ReturnUnit(AActor* Actor);

protected:
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

	AActor* RetrieveActor(UClass* Class, const FTransform& Transform, FPool& PoolType);

	void ReturnActor(AActor* Actor, FPool& PoolType);

	bool IsActorInPool(AActor* Actor, TArray<AActor*> ActorPool) const;

private:
	UFUNCTION()
		void OnProjectileDestroyed(AActor* Actor);

	UFUNCTION()
		void OnUnitDestroyed(AActor* Actor);

	UPROPERTY()
		FPool ProjectilePool;
	UPROPERTY()
		FPool UnitPool;
		
};
