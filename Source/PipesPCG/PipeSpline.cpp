#include "PipeSpline.h"

// Sets default values
APipeSpline::APipeSpline()
{
	PrimaryActorTick.bCanEverTick = false;
	InitializeProperties();
}

void APipeSpline::InitializeProperties()
{
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

void APipeSpline::SetUpMesh(UPrimitiveComponent* Component) const
{
	Component->SetMobility(EComponentMobility::Movable);
    Component->CreationMethod = EComponentCreationMethod::UserConstructionScript;
    Component->RegisterComponentWithWorld(GetWorld());
    Component->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);
}

void APipeSpline::SpawnSplineMeshes()
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints()-1; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		
		SetUpMesh(SplineMeshComponent);

		SplineMeshComponent->SetStaticMesh(PipeMesh);

		SplineMeshComponent->SetForwardAxis(ForwardAxis, true);
		
		SplineMeshComponent->SetStartAndEnd(
			Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local),
			Spline->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Type::Local),
			true);
	}
}

void APipeSpline::SpawnStaticMeshes()
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints(); i++)
	{
		UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
		SetUpMesh(MeshComponent);
		MeshComponent->SetRelativeLocation(Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local));
		MeshComponent->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local)));
		MeshComponent->SetStaticMesh(RingMesh);
	}
}

void APipeSpline::OnConstruction(const FTransform& Transform)
{
	SpawnSplineMeshes();
	SpawnStaticMeshes();
}