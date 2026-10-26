// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

class AEnemyCharacter;

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Chasing,
	Attacking,
	Retreating,
	Healing,
	Dead
};

UCLASS()
class COMBATSANDBOX_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	void HandleEnemyDeath();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void UpdateAIState();

	UPROPERTY()
	TObjectPtr<AEnemyCharacter> EnemyCharacter;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	EEnemyState CurrentState = EEnemyState::Idle;

	FTimerHandle AITimerHandle;
	FTimerHandle AttackTimerHandle;
};
