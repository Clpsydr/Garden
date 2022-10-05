#include "PoolingSystem.h"

AActor* UPoolingSystem::RetrieveBullet(UClass* Class, const FTransform& Transform)
{
	AActor* NewBullet = RetrieveActor(Class, Transform, ProjectilePool);
	return NewBullet;
}

AActor* UPoolingSystem::RetrieveUnit(UClass* Class, const FTransform& Transform)
{
	AActor* NewUnit = RetrieveActor(Class, Transform, UnitPool);
	return NewUnit;
}

AActor* UPoolingSystem::RetrieveActor(UClass* Class, const FTransform& Transform, FPool& PoolType)
{
	if (!Class)
	{
		return nullptr;
	}

	TArray<AActor*>* Pool = PoolType.InactiveActors.Find(Class);
	AActor* Result = nullptr;
	if (Pool && Pool->Num() > 0)
	{
		Result = Pool->Pop();
		//FVector CorrectedScale = Transform.GetScale3D() * Result->GetActorScale();  
		Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), FVector(1.f, 1.f, 1.f)), false, nullptr,
			ETeleportType::ResetPhysics);
		Result->SetActorTickEnabled(true);
	}
	else
	{
		Result = GetWorld()->SpawnActor(Class, &Transform);
		if (!Result)
		{
			return nullptr;
		}
		
		PoolType.ActiveActors.Add(Result);

		if (PoolType.ActiveActors == UnitPool.ActiveActors)
		{
			Result->OnDestroyed.AddDynamic(this, &UPoolingSystem::OnUnitDestroyed);
		}
		else if (PoolType.ActiveActors == ProjectilePool.ActiveActors)
		{
			Result->OnDestroyed.AddDynamic(this, &UPoolingSystem::OnProjectileDestroyed);
		}
	}

	return Result;
}

void UPoolingSystem::ReturnBullet(AActor* Actor)
{
	ReturnActor(Actor, ProjectilePool);
}

void UPoolingSystem::ReturnUnit(AActor* Actor)
{
	ReturnActor(Actor, UnitPool);
}

void UPoolingSystem::ReturnActor(AActor* Actor, FPool& PoolType)
{
	if (!Actor)
	{
		return;
	}

	if (!IsActorInPool(Actor, PoolType.ActiveActors))
	{
		Actor->Destroy();
	}

	TArray<AActor*>& Pool = PoolType.InactiveActors.FindOrAdd(Actor->GetClass());
	if (!Pool.Contains(Actor))
	{
		Actor->SetActorTickEnabled(false);
		Pool.Add(Actor);
	}
}

bool UPoolingSystem::IsActorInPool(AActor* Actor, TArray<AActor*> ActorPool) const
{
	return ActorPool.Contains(Actor);
}

bool UPoolingSystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UPoolingSystem::OnUnitDestroyed(AActor* Actor)
{
	check(IsActorInPool(Actor, UnitPool.ActiveActors));
	TArray<AActor*>* Pool = UnitPool.InactiveActors.Find(Actor->GetClass());
	if (Pool && Pool->Contains(Actor))
	{
		Pool->Remove(Actor);
	}
	else
	{
		UnitPool.ActiveActors.Remove(Actor);
	}
	
}

void UPoolingSystem::OnProjectileDestroyed(AActor* Actor)
{
	check(IsActorInPool(Actor, ProjectilePool.ActiveActors));
	TArray<AActor*>* Pool = ProjectilePool.InactiveActors.Find(Actor->GetClass());
	if (Pool && Pool->Contains(Actor))
	{
		Pool->Remove(Actor);
	}
	else
	{
		ProjectilePool.ActiveActors.Remove(Actor);
	}
}