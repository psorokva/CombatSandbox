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

	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this,
		&AEnemyAIController::FindAndChasePlayer
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