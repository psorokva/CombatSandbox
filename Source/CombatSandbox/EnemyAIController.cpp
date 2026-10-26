// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyAIController.h"
#include "AbilityDataAsset.h"
#include "EnemyCharacter.h"
#include "CombatComponent.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

	EnemyCharacter = Cast<AEnemyCharacter>(InPawn);

	if (!EnemyCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast possessed pawn to AEnemyCharacter"));
		return;
	}

    //UE_LOG(LogTemp, Warning, TEXT("EnemyAIController possessed: %s"), *InPawn->GetName());

	GetWorld()->GetTimerManager().SetTimer(
		AITimerHandle,
		this,
		&AEnemyAIController::UpdateAIState,
		0.2f,
		true
	);
}

void AEnemyAIController::UpdateAIState()
{
	if (CurrentState == EEnemyState::Dead)
	{
		return;
	}

	if (!EnemyCharacter)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		CurrentState = EEnemyState::Idle;
		StopMovement();
		return;
	}

	const UAbilityDataAsset* AbilityData = EnemyCharacter->GetPrimaryAbility();

	if (!AbilityData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy has no PrimaryAbility assigned"));
		return;
	}

	const FVector EnemyLocation = EnemyCharacter->GetActorLocation();
	const FVector PlayerLocation = PlayerPawn->GetActorLocation();

	float DistanceToPlayer = FVector::Dist(EnemyLocation, PlayerLocation);

	const float AttackRange = AbilityData->AttackRange;
	const float MinRange = AbilityData->MinRange;
	const float DetectionRange = AbilityData->DetectionRange;
	const bool IsHealer = AbilityData->bPrimarilyHealer;

	// Retreat when the player gets too close
	if (MinRange > 0.0f && DistanceToPlayer < MinRange)
	{
		if (CurrentState != EEnemyState::Retreating) // TODO: fix running into walls
		{
			CurrentState = EEnemyState::Retreating;

			const FVector RetreatDirection = (EnemyLocation - PlayerLocation).GetSafeNormal();

			const FVector RetreatDestination = EnemyLocation + RetreatDirection * AbilityData->RetreatDistance;

			MoveToLocation(RetreatDestination);

			UE_LOG(LogTemp, Warning, TEXT("Enemy State: RETREATING"));
		}

		return;
	}

	// Heal if healer and other enemy's health < full
	if (IsHealer) // TODO Add DistanceToEnemy and health check?
	{
		if (CurrentState != EEnemyState::Healing)
		{
			CurrentState = EEnemyState::Healing;
			StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy State: HEALING"));
		}

		// Add healing function here
		return;
	}
	
	// Attack when within the ability's range
	if (DistanceToPlayer <= AttackRange)
	{
		if (CurrentState != EEnemyState::Attacking)
		{
			CurrentState = EEnemyState::Attacking;
			StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy State: ATTACKING"));
		}

		if (EnemyCharacter->CombatComponent)
		{
			EnemyCharacter->CombatComponent->Attack(PlayerPawn);
		}

		return;
	}

	// Chase while the player is detected
	if (DistanceToPlayer <= DetectionRange)
	{
		if (CurrentState != EEnemyState::Chasing)
		{
			CurrentState = EEnemyState::Chasing;

			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

			UE_LOG(LogTemp, Warning, TEXT("Enemy State: CHASING"));
		}

		MoveToActor(PlayerPawn);
		return;
	}
	
	// Otherwise, return to idle
	if (CurrentState != EEnemyState::Idle)
	{
		CurrentState = EEnemyState::Idle;
		//GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		StopMovement();
		UE_LOG(LogTemp, Warning, TEXT("Enemy State: IDLE"));
	}	
	
}

//void AEnemyAIController::Die()
//{
//	CurrentState = EEnemyState::Dead;
//
//	StopMovement();
//	GetWorld()->GetTimerManager().ClearTimer(AITimerHandle);
//	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
//	
//	UE_LOG(LogTemp, Warning, TEXT("Enemy State: DEAD"));
//}

void AEnemyAIController::HandleEnemyDeath()
{
	GetWorld()->GetTimerManager().ClearTimer(AITimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	StopMovement();
	CurrentState = EEnemyState::Dead;
	UE_LOG(LogTemp, Warning, TEXT("Enemy State: DEAD"));
}