// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	overlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("overlapComp"));
	overlapComp->SetBoxExtent(FVector(75, 75, 50));
	RootComponent = overlapComp;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(RootComponent);

	overlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OverlapLaunchPad);

	LaunchPower = 1000.0f;
	LaunchAngle = 35.0f;
	
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchPad::OverlapLaunchPad(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FRotator direction = GetActorRotation();	
	direction.Pitch += LaunchAngle;
	FVector launchVelocity = direction.Vector() * LaunchPower;

	AFPSCharacter* character = Cast<AFPSCharacter>(OtherActor);

	if (character) {
		character->LaunchCharacter(launchVelocity, true, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, PickupEX, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics()) {
		OtherComp->AddImpulse(launchVelocity, NAME_None, true);
	}
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

