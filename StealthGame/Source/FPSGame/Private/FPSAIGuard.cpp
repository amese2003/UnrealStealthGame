// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"



// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIstate::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

	if (IsPatrol)
		MoveToNextTarget();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIstate::Alerted);


	AController* controller = GetController();

	if (controller)
		controller->StopMovement();
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volumn) {
	if (GuardState == EAIstate::Alerted) 
		return;

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	SetGuardState(EAIstate::suspicious);

	AController* controller = GetController();

	if (controller)
		controller->StopMovement();
		

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);	
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
}

void AFPSAIGuard::ResetOrientation() {
	if (GuardState == EAIstate::Alerted)
		return;

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIstate::Idle);

	if (IsPatrol)
		MoveToNextTarget();
}

void AFPSAIGuard::SetGuardState(EAIstate newState)
{
	if (GuardState == newState)
		return;

	GuardState = newState;

	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextTarget()
{
	if (CurrentNode == nullptr || CurrentNode == SecondNode)
		CurrentNode = FirstNode;
	else
		CurrentNode = SecondNode;


	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentNode);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsPatrol) {
		FVector delta = GetActorLocation() - CurrentNode->GetActorLocation();
		float distanceToGoal = delta.Size();



		if (distanceToGoal < 100)
			MoveToNextTarget();
	}

}

