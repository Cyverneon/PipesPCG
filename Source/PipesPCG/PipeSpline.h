#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PipeSpline.generated.h"

UCLASS()
class PIPESPCG_API APipeSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APipeSpline();

	USceneComponent* SceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh* CubeMesh;

	void OnConstruction(const FTransform& Transform) override;
};
