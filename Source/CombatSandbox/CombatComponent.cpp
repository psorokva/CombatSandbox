#include "CombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::Attack(AActor* Target)
{
	if (!CanAttack(Target))
	{
		return;
	}

	UGameplayStatics::ApplyDamage(
		Target,
		AttackDamage,
		GetOwner()->GetInstigatorController(),
		GetOwner(),
		UDamageType::StaticClass()
	);

	UE_LOG(
		LogTemp, 
		Warning, 
		TEXT("%s attacked %s for %.1f damage"), 
		*GetOwner()->GetName(), 
		*Target->GetName(), 
		AttackDamage
	);


	bCanAttack = false;

	GetWorld()->GetTimerManager().SetTimer(
		AttackCooldownTimerHandle,
		this,
		&UCombatComponent::ResetAttackCooldown,
		AttackCooldown,
		false
	);
}

bool UCombatComponent::CanAttack(AActor* Target) const
{
	if (!Target || !GetOwner() || !bCanAttack) {
		return false;
	}

	const float DistanceToTarget = FVector::Dist(
		GetOwner()->GetActorLocation(), 
		Target->GetActorLocation()
	);

	return DistanceToTarget <= AttackRange;

}

void UCombatComponent::ResetAttackCooldown()
{
	bCanAttack = true;
}