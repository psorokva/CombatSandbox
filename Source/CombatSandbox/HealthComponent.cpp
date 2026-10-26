#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;
}

void UHealthComponent::ApplyDamage(float DamageAmount)
{
    if (DamageAmount <= 0.0f || IsDead())
    {
        return;
    }

    CurrentHealth = FMath::Max(
        0.0f,
        CurrentHealth - DamageAmount
    );

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("%s health: %.1f / %.1f"),
        *GetNameSafe(GetOwner()),
        CurrentHealth,
        MaxHealth
    );

    if (IsDead())
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("%s has died"),
            *GetNameSafe(GetOwner())
        );
    }

}