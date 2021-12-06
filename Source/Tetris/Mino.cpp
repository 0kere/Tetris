// Fill out your copyright notice in the Description page of Project Settings.

#include "Mino.h"
#include "Playfield.h"

// Sets default values
AMino::AMino()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	visibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	visibleComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> visualAsset(TEXT("/Game/Mino.Mino"));
	if (visualAsset.Succeeded())
	{
		visibleComponent->SetStaticMesh(visualAsset.Object);
		visibleComponent->SetRelativeLocation(FVector(0));
		visibleComponent->SetWorldScale3D(FVector(0.1f));
	}
}

// Called when the game starts or when spawned
void AMino::BeginPlay()
{
	Super::BeginPlay();
}

void AMino::Init(FVector2D coord, UMaterialInterface* mat, APlayfield* pf, bool isFlux_, int index)
{
	tetrominoIndex = index;
	isFlux = isFlux_;
	curCoords = coord;
	SetPosition(coord);
	//visibleComponent->GetStaticMesh()->SetMaterial(0, mat);
	visibleComponent->SetMaterial(0, mat);
	visibleComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(0, mat);

	playfield = pf;
}

// Called every frame
void AMino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMino::Move(FVector2D nextCoord, float timer)
{
	FVector cur = FVector(curCoords.X * -width, this->GetActorLocation().Y, curCoords.Y * -width);
	FVector next = FVector(nextCoord.X * -width, this->GetActorLocation().Y, nextCoord.Y * -width);
	this->SetActorLocation(FMath::Lerp(cur, next, timer));
}

void AMino::Flux(int dir, float _speed)
{
	fluxLerpTimer = 0.f;
	isFluxing = true;
	isMoving = true;
	direction = dir;
	speed = _speed;
}

void AMino::Destroy()
{
	AActor::Destroy();
}

void AMino::SetPosition(FVector2D coords)
{
	curCoords = coords;
	this;
	if (this->SetActorLocation(FVector(coords.X * -width, this->GetActorLocation().Y, coords.Y * -width)))
	{
		this->SetActorLocation(FVector(coords.X * -width, this->GetActorLocation().Y, coords.Y * -width)); //neg width as grid is upside down 
	}
}


