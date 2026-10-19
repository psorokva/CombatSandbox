#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UAbilityDataAsset;

UCLASS(ClassGroup=(Combat), meta=(BlueprintSpawnableComponent))
class COMBATSANDBOX_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatComponent();

	bool CanAttack(AActor* Target) const;
	void Attack(AActor* Target);

private:
	bool bCanAttack = true;

	FTimerHandle AttackCooldownTimerHandle;

	const UAbilityDataAsset* GetAbilityData() const;

	void ResetAttackCooldown();
};