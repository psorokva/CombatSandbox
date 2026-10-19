// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "CombatComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(
		TEXT("CombatComponent")
	);
}

float AEnemyCharacter::TakeDamage(
    float DamageAmount,
    const FDamageEvent& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    Health -= DamageAmount;

    if (Health <= 0.0f)
    {
        Health = 0.0f;
    }

    return DamageAmount;
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

    //UE_LOG(
    //    LogTemp,
    //    Warning,
    //    TEXT("Enemy BeginPlay: %s | Controller: %s"),
    //    *GetName(),
    //    GetController()
    //    ? *GetController()->GetName()
    //    : TEXT("None")
    //);
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

