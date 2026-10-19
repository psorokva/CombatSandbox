#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityDataAsset.generated.h"

UCLASS(BlueprintType)
class COMBATSANDBOX_API UAbilityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float DetectionRange = 1500.0f;

	// Maximum distance at which the ability can hit.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AttackRange = 200.0f;

	// Minimum preferred distance. Set higher for ranged units.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float MinRange = 0.0f;

	// How far enemy should retreat before resuming attack
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float RetreatDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AttackDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float AttackCooldown = 2.0f;

};


