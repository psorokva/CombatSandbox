// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "CombatComponent.h"
#include "HealthComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(
		TEXT("CombatComponent")
	);
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(
        TEXT("HealthComponent")
    );
}

float AEnemyCharacter::TakeDamage(
    float DamageAmount,
    const FDamageEvent& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(
        DamageAmount,
        DamageEvent,
        EventInstigator,
        DamageCauser
    );

    if (HealthComponent)
    {
        HealthComponent->ApplyDamage(ActualDamage);
        if (HealthComponent->IsDead())
        {
            Die();
        }
    }

    return ActualDamage;
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

bool AEnemyCharacter::IsDead() const
{
    return HealthComponent && HealthComponent->IsDead();
}

void AEnemyCharacter::Die()
{
    if (IsDead() == false || bDeathHandled)
    {
        return;
    }

    bDeathHandled = true;
    UE_LOG(LogTemp, Warning, TEXT("%s died"), *GetName());

    if (AEnemyAIController* EnemyController = Cast<AEnemyAIController>(GetController()))
    {
        EnemyController->HandleEnemyDeath();
    }
}

