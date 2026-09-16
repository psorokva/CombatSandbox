// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
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
	if (DistanceToPlayer <= DetectionRange)
	{
		if (CurrentState != EEnemyState::Chasing)
		{
			CurrentState = EEnemyState::Chasing;

			UE_LOG(LogTemp, Warning, TEXT("Enemy State: CHASING"));
		}
		MoveToActor(PlayerPawn);
	}
	else
	{
		if (CurrentState != EEnemyState::Idle)
		{
			CurrentState = EEnemyState::Idle;
			UE_LOG(LogTemp, Warning, TEXT("Enemy State: IDLE"));
		}
		
		StopMovement();
	}
}