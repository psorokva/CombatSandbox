// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Chasing,
	Attacking,
	Dead
};

UCLASS()
class COMBATSANDBOX_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	void FindAndChasePlayer();

	void UpdateAIState();

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRange = 1500.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	EEnemyState CurrentState = EEnemyState::Idle;

	FTimerHandle AITimerHandle;
};
