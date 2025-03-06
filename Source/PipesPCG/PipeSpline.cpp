#include "PipeSpline.h"

// Sets default values
APipeSpline::APipeSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	bRunConstructionScriptOnDrag = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (Spline)
	{
		RootComponent = Spline;
	}
}

void APipeSpline::OnConstruction(const FTransform& Transform)
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++)
	{
		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		MeshComponent->SetMobility(EComponentMobility::Movable);
		MeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		MeshComponent->RegisterComponentWithWorld(GetWorld());
		MeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		MeshComponent->SetRelativeLocation(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local));
		MeshComponent->SetStaticMesh(CubeMesh);
	}
}