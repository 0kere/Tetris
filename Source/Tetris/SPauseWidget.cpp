// Fill out your copyright notice in the Description page of Project Settings.


#include "SPauseWidget.h"
#include "MainHUD.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"


#define LOCTEXT_NAMESPACE "MainMenu"

void SPauseWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHud = InArgs._OwningHud;

	const FMargin ContentPadding = FMargin(500.f, 300.f);
	const FMargin ButtonPadding = FMargin(10.f);

	const FText TitleText = LOCTEXT("PausedText", "Paused");
	const FText ResumeText = LOCTEXT("ResumeText", "Resume");
	const FText QuitText = LOCTEXT("QuitGame", "Quit");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.f;

	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.f;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.ColorAndOpacity(FLinearColor(255.f, 255.f, 255.f, 0.3f))
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

	//Resume button
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SPauseWidget::OnResumeClicked)
		[
			SNew(STextBlock)
			.Font(ButtonTextStyle)
		.Text(ResumeText)
		.Justification(ETextJustify::Center)
		]
		]

	//quit button
	+ SVerticalBox::Slot()
		.Padding(ButtonPadding)
		[
			SNew(SButton)
			.OnClicked(this, &SPauseWidget::OnQuitClicked)
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

FReply SPauseWidget::OnResumeClicked()
{
	if (OwningHud.IsValid())
	{
		OwningHud->RemovePauseMenu();
	}
	return FReply::Handled();
}

FReply SPauseWidget::OnQuitClicked()
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
