// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Combat/CombatPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "CombatCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Blueprint/UserWidget.h"
#include "CombatSandbox.h"
#include "Widgets/Input/SVirtualJoystick.h"

#include "EnemyCharacter.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "DrawDebugHelpers.h"

void ACombatPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACombatPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// add the input mapping context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}

	// only spawn touch controls on local player controllers
	if (IsLocalPlayerController() && ShouldUseTouchControls())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogCombatSandbox, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(
				AttackAction,
				ETriggerEvent::Started,
				this,
				&ACombatPlayerController::PlayerAttack
			);
		}
	}
}

void ACombatPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// subscribe to the pawn's OnDestroyed delegate
	InPawn->OnDestroyed.AddDynamic(this, &ACombatPlayerController::OnPawnDestroyed);
}

void ACombatPlayerController::SetRespawnTransform(const FTransform& NewRespawn)
{
	// save the new respawn transform
	RespawnTransform = NewRespawn;
}

void ACombatPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	// spawn a new character at the respawn transform
	if (ACombatCharacter* RespawnedCharacter = GetWorld()->SpawnActor<ACombatCharacter>(CharacterClass, RespawnTransform))
	{
		// possess the character
		Possess(RespawnedCharacter);
	}
}

bool ACombatPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void ACombatPlayerController::PlayerAttack()
{
	APawn* PlayerPawn = GetPawn();

	if (!PlayerPawn)
	{
		UE_LOG(LogCombatSandbox, Warning, TEXT("Player Attack: did not find Player Pawn"));
		return;
	}

	const FVector PlayerLocation = GetPawn()->GetActorLocation();

	FVector AttackDirection = GetControlRotation().Vector();
	AttackDirection.Z = 0.0f;
	AttackDirection.Normalize();

	const FVector Start = PlayerLocation + FVector(0.0f, 0.0f, 60.0f);

	const FVector End = Start + AttackDirection * 200.0f;

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(PlayerPawn);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		bHit ? FColor::Green : FColor::Red,
		false,
		5.0f,
		0,
		2.0f
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Start: %s | End: %s"),
		*Start.ToString(),
		*End.ToString()
	);

	if (!bHit)
	{
		UE_LOG(LogCombatSandbox, Warning, TEXT("Player Attack: did not hit anything"));
		return;
	}
	else {
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("Hit actor: %s | Hit component: %s | Location: %s"),
			*GetNameSafe(Hit.GetActor()),
			*GetNameSafe(Hit.GetComponent()),
			*Hit.ImpactPoint.ToString()
		);
	}

	AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Hit.GetActor());

	if (!Enemy)
	{
		UE_LOG(LogCombatSandbox, Warning, TEXT("Player Attack: no enemy found"));
		return;
	}

	UGameplayStatics::ApplyDamage(
		Enemy,
		10.0f,
		this,
		PlayerPawn,
		UDamageType::StaticClass()
	);
}
