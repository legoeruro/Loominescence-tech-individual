#include "FlockingManager.h"
#include "Agent.h"

#define AGENT_COUNT 40    

void UFlockingManager::Init( UWorld *world, UStaticMeshComponent *mesh ) {
	UE_LOG(LogTemp, Warning, TEXT("MANAGER INIT"));

	World = world;

	float incr = (PI * 2.f) / AGENT_COUNT;
	for( int i = 0; i < AGENT_COUNT; i++ ) {
		if( World != nullptr ) {
			FRotator rotation = FRotator();

			FVector location = FVector();
			location.X = FMath::Sin( incr * i ) * 150.f;
			location.Z = FMath::Cos( incr * i ) * 150.f;

			location.X += FMath::RandRange(-50.f, 50.f);
			location.Y += FMath::RandRange(-50.f, 50.f);
			location.Z += FMath::RandRange(-50.f, 50.f);

			AAgent * agent = World->SpawnActor<AAgent>( location, rotation );
			agent->SetActorScale3D(FVector(0.05f));
			agent->Init( mesh, i );
			Agents.Add( agent );
		}
	}

	initialized = true;
}

void UFlockingManager::LimitVector(FVector& v, float max) {
	if (v.Size() > max) {
		v = v.GetSafeNormal() * max;
	}
}

FVector UFlockingManager::RuleSeparation(AAgent* agent) {
	FVector steer = FVector::ZeroVector;
	int count = 0;

	for (AAgent* other : Agents) {
		if (other == agent) continue;

		float d = FVector::Dist(agent->GetActorLocation(), other->GetActorLocation());
		if (d < DesiredSeparation && d > 0) {
			FVector diff = agent->GetActorLocation() - other->GetActorLocation();
			diff.Normalize();
			diff /= d;
			steer += diff;
			count++;
		}
	}

	if (count > 0)
		steer /= count;

	LimitVector(steer, MaxForce);
	return steer;
}

FVector UFlockingManager::RuleAlignment(AAgent* agent) {
	FVector sum = FVector::ZeroVector;
	int count = 0;

	for (AAgent* other : Agents) {
		if (other == agent) continue;

		float d = FVector::Dist(agent->GetActorLocation(), other->GetActorLocation());
		if (d < NeighborRadius) {
			sum += other->Velocity;
			count++;
		}
	}

	if (count > 0) {
		sum /= count;    // average heading
		sum = sum.GetSafeNormal() * MaxSpeed;
		FVector steer = sum - agent->Velocity;
		LimitVector(steer, MaxForce);
		return steer;
	}

	return FVector::ZeroVector;
}

FVector UFlockingManager::RuleCohesion(AAgent* agent) {
	FVector center = FVector::ZeroVector;
	int count = 0;

	for (AAgent* other : Agents) {
		if (other == agent) continue;

		float d = FVector::Dist(agent->GetActorLocation(), other->GetActorLocation());
		if (d < NeighborRadius) {
			center += other->GetActorLocation();
			count++;
		}
	}

	if (count > 0) {
		center /= count;
		FVector desired = center - agent->GetActorLocation();
		desired = desired.GetSafeNormal() * MaxSpeed;
		FVector steer = desired - agent->Velocity;
		LimitVector(steer, MaxForce);
		return steer;
	}

	return FVector::ZeroVector;
}

void UFlockingManager::Flock(float DeltaTime) {
	if (!initialized) return;

	for (AAgent* agent : Agents) {

		FVector sep = RuleSeparation(agent);
		FVector ali = RuleAlignment(agent);
		FVector coh = RuleCohesion(agent);

		FVector accel =
			sep * seperationWeight +
			ali * alignmentWeight +
			coh * cohesionWeight;

		agent->Velocity += accel * DeltaTime;
		LimitVector(agent->Velocity, MaxSpeed);
	}
}