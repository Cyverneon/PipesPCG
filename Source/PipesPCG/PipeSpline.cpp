#include "PipeSpline.h"

// Sets default values
APipeSpline::APipeSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	bRunConstructionScriptOnDrag = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
}

void APipeSpline::OnConstruction(const FTransform& Transform)
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints()-1; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		FVector StartPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		FVector StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		FVector EndPos = Spline->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local);
		FVector EndTangent = Spline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local);
		SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		SplineMeshComponent->SetStaticMesh(PipeMesh);
	}
	//for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++)
	//{
	//	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	//	MeshComponent->SetMobility(EComponentMobility::Movable);
	//	MeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	//	MeshComponent->RegisterComponentWithWorld(GetWorld());
	//	MeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
	//	MeshComponent->SetRelativeLocation(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local));
	//	MeshComponent->SetStaticMesh(CubeMesh);
	//}
}