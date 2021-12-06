// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tetromino.h"
#include "LTetromino.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API ALTetromino : public ATetromino
{
	GENERATED_BODY()

public:
	ALTetromino();
	virtual void BeginPlay() override;
};
