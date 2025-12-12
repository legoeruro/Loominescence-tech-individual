#pragma once

#include "CoreMinimal.h"
#include "FlockingManager.generated.h"

UCLASS(BlueprintType, Blueprintable)
class FLOCKING_API UFlockingManager : public UObject
{

public:
	GENERATED_BODY()

	void Init( UWorld *world, UStaticMeshComponent *mesh );
	void Flock(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	const TArray<AAgent*>& GetAgents() const { return Agents; }

	// Tuning parameters for boids algorithm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids")
	float NeighborRadius = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids")
	float DesiredSeparation = 80.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids")
	float MaxSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids")
	float MaxForce = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids|Weights")
	float seperationWeight = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids|Weights")
	float alignmentWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Boids|Weights")
	float cohesionWeight = 1.0f;

private:
	UWorld *World;	
	bool initialized;
	TArray<class AAgent *> Agents;

	FVector RuleSeparation(AAgent* agent);
	FVector RuleAlignment(AAgent* agent);
	FVector RuleCohesion(AAgent* agent);

	void LimitVector(FVector& v, float max);
};