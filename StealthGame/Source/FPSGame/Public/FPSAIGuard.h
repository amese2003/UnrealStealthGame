// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIstate : uint8 {
	Idle,
	suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category ="Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volumn);

	FRotator OriginalRotation;

	UFUNCTION()
	void ResetOrientation();

	FTimerHandle TimerHandle_ResetOrientation;

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
	EAIstate GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	void SetGuardState(EAIstate newState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIstate newState);

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool IsPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* CurrentNode;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* FirstNode;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* SecondNode;


	void MoveToNextTarget();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
