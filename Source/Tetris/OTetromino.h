// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "OTetromino.generated.h"

/**
 *
 */
UCLASS()
class TETRIS_API AOTetromino : public ATetromino
{
	GENERATED_BODY()
public:
	AOTetromino();
	virtual void BeginPlay() override;
};
