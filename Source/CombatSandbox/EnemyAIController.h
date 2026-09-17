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
	void AttackPlayer();

	UFUNCTION(BlueprintCallable)
	void Die();

	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRange = 1500.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackDamage = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	EEnemyState CurrentState = EEnemyState::Idle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackCooldown = 1.0f;

	FTimerHandle AITimerHandle;
	FTimerHandle AttackTimerHandle;
};
