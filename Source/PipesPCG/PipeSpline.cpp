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

	// load default materials
	const ConstructorHelpers::FObjectFinder<UMaterial> DefaultPipeMaterial(TEXT("/Script/Engine.Material'/Game/StarterContent/Materials/M_Metal_Burnished_Steel.M_Metal_Burnished_Steel'"));
	PipeMaterial = DefaultPipeMaterial.Object;

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

void APipeSpline::AlignSplineToGrid()
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		FVector OriginalPointPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local);
		OriginalPointPos.X = UKismetMathLibrary::Round(OriginalPointPos.X);
		OriginalPointPos.Y = UKismetMathLibrary::Round(OriginalPointPos.Y);
		OriginalPointPos.Z = UKismetMathLibrary::Round(OriginalPointPos.Z);
		Spline->SetLocationAtSplinePoint(i, OriginalPointPos, ESplineCoordinateSpace::Type::Local);
	}
}

void APipeSpline::SpawnSplineMeshes()
{
	for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		SetUpMesh(SplineMeshComponent);

		SplineMeshComponent->SetStaticMesh(PipeMesh);

		SplineMeshComponent->SetForwardAxis(ForwardAxis, true);

		if (PipeMaterial)
		{
			// index (for meshes with multiple materials), material reference
			SplineMeshComponent->SetMaterial(0, PipeMaterial);
		}

		SplineMeshComponent->SetStartAndEnd(
			Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Type::Local),
			Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Type::Local),
			true);
	}
}

void APipeSpline::CreateBrace(FVector Location, FVector Tangent)
{
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	SetUpMesh(MeshComponent);
	MeshComponent->SetRelativeLocation(Location);
	MeshComponent->SetRelativeRotation(UKismetMathLibrary::MakeRotFromX(Tangent));
	MeshComponent->SetStaticMesh(RingMesh);
}

void APipeSpline::SpawnStaticMeshes()
{
	if (BraceFrequency > 0)
	{
		int NumBraces = Spline->GetSplineLength() / BraceFrequency;

		float Offset = (Spline->GetSplineLength() - ((NumBraces-1) * BraceFrequency))/2;

		for (int i = 0; i < NumBraces; i++)
		{
			FVector Loc = Spline->GetLocationAtDistanceAlongSpline((BraceFrequency * i)+Offset, ESplineCoordinateSpace::Type::Local);
			FVector Tang = Spline->GetTangentAtDistanceAlongSpline((BraceFrequency * i)+Offset, ESplineCoordinateSpace::Type::Local);
			CreateBrace(Loc, Tang);
		}
	}
	if (CapEnds)
	{
		CreateBrace(
			Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Type::Local)
		);
		CreateBrace(
			Spline->GetLocationAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Type::Local),
			Spline->GetTangentAtSplinePoint(Spline->GetNumberOfSplinePoints(), ESplineCoordinateSpace::Type::Local)
		);
	}
}

void APipeSpline::OnConstruction(const FTransform& Transform)
{
	AlignSplineToGrid();
	SpawnSplineMeshes();
	SpawnStaticMeshes();
}