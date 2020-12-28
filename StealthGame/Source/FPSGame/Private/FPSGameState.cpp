// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool MissionSuccess) {
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++) {
		APawn* Pawn = It->Get();

		if (Pawn && Pawn->IsLocallyControlled()) {
			Pawn->DisableInput(nullptr);
		}

	}
}