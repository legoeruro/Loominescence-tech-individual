// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FlockingManager.h"
#include "FlockingGameModeBase.generated.h"

UCLASS()
class FLOCKING_API AFlockingGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFlockingGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flocking")
	class UStaticMeshComponent * AgentMesh;

	UPROPERTY(BlueprintReadOnly, Category="Flocking")
	UFlockingManager *Manager;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};