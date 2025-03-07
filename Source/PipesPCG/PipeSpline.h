#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "PipeSpline.generated.h"

UCLASS()
class PIPESPCG_API APipeSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APipeSpline();

	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "PipeSpline")
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, Category = "PipeSpline")
	UStaticMesh* PipeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PipeSpline")
	UStaticMesh* CubeMesh;

	void OnConstruction(const FTransform& Transform) override;
};
