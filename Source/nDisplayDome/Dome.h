// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Dome.generated.h"

UCLASS()
class NDISPLAYDOME_API ADome : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADome();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void PostLoad();
	virtual void PostActorCreated();
	virtual void OnConstruction(const FTransform& Transform);
	void CreateDome();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProceduralMeshComponent* Mesh;

	/** The Field of view for this dome, in degrees*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float FieldOfView;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius;

	/** An UV rotational offset in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float UVRotation;
};
