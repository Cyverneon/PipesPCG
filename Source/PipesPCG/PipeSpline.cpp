#include "PipeSpline.h"

// Sets default values
APipeSpline::APipeSpline()
{
	PrimaryActorTick.bCanEverTick = false;

	// create scene component and spline
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);

	// load default meshes
	const ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultPipeMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	PipeMesh = DefaultPipeMesh.Object;

	const ConstructorHelpers::FObjectFinder<UStaticMesh> DefaultRingMesh(TEXT("/Game/Meshes/Pipe2.Pipe2"));
	RingMesh = DefaultRingMesh.Object;

	// set correct default forward axis for the default mesh
	ForwardAxis = ESplineMeshAxis::Z;
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

		SplineMeshComponent->SetStaticMesh(PipeMesh);

		SplineMeshComponent->SetForwardAxis(ForwardAxis, true);

		const FVector StartPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		const FVector StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		const FVector EndPos = Spline->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local);
		const FVector EndTangent = Spline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local);
		SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	}
	for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++)
	{
		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		MeshComponent->SetMobility(EComponentMobility::Movable);
		MeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		MeshComponent->RegisterComponentWithWorld(GetWorld());
		MeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
		MeshComponent->SetRelativeLocation(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local));
		MeshComponent->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local)));
		MeshComponent->SetStaticMesh(RingMesh);
	}
}