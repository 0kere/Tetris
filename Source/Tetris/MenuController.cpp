// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuController.h"
#include "MainHUD.h"

AMenuController::AMenuController()
{

}

void AMenuController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMenuController::TogglePause).bExecuteWhenPaused = true; //update this to pause menu which has a button with an onClick to return to main menu
	}
}

void AMenuController::TogglePause()
{
		if (AMainHUD* MainHUD = Cast<AMainHUD>(GetHUD()))
		{
			
			if (MainHUD->isPaused)
			{
				MainHUD->RemovePauseMenu();
			}
			else
			{
				MainHUD->ShowPauseMenu();
			}
		}

}
