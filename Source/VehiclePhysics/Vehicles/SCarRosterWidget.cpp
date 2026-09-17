#include "SCarRosterWidget.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/CoreStyle.h"

namespace
{
	/** Seconds of fade at the end of the flash. */
	constexpr float FadeSeconds = 0.6f;
}

void SCarRosterWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
		.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.55f))
		.Padding(FMargin(14.f, 10.f))
		[
			SAssignNew(List, SVerticalBox)
		]
	];

	// Starts hidden. Nothing has been switched yet, so there is nothing to report.
	SetVisibility(EVisibility::Hidden);
}

void SCarRosterWidget::SetRoster(const TArray<FText>& InNames, int32 InCurrent)
{
	Names = InNames;
	Current = InCurrent;
	Rebuild();
}

void SCarRosterWidget::Rebuild()
{
	if (!List.IsValid())
	{
		return;
	}

	List->ClearChildren();

	for (int32 i = 0; i < Names.Num(); ++i)
	{
		const bool bIsCurrent = (i == Current);

		// The current row is the only one at full white; the rest sit back far enough
		// to be read as context rather than as options competing with it.
		const FLinearColor Colour = bIsCurrent
			? FLinearColor(1.f, 1.f, 1.f, 1.f)
			: FLinearColor(1.f, 1.f, 1.f, 0.45f);

		List->AddSlot()
			.AutoHeight()
			.Padding(0.f, 2.f)
			[
				SNew(STextBlock)
				.Text(FText::Format(
					NSLOCTEXT("CarRoster", "Row", "{0}  {1}"),
					FText::FromString(bIsCurrent ? TEXT(">") : TEXT("   ")),
					Names[i]))
				.ColorAndOpacity(FSlateColor(Colour))
				.Font(FCoreStyle::GetDefaultFontStyle(bIsCurrent ? "Bold" : "Regular", 15))
			];
	}
}

void SCarRosterWidget::Flash(float Seconds)
{
	FlashSeconds = FMath::Max(Seconds, 0.f);
	Remaining = FlashSeconds;

	if (Remaining <= 0.f)
	{
		SetVisibility(EVisibility::Hidden);
		return;
	}

	// HitTestInvisible, not Visible: the list must never eat a click or steal focus
	// from the car, which is the whole reason it is a readout and not a menu.
	SetVisibility(EVisibility::HitTestInvisible);
	SetRenderOpacity(1.f);
}

void SCarRosterWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
	const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (Remaining <= 0.f)
	{
		return;
	}

	Remaining -= InDeltaTime;

	if (Remaining <= 0.f)
	{
		Remaining = 0.f;
		SetVisibility(EVisibility::Hidden);
		return;
	}

	SetRenderOpacity(FMath::Clamp(Remaining / FadeSeconds, 0.f, 1.f));
}
