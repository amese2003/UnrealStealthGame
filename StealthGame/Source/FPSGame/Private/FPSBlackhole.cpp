// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSBlackhole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSBlackhole::AFPSBlackhole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = meshComp;

	InnerSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InnerSphereComp"));
	InnerSphereComponent->SetSphereRadius(100);
	InnerSphereComponent->SetupAttachment(meshComp);

	// 4.25 버전에서, 생성자때 addDynamic이 잘 안되는듯...
	//InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackhole::OverlapInnerSphere);

	OuterSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphereComp"));
	OuterSphereComponent->SetSphereRadius(3000);
	OuterSphereComponent->SetupAttachment(meshComp);
	//OuterSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackhole::OverlapInnerSphere);
}

// Called when the game starts or when spawned
void AFPSBlackhole::BeginPlay()
{
	Super::BeginPlay();
	InnerSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackhole::OverlapInnerSphere);
	
}

void AFPSBlackhole::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		OtherActor->Destroy();
	}
}

// Called every frame
void AFPSBlackhole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> overlapComp;
	OuterSphereComponent->GetOverlappingComponents(overlapComp);

	for (int32 i = 0; i < overlapComp.Num(); i++) {
		UPrimitiveComponent* frontComp = overlapComp[i];

		if (frontComp && frontComp->IsSimulatingPhysics()) {
			const float sphereRadius = OuterSphereComponent->GetScaledSphereRadius();
			const float ForceStrength = -3000;

			frontComp->AddRadialForce(GetActorLocation(), sphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
			
		}
	}

}

