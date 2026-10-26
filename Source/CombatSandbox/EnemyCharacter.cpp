// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
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

    UE_LOG(LogTemp, Warning, TEXT("Player Attack: hit enemy for %.1f damage. Enemy health: %.1f"), DamageAmount, Health);

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

