// Fill out your copyright notice in the Description page of Project Settings.


#include "Dome.h"

// Sets default values
ADome::ADome()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = Mesh;
	Mesh->bUseAsyncCooking = true;
	FieldOfView = 180.f;
	Radius = 100.f;
	UVRotation = 0.f;
}

// Called when the game starts or when spawned
void ADome::BeginPlay()
{
	Super::BeginPlay();
	CreateDome();
	
}

// Called every frame
void ADome::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADome::PostLoad()
{
	Super::PostLoad();
	CreateDome();
}

void ADome::PostActorCreated()
{
	Super::PostActorCreated();
	CreateDome();
}
void ADome::OnConstruction(const FTransform& Transform) 
{
	CreateDome();
}

void ADome::CreateDome()
{	
	float lift = (180.0f - FieldOfView) / 2.0f;
	int azimuthSteps = 256;
	int elevationSteps = 64;
	float uvRotationRad = FMath::DegreesToRadians(UVRotation);

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;

	//This is lazily copied code that used triangle_fans as indexlists
	//The vertex-generation was kept as-is and produces an index list in the end 
	//from the fan_indices.
	TArray<int32> fan_indices;
	TArray<int32> indices;
	

	int e, numVerts = 0;
	float x, y, z;
	float s, t;
	float elevation, azimuth;
	float de; //delta elevation
	e = 0;

	de = static_cast<float>(e) / static_cast<float>(elevationSteps);
	elevation = FMath::DegreesToRadians(lift + de * (90.0f - lift));
	z = sinf(elevation);
	const float liftRatio = 1.0f - (lift / 90.0f);
	for (int a = 0; a < azimuthSteps; a++)
	{
		azimuth = FMath::DegreesToRadians(static_cast<float>(a * 360) / static_cast<float>(azimuthSteps));

		x = cosf(elevation) * sinf(azimuth);
		y = -cosf(elevation) * cosf(azimuth);

		s = liftRatio * (static_cast<float>(elevationSteps - e) / static_cast<float>(elevationSteps)) * sinf(azimuth + uvRotationRad);
		t = liftRatio * (static_cast<float>(elevationSteps - e) / static_cast<float>(elevationSteps)) * -cosf(azimuth + uvRotationRad);
		s = (s * 0.5f) + 0.5f;
		t = (t * 0.5f) + 0.5f;

		vertices.Add(FVector(x * Radius, y * Radius, z * Radius));
		normals.Add(FVector(x, y, z));
		UV0.Add(FVector2D(s, 1.0f-t));
	}

	for (e = 1; e <= elevationSteps - 1; e++)
	{
		de = static_cast<float>(e) / static_cast<float>(elevationSteps);
		elevation = FMath::DegreesToRadians(lift + de * (90.0f - lift));

		z = sinf(elevation);

		for (int a = 0; a < azimuthSteps; a++)
		{
			azimuth = FMath::DegreesToRadians(static_cast<float>(a * 360) / static_cast<float>(azimuthSteps));

			x = cosf(elevation) * sinf(azimuth);
			y = -cosf(elevation) * cosf(azimuth);

			s = liftRatio * (static_cast<float>(elevationSteps - e) / static_cast<float>(elevationSteps)) * sinf(azimuth + uvRotationRad);
			t = liftRatio * (static_cast<float>(elevationSteps - e) / static_cast<float>(elevationSteps)) * -cosf(azimuth + uvRotationRad);
			s = (s * 0.5f) + 0.5f;
			t = (t * 0.5f) + 0.5f;

			vertices.Add(FVector(x * Radius, y * Radius, z * Radius));
			normals.Add(FVector(x, y, z));
			UV0.Add(FVector2D(s, 1.0f-t));

			fan_indices.Add(numVerts);
			fan_indices.Add(azimuthSteps + numVerts++);
		}

		fan_indices.Add(numVerts - azimuthSteps);
		fan_indices.Add(numVerts);
	}

	e = elevationSteps;
	de = static_cast<float>(e) / static_cast<float>(elevationSteps);
	elevation = FMath::DegreesToRadians(lift + de * (90.0f - lift));

	z = sinf(elevation);

	vertices.Add(FVector(0.f, 0.f, z*Radius));
	normals.Add(FVector(0.f, 0.f, 1.f));
	UV0.Add(FVector2D(.5f, .5f));

	fan_indices.Add(numVerts + azimuthSteps);

	for (int a = 1; a <= azimuthSteps; a++)
		fan_indices.Add(numVerts + azimuthSteps - a);
	fan_indices.Add(numVerts + azimuthSteps - 1);

	//In order to create the triangle index list:

	unsigned int offset = 0;
	unsigned int size = 0;

	int lastIndexOnRing = azimuthSteps - 1;
	for (int j = 0; j < elevationSteps; ++j) {
		for (int i = 0; i < azimuthSteps - 1; ++i)
		{
			indices.Add(j*azimuthSteps + i);
			indices.Add(j*azimuthSteps + i + 1);
			indices.Add(j*azimuthSteps + i + azimuthSteps);
			indices.Add(j*azimuthSteps + i + 1);
			indices.Add(j*azimuthSteps + i + azimuthSteps + 1);
			indices.Add(j*azimuthSteps + i + azimuthSteps);
		}
		indices.Add(j*azimuthSteps + lastIndexOnRing);
		indices.Add(j*azimuthSteps + 0);
		indices.Add(j*azimuthSteps + lastIndexOnRing + azimuthSteps);
		indices.Add(j*azimuthSteps + 0);
		indices.Add(j*azimuthSteps + 0 + azimuthSteps);
		indices.Add(j*azimuthSteps + lastIndexOnRing + azimuthSteps);
	}


	Mesh->CreateMeshSection_LinearColor(0, vertices, indices, normals, UV0, {}, {}, true);

	// Enable collision data
	Mesh->ContainsPhysicsTriMeshData(true);
}

