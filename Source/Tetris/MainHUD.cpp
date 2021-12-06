// Fill out your copyright notice in the Description page of Project Settings.

//code adapted from (Ward, 2020)
#include "MainHUD.h"
#include "SMainMenuWidget.h"
#include "SInGameWidget.h"
#include "SPauseWidget.h"
#include "SGameOverWidget.h"

#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMainMenuWidget).OwningHud(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMainHUD::ShowMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		MenuWidget = SNew(SMainMenuWidget).OwningHud(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(MenuWidgetContainer, SWeakWidget).PossiblyNullContent(MenuWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}

		for (TActorIterator<APlayfield> pfItr(GetWorld()); pfItr; ++pfItr)
		{
			pfItr->GameOver();
		}
	}
}

void AMainHUD::RemoveMenu()
{
	if (GEngine && MenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(MenuWidgetContainer.ToSharedRef());
		
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}

		for (TActorIterator<APlayfield> pfItr(GetWorld()); pfItr; ++pfItr)
		{
			pfItr->StartGame();
			break;
		}
		ShowGameMenu();
	}
}
//end of adapted code
void AMainHUD::ShowGameMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		GameWidget = SNew(SInGameWidget).OwningHud(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameWidgetContainer, SWeakWidget).PossiblyNullContent(GameWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMainHUD::RemoveGameMenu()
{
	if (GEngine && GameWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(GameWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
}

void AMainHUD::ShowPauseMenu()
{
	if (GEngine && GEngine->GameViewport && GameWidgetContainer.IsValid())
	{
		RemoveGameMenu();

		PauseWidget = SNew(SPauseWidget).OwningHud(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(PauseWidgetContainer, SWeakWidget).PossiblyNullContent(PauseWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
		isPaused = true;
		UGameplayStatics::SetGamePaused(GetWorld(), isPaused); //adapted from (Jason.J, 2017)
		

	}
}

void AMainHUD::RemovePauseMenu()
{
	if (GEngine && PauseWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(PauseWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
		isPaused = false;
		UGameplayStatics::SetGamePaused(GetWorld(), isPaused);

		ShowGameMenu();
	}
}

void AMainHUD::ShowGameOverMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		RemoveGameMenu();

		GameOverWidget = SNew(SGameOverWidget).OwningHud(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(GameOverWidgetContainer, SWeakWidget).PossiblyNullContent(GameOverWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMainHUD::RemoveGameOverMenu()
{
	if (GEngine && GameOverWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(GameOverWidgetContainer.ToSharedRef());

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
		for (TActorIterator<APlayfield> pfItr(GetWorld()); pfItr; ++pfItr)
		{
			pfItr->RestartGame();
			break;
		}
		ShowGameMenu();
	}
}

void AMainHUD::ScoreUpdated(int score)
{
	GameWidget->OnScoreChanged(score);
	Score = score;
}

void AMainHUD::LevelUpdated(int level)
{
	GameWidget->OnLevelChanged(level);
}

void AMainHUD::FluxChargeUpdated(int fluxCharge)
{
	GameWidget->OnFluxChargeChanged(fluxCharge);
}
