#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COMBATSANDBOX_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	void ApplyDamage(float DamageAmount);

	float GetCurrentHealth() const { return CurrentHealth; }
	float GetMaxHealth() const { return MaxHealth; }

	bool IsDead() const { return CurrentHealth <= 0.0f; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health",
		meta = (ClampMin = "1.0"))
	float MaxHealth = 100.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health",
		meta = (ClampMin = "1.0"))
	float CurrentHealth = 100.0f;

};