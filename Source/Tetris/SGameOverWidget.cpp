// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameOverWidget.h"
#include "MainHUD.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"


#define LOCTEXT_NAMESPACE "MainMenu"

void SGameOverWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHud = InArgs._OwningHud;

	const FMargin ContentPadding = FMargin(500.f, 300.f);
	const FMargin ButtonPadding = FMargin(10.f);

	const FText TitleText = LOCTEXT("GameOverText", "Game Over");
	const FText PlayText = LOCTEXT("PlayAgainGame", "Play Again");
	const FText QuitText = LOCTEXT("QuitGame", "Quit");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.f;

	FSlateFontInfo ScoreTextStyle = ButtonTextStyle;
	ScoreTextStyle.Size = 50.f;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FColor::Black)
		]
	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
		[
			SNew(SVerticalBox)

			//title text
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Font(TitleTextStyle)
		.Text(TitleText)
		.Justification(ETextJustify::Center)
		]

	//score text
	+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Font(ScoreTextStyle)
		.Text(FText::FromString("Score: " + FString::FromInt(OwningHud->Score)))
		.Justification(ETextJustify::Center)
		]

	//play button
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SGameOverWidget::OnPlayClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(PlayText)
		.Justification(ETextJustify::Center)
		]
		]

	//quit button
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SGameOverWidget::OnQuitClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(QuitText)
		.Justification(ETextJustify::Center)
		]
		]

		]
		];
}

FReply SGameOverWidget::OnPlayClicked()
{
	if (OwningHud.IsValid())
	{
		OwningHud->RemoveGameOverMenu();

	}
	return FReply::Handled();
}

FReply SGameOverWidget::OnQuitClicked()
{
	if (OwningHud.IsValid())
	{
		if (APlayerController* PC = OwningHud->PlayerOwner)
		{
			PC->ConsoleCommand("quit");
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
