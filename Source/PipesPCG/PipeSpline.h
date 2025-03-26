#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "PipeSpline.generated.h"

UCLASS()
class PIPESPCG_API APipeSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	APipeSpline();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	UPROPERTY(VisibleAnywhere, Category = "PipeSpline")
	USplineComponent* Spline;

	/* Mesh used for the pipe body */
	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	UStaticMesh* PipeMesh;
	
	/* Mesh used for the decorative rings around the pipe at each spline point */
	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	UStaticMesh* BraceMesh;
	
	/* Forward axis for the pipe mesh */
	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	int BraceFrequency;

	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	bool CapEnds;

	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	UMaterial* PipeMaterial;

	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	UMaterial* BraceMaterial;

	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void InitializeProperties();
	void SetUpMesh(UPrimitiveComponent* Component) const;
	void CreateBrace(FVector Location, FVector Tangent);
	void AlignSplineToGrid(int GridSize);
	void SpawnSplineMeshes();
	void SpawnStaticMeshes();
};
