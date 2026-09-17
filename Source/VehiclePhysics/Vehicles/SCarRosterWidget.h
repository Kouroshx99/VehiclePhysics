#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SVerticalBox;

/**
 * The car list that appears in the corner when you switch.
 *
 * Deliberately not a menu. It takes no input and never pauses the game - it is a
 * readout of what the button just did, because on a pad the button IS the interface
 * and a list you have to navigate would be slower than the thing it replaces.
 */
class SCarRosterWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCarRosterWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/** Replaces the list and highlights a row. */
	void SetRoster(const TArray<FText>& InNames, int32 InCurrent);

	/** Shows the list, then fades it after this many seconds. */
	void Flash(float Seconds);

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime,
		const float InDeltaTime) override;

private:
	void Rebuild();

	TArray<FText> Names;
	int32 Current = INDEX_NONE;

	/** Counts down while shown; the last second of it is the fade. */
	float Remaining = 0.f;
	float FlashSeconds = 0.f;

	TSharedPtr<SVerticalBox> List;
};
