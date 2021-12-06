// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "TTetromino.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ATTetromino : public ATetromino
{
	GENERATED_BODY()
public:
	ATTetromino();
	virtual void BeginPlay() override;
};
