// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/DamageType.h"
#include "TimerManager.h"

AEnemyAIController::AEnemyAIController()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemyAIController CREATED"));
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UE_LOG(LogTemp, Warning, TEXT("EnemyAIController possessed: %s"), *InPawn->GetName());

	GetWorld()->GetTimerManager().SetTimer(
		AITimerHandle,
		this,
		&AEnemyAIController::UpdateAIState,
		0.2f,
		true
	);
}

void AEnemyAIController::FindAndChasePlayer()
{
	UE_LOG(LogTemp, Warning, TEXT("FindAndChasePlayer CALLED"));

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerPawn NOT FOUND"));
        return;
    }

    MoveToActor(PlayerPawn);
}

void AEnemyAIController::UpdateAIState()
{
	if (CurrentState == EEnemyState::Dead)
	{
		return;
	}

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		CurrentState = EEnemyState::Idle;
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	float DistanceToPlayer = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerPawn->GetActorLocation());
	
	if (DistanceToPlayer <= AttackRange)
	{
		if (CurrentState != EEnemyState::Attacking)
		{
			CurrentState = EEnemyState::Attacking;
			StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy State: ATTACKING"));

			AttackPlayer();

			GetWorld()->GetTimerManager().SetTimer(
				AttackTimerHandle,
				this,
				&AEnemyAIController::AttackPlayer,
				AttackCooldown,
				true
			);
		}

		// Implement attack logic here
	}
	else if (DistanceToPlayer <= DetectionRange)
	{
		if (CurrentState != EEnemyState::Chasing)
		{
			CurrentState = EEnemyState::Chasing;

			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

			UE_LOG(LogTemp, Warning, TEXT("Enemy State: CHASING"));
		}
		MoveToActor(PlayerPawn);
	}
	else
	{
		if (CurrentState != EEnemyState::Idle)
		{
			CurrentState = EEnemyState::Idle;
			GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
			StopMovement();
			UE_LOG(LogTemp, Warning, TEXT("Enemy State: IDLE"));
		}	
	}
}

void AEnemyAIController::AttackPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn)
	{
		return;
	}
	UGameplayStatics::ApplyDamage(PlayerPawn, AttackDamage, this, GetPawn(), UDamageType::StaticClass());

	UE_LOG(LogTemp, Warning, TEXT("Enemy attacked player for %.1f damage"),	AttackDamage);
}

void AEnemyAIController::Die()
{
	CurrentState = EEnemyState::Dead;

	StopMovement();
	GetWorld()->GetTimerManager().ClearTimer(AITimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	
	UE_LOG(LogTemp, Warning, TEXT("Enemy State: DEAD"));
}