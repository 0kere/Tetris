// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class TETRIS_API AMainHUD : public AHUD
{
	GENERATED_BODY()

protected:
	TSharedPtr<class SMainMenuWidget> MenuWidget;
	TSharedPtr<class SWidget> MenuWidgetContainer;

	TSharedPtr<class SInGameWidget> GameWidget;
	TSharedPtr<class SWidget> GameWidgetContainer;

	TSharedPtr<class SPauseWidget> PauseWidget;
	TSharedPtr<class SWidget> PauseWidgetContainer;

	TSharedPtr<class SGameOverWidget> GameOverWidget;
	TSharedPtr<class SWidget> GameOverWidgetContainer;

	virtual void BeginPlay() override;


public:
	bool isPaused;

	void ShowMenu();
	void RemoveMenu();

	void ShowGameMenu();
	void RemoveGameMenu();

	void ShowPauseMenu();
	void RemovePauseMenu();

	void ShowGameOverMenu();
	void RemoveGameOverMenu();

	void ScoreUpdated(int score);
	void LevelUpdated(int level);
	void FluxChargeUpdated(int fluxCharge);

	int Score;
};
