#include "CombatComponent.h"
#include "AbilityDataAsset.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

const UAbilityDataAsset* UCombatComponent::GetAbilityData() const
{
	const AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(GetOwner());

	if (!EnemyCharacter)
	{
		return nullptr;
	}

	return EnemyCharacter->GetPrimaryAbility();
}

void UCombatComponent::Attack(AActor* Target)
{
	AActor* OwnerActor = GetOwner();

	if (!IsValid(OwnerActor) || !IsValid(Target))
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Attack aborted: invalid owner or target")
		);
		return;
	}

	if (!CanAttack(Target))
	{
		return;
	}

	const UAbilityDataAsset* AbilityData = GetAbilityData();

	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy has no AbilityData assigned"));
		return;
	}

	const float Damage = AbilityData->AttackDamage;
	const float Cooldown = AbilityData->AttackCooldown;

	UGameplayStatics::ApplyDamage(
		Target,
		Damage,
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
		Damage
	);


	bCanAttack = false;

	GetWorld()->GetTimerManager().SetTimer(
		AttackCooldownTimerHandle,
		this,
		&UCombatComponent::ResetAttackCooldown,
		Cooldown,
		false
	);
}

bool UCombatComponent::CanAttack(AActor* Target) const
{
	AActor* OwnerActor = GetOwner();

	if (!IsValid(OwnerActor) || !IsValid(Target) || !bCanAttack)
	{
		return false;
	}

	const UAbilityDataAsset* AbilityData = GetAbilityData();

	if (!AbilityData)
	{
		return false;
	}

	const float DistanceToTarget = FVector::Dist(
		OwnerActor->GetActorLocation(),
		Target->GetActorLocation()
	);

	return DistanceToTarget <= AbilityData->AttackRange;

}

void UCombatComponent::ResetAttackCooldown()
{
	bCanAttack = true;
}