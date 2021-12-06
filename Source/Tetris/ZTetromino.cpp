// Fill out your copyright notice in the Description page of Project Settings.


#include "ZTetromino.h"

AZTetromino::AZTetromino()
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> mat(TEXT("MaterialInterface'/Game/Materials/mZTetromino.mZTetromino'"));

	if (mat.Succeeded())
	{
		UMaterialInstanceDynamic* instance = UMaterialInstanceDynamic::Create(mat.Object, NULL);
		thisMat = instance;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ghost(TEXT("MaterialInterface'/Game/Materials/mGhostBlock.mGhostBlock'"));

	if (ghost.Succeeded())
	{
		UMaterialInstanceDynamic* instanceGhost = UMaterialInstanceDynamic::Create(ghost.Object, NULL);
		ghostMat = instanceGhost;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> flux(TEXT("/Game/Materials/FluxMaterials/mZFluxBlock.mZFluxBlock"));

	if (flux.Succeeded())
	{
		UMaterialInstanceDynamic* instanceFlux = UMaterialInstanceDynamic::Create(flux.Object, NULL);

		//instanceFlux->SetVectorParameterValue("Color", matColor);
		fluxMat = instanceFlux;
	}
}

void AZTetromino::BeginPlay()
{
	Super::BeginPlay();
	index = 6;
	//Spawn coords for this shape
	amountOfMinos = 4;
	spawningCoords.Init(FVector2D(0), amountOfMinos);
	spawningCoords[0] = FVector2D(0, 0);
	spawningCoords[1] = FVector2D(-1, -1);
	spawningCoords[2] = FVector2D(1, 0);
	spawningCoords[3] = FVector2D(0, -1);

	//rotation state 1 for this shape
	rotStateOne.Init(FVector2D(0), amountOfMinos);
	rotStateOne[0] = FVector2D(0, 0);
	rotStateOne[1] = FVector2D(0, 1);
	rotStateOne[2] = FVector2D(1, 0);
	rotStateOne[3] = FVector2D(1, -1);
	//rotation state 2 for this shape
	rotStateTwo.Init(FVector2D(0), amountOfMinos);
	rotStateTwo[0] = FVector2D(0, 0);
	rotStateTwo[1] = FVector2D(-1, 0);
	rotStateTwo[2] = FVector2D(1, 1);
	rotStateTwo[3] = FVector2D(0, 1);
	//rotation state 3 for this shape
	rotStateThree.Init(FVector2D(0), amountOfMinos);
	rotStateThree[0] = FVector2D(0, 0);
	rotStateThree[1] = FVector2D(0, -1);
	rotStateThree[2] = FVector2D(-1, 0);
	rotStateThree[3] = FVector2D(-1, 1);

}