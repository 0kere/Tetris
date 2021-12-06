// Fill out your copyright notice in the Description page of Project Settings.


#include "SInGameWidget.h"
#include "MainHUD.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/World.h"


void SInGameWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;
	OwningHud = InArgs._OwningHud;

	const FMargin ContentPadding = FMargin(10.f, 300.f);

	FSlateFontInfo TextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TextStyle.Size = 35.f;

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		.Padding(ContentPadding)
		[
			SNew(SVerticalBox)

			//score text
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FColor::Black)
			.Font(TextStyle)
		.Text(GetScoreText())
		.Text_Lambda([this]()->FText {return GetScoreText(); }) //code adapted from (antithing, 2018)
		.Justification(ETextJustify::Right)
		]

	//Level text
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FColor::Black)
			.Font(TextStyle)
		.Text(GetLevelText())
		.Text_Lambda([this]()->FText {return GetLevelText(); })
		.Justification(ETextJustify::Right)
		]

	//Flux charge text
	+ SVerticalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.ColorAndOpacity(FColor::Black)
			.Font(TextStyle)
		.Text(GetFluxChargeText())
		.Text_Lambda([this]()->FText {return GetFluxChargeText(); })
		.Justification(ETextJustify::Right)
		]

		]
		];
}

void SInGameWidget::OnScoreChanged(int _score)
{
	score = _score;
}

void SInGameWidget::OnLevelChanged(int _level)
{
	level = _level;
}

void SInGameWidget::OnFluxChargeChanged(int _fluxCharge)
{
	fluxCharge = _fluxCharge;
}
