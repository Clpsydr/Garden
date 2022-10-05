#include "BattleStatComponent.h"
#include "GameStructs.h"

UBattleStatComponent::UBattleStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBattleStatComponent::BeginPlay()
{
	Super::BeginPlay();

	HP = MaxHP;
}

void UBattleStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBattleStatComponent::TakeDamage(FCombatBlob DamageInfo)
{
	
	HP = FMath::Clamp(HP -= DamageInfo.Value, 0.f, HP);
	if (HP <= 0.f)
	{

		if (OnDeath.IsBound())
		{
			OnDeath.Broadcast(DamageInfo.Killer);
		}
	}
	else
	{
		if (OnHealthChanged.IsBound())
		{
			OnHealthChanged.Broadcast(DamageInfo.Value);
		}
	}

}