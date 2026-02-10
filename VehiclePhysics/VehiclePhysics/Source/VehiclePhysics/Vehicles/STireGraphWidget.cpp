#include "STireGraphWidget.h"

#include "Brushes/SlateColorBrush.h"
#include "GoogleInstantPreview/instant_preview_server.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/CoreStyle.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Physics/PacejkaResult.h"

void STireGraphWidget::Construct(const FArguments& InArgs)
{
	Asset = InArgs._Asset;
	bIsInteractive = InArgs._bIsInteractive;

	ChildSlot
	[
		SNew(SOverlay)
	    + SOverlay::Slot()
	    .HAlign(HAlign_Fill)
	    .VAlign(VAlign_Fill)
	    [
	    	SNew(SBorder)
	    	.BorderBackgroundColor(FLinearColor(0.05f,0.05f,0.05f,0.8f))
	    	[
	    		SNullWidget::NullWidget // painting is done in OnPaint
	    	]
	    ]
	];
}

void STireGraphWidget::DrawBackground(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		WhiteBrush,
		ESlateDrawEffect::None,
		FLinearColor(0.02f, 0.02f, 0.02f, 0.5f));
}

int32 STireGraphWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D Size = AllottedGeometry.GetLocalSize();

	// --- Background ---
	DrawBackground(AllottedGeometry, OutDrawElements, LayerId);
	
	const float Margin = 20.0f; // padding inside square

	// --- Grid Lines (square cells) ---
	const float GridStepWorld = 0.1f; // world units
	FVector2D TopLeftWorld = ScreenToWorld(FVector2D(0,0), Size);
	FVector2D BottomRightWorld = ScreenToWorld(Size, Size);
	FVector2D GraphTopLeft = FVector2D(Margin, Margin);
	FVector2D GraphBottomRight = Size - FVector2D(Margin, Margin);
	
	// Vertical grid lines
	float FirstX = FMath::GridSnap(TopLeftWorld.X, GridStepWorld);
	for (double X = FirstX; X < BottomRightWorld.X; X += GridStepWorld)
	{
		FVector2D A = WorldToScreen(FVector2D(X, TopLeftWorld.Y), Size);
		FVector2D B = WorldToScreen(FVector2D(X, BottomRightWorld.Y), Size);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 1,
			AllottedGeometry.ToPaintGeometry(),
			{A, B},
			ESlateDrawEffect::None,
			FLinearColor(0.15f,0.15f,0.15f,1.f),
			true, 1.f
		);
	}

	// Horizontal grid lines
	float FirstY = FMath::GridSnap(BottomRightWorld.Y, GridStepWorld);
	for (double Y = FirstY; Y < TopLeftWorld.Y; Y += GridStepWorld)
	{
		FVector2D A = WorldToScreen(FVector2D(TopLeftWorld.X, Y), Size);
		FVector2D B = WorldToScreen(FVector2D(BottomRightWorld.X, Y), Size);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 1,
			AllottedGeometry.ToPaintGeometry(),
			{A, B},
			ESlateDrawEffect::None,
			FLinearColor(0.15f,0.15f,0.15f,1.f),
			true, 1.f
		);
	}
	
	// --- Axis Labels ---
	float FontSize = bIsInteractive ?  10.f : 8.f; // Fixed size in-game
	float LabelOffset = 10.f; // Pixels from axis
	float EdgeMargin = 30.f; // Keep labels further from widget edges

    // Vertical axis labels (Y-axis)
    for (double Val = -1.2f; Val <= 1.2f; Val += GridStepWorld * (bIsInteractive ? 1 : 2)) // Reduced range for clarity
    {
        FVector2D WorldPos = FVector2D(0.f, Val);
        FVector2D ScreenPos = WorldToScreen(WorldPos, Size);
        ScreenPos.X = FMath::Clamp(ScreenPos.X, EdgeMargin, Size.X - EdgeMargin); // Clip to widget bounds
        ScreenPos.Y = FMath::Clamp(ScreenPos.Y, EdgeMargin, Size.Y - EdgeMargin);

        FString Label = FString::Printf(TEXT("%.1f"), Val);

        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 2,
            AllottedGeometry.ToPaintGeometry(
                FVector2D(GraphTopLeft.X - LabelOffset + 0.f, ScreenPos.Y - FontSize / 2), // Left of Y-axis
                FVector2D(1.f, 1.f)
            ),
            FText::FromString(Label),
            FCoreStyle::GetDefaultFontStyle("Regular", FontSize),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
    }

    // Horizontal axis labels (X-axis)
    for (double Val = -1.4f; Val <= 1.4f; Val += GridStepWorld * (bIsInteractive ? 1 : 2)) // Reduced range
    {
        FVector2D WorldPos = FVector2D(Val, 0.f);
        FVector2D ScreenPos = WorldToScreen(WorldPos, Size);
        ScreenPos.X = FMath::Clamp(ScreenPos.X, EdgeMargin, Size.X - EdgeMargin);
        ScreenPos.Y = FMath::Clamp(ScreenPos.Y, EdgeMargin, Size.Y - EdgeMargin);

        FString Label = FString::Printf(TEXT("%.1f"), Val);

        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 2,
            AllottedGeometry.ToPaintGeometry(
                FVector2D(ScreenPos.X - FontSize / 2, GraphBottomRight.Y + LabelOffset - 10.f), // Below X-axis
                FVector2D(1.f, 1.f)
            ),
            FText::FromString(Label),
            FCoreStyle::GetDefaultFontStyle("Regular", FontSize),
            ESlateDrawEffect::None,
            FLinearColor::White
        );
    }

	// --- Axes ---
	TArray<FVector2D> AxisPoints;
	AxisPoints.Add(FVector2D(GraphTopLeft.X, GraphBottomRight.Y)); // X axis start
	AxisPoints.Add(FVector2D(GraphBottomRight.X, GraphBottomRight.Y)); // X axis end
	AxisPoints.Add(FVector2D(GraphTopLeft.X, GraphBottomRight.Y)); // Y axis start
	AxisPoints.Add(FVector2D(GraphTopLeft.X, GraphTopLeft.Y)); // Y axis end

	// --- Zero Axes ---
	{
		// Vertical line at X=0
		FVector2D ZA = WorldToScreen(FVector2D(0, TopLeftWorld.Y), Size);
		FVector2D ZB = WorldToScreen(FVector2D(0, BottomRightWorld.Y), Size);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 2,
			AllottedGeometry.ToPaintGeometry(),
			{ZA, ZB},
			ESlateDrawEffect::None,
			FLinearColor::White,
			true, 2.f
		);

		// Horizontal line at Y=0
		FVector2D HA = WorldToScreen(FVector2D(TopLeftWorld.X, 0),Size);
		FVector2D HB = WorldToScreen(FVector2D(BottomRightWorld.X, 0), Size);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 2,
			AllottedGeometry.ToPaintGeometry(),
			{HA, HB},
			ESlateDrawEffect::None,
			FLinearColor::White,
			true, 2.f
		);
	}

	// --- Find largest square that fits inside widget ---
	float GraphSize = FMath::Min(Size.X, Size.Y); 
	FVector2D GraphOrigin(
		(Size.X - GraphSize) * .5f, // left padding if width > height
		(Size.Y - GraphSize) * .5f  // top padding if height > width
	);

	// --- Helper mapping functions ---
	auto MapX = [&](float Input, float Min, float Max) -> float
	{
		return FMath::GetMappedRangeValueClamped(
			FVector2D(Min, Max),
			FVector2D(GraphTopLeft.X, GraphBottomRight.X),
			Input
		);
	};

	auto MapY = [&](float Input, float Min, float Max) -> float
	{
		return FMath::GetMappedRangeValueClamped(
			FVector2D(Min, Max),
			FVector2D(GraphBottomRight.Y, GraphTopLeft.Y), // flip Y
			Input
		);
	};

	// --- Collect tire samples ---
	float MinFx = FLT_MAX, MaxFx = FLT_MIN;
	float MinFy = FLT_MAX, MaxFy = FLT_MIN;
	float MinMz = FLT_MAX, MaxMz = FLT_MIN;
	struct Sample
	{
		float Kappa, Alpha;
		float Fx, Fy, Mz;
	};
	TArray<FPacejkaResult> Samples;
	Samples.Reserve(201);
	for(int j = -100; j <= 100; j+=1)
	{

		float kappa = j/100.f;
		float alpha = kappa * PI /2.f;
		FPacejkaResult resultx = Asset->ComputeTireForcesOverriden(2637, kappa, CurrentSlipPoint.Y,
			CurrentSlipPoint.Z, 10, 0,
			10, 0);
		FPacejkaResult resulty = Asset->ComputeTireForcesOverriden(2637, CurrentSlipPoint.X, alpha,
	CurrentSlipPoint.Z, 10, 0,
	10, 0);
		resulty.Fx = resultx.Fx;
		resulty.Fx0 = resultx.Fx0;
		resulty.Kappa = resultx.Kappa;
		resulty.Ux = resultx.Ux;
		resulty.US_Vx = resultx.US_Vx;
		MinFx = FMath::Min(MinFx, resulty.Ux); MaxFx = FMath::Max(MaxFx, resulty.Ux);
		MinFy = FMath::Min(MinFy, resulty.Uy); MaxFy = FMath::Max(MaxFy, resulty.Uy);
		MinMz = FMath::Min(MinMz, resulty.Mz_normalized); MaxMz = FMath::Max(MaxMz, resulty.Mz_normalized);
		
		//Samples.Add({kappa, alpha, Fx, Fy, Mz});
		Samples.Add(resulty);
	}

	// Fallbacks
	if (MinFx >= MaxFx) { MinFx = -1; MaxFx = 1; }
	if (MinFy >= MaxFy) { MinFy = -1; MaxFy = 1; }
	if (MinMz >= MaxMz) { MinMz = -1; MaxMz = 1; }
	
	// --- Build curves ---
	TArray<FVector2D> FxPoints, FyPoints, MzPoints;
	for (const FPacejkaResult& S : Samples)
	{
		//float X = MapX(S.Kappa, -1.f, 1.f);
		//FxPoints.Add(FVector2D(X, MapY(S.Fx, MinFx, MaxFx)));
		//FyPoints.Add(FVector2D(X, MapY(S.Fy, MinFy, MaxFy)));
		//MzPoints.Add(FVector2D(X, MapY(S.Mz, MinMz, MaxMz)));
		FxPoints.Add(WorldToScreen(FVector2D(S.Kappa, S.Ux), Size));
		FyPoints.Add(WorldToScreen(FVector2D(S.Alpha, S.Uy), Size));
		MzPoints.Add(WorldToScreen(FVector2D(S.Alpha, S.Mz_normalized), Size));
	}

	// --- Draw Curves (Fx, Fy, Mz) ---
	auto DrawCurve = [&](const TArray<FVector2D>& Points, FLinearColor Color)
	{
		if (Points.Num() > 1)
		{
			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId + 1,
				AllottedGeometry.ToPaintGeometry(),
				Points,
				ESlateDrawEffect::None,
				Color,
				true,
				2.0f
			);
		}
	};
	
	DrawCurve(FxPoints, FLinearColor(1.f,0.f,0.4f,1.f));
	DrawCurve(FyPoints, FLinearColor(1.f,0.8f,0.f,1.f));
	DrawCurve(MzPoints, FLinearColor(0.f,0.8f,0.6f,1.f));

	// --- Draw Runtime Data Point ---
    if (CurrentSlipPoint != FVector::ZeroVector)
    {
    	float kappa = CurrentSlipPoint.X;
    	float alpha = CurrentSlipPoint.Y;
    	FPacejkaResult result = Asset->ComputeTireForcesOverriden(2637, kappa, alpha,
			CurrentSlipPoint.Z, 10, 0,
			10, 0.0167f);
        // Vertical line for slip ratio (kappa) on Fx curve
        float KappaX = WorldToScreen(FVector2D(CurrentSlipPoint.X, 0.f), Size).X;
        KappaX = FMath::Clamp(KappaX, Margin, Size.X - Margin);
        TArray<FVector2D> KappaLine = {
            FVector2D(KappaX, Margin),
            FVector2D(KappaX, Size.Y - Margin)
        };
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId + 2,
            AllottedGeometry.ToPaintGeometry(),
            KappaLine,
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.f, 0.4f, 1.f), // Semi-transparent matching Fx color
            true,
            1.0f);

        // Vertical line for slip angle (alpha) on Fy and Mz curves
        float AlphaX = WorldToScreen(FVector2D(CurrentSlipPoint.Y, 0.f), Size).X;
        AlphaX = FMath::Clamp(AlphaX, Margin, Size.X - Margin);
        TArray<FVector2D> AlphaLine = {
            FVector2D(AlphaX, Margin),
            FVector2D(AlphaX, Size.Y - Margin)
        };
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId + 2,
            AllottedGeometry.ToPaintGeometry(),
            AlphaLine,
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.8f, 0.f, 1.f), // Semi-transparent matching Fy color
            true,
            1.0f);

        // Circle for Fx at (kappa, Fx)
        FVector2D FxPointScreen = WorldToScreen(FVector2D(CurrentSlipPoint.X, result.Ux), Size);
        FxPointScreen.X = FMath::Clamp(FxPointScreen.X, EdgeMargin, Size.X - EdgeMargin);
        FxPointScreen.Y = FMath::Clamp(FxPointScreen.Y, EdgeMargin, Size.Y - EdgeMargin);
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId + 3,
            AllottedGeometry.ToPaintGeometry(FxPointScreen - FVector2D(4.f, 4.f), FVector2D(8.f, 8.f)),
            FCoreStyle::Get().GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.f, 0.4f, 1.f) // Fx color
        );
    	TArray<FVector2D> FxHLine = {
    		FVector2D(EdgeMargin, FxPointScreen.Y),
			FVector2D(Size.X - EdgeMargin, FxPointScreen.Y)
		};
    	FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 2,
			AllottedGeometry.ToPaintGeometry(),
			FxHLine,
			ESlateDrawEffect::None,
			FLinearColor(1.f, 0.f, 0.4f, 1.f),
			true,
			1.0f
		);

        // Text for Fx value near the circle
        FString FxLabel = FString::Printf(TEXT("Fx: %.2f"), result.Ux);
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 4,
            AllottedGeometry.ToPaintGeometry(FxPointScreen + FVector2D(10.f, 0.f), FVector2D(1.f, 1.f)),
            FText::FromString(FxLabel),
            FCoreStyle::GetDefaultFontStyle("Regular", 10.f),
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.f, 0.4f, 1.f)
        );

        // Circle for Fy at (alpha, Fy)
        FVector2D FyPointScreen = WorldToScreen(FVector2D(CurrentSlipPoint.Y, result.Uy), Size);
        FyPointScreen.X = FMath::Clamp(FyPointScreen.X, EdgeMargin, Size.X - EdgeMargin);
        FyPointScreen.Y = FMath::Clamp(FyPointScreen.Y, EdgeMargin, Size.Y - EdgeMargin);
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId + 3,
            AllottedGeometry.ToPaintGeometry(FyPointScreen - FVector2D(4.f, 4.f), FVector2D(8.f, 8.f)),
            FCoreStyle::Get().GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.8f, 0.f, 1.f) // Fy color
        );
    	TArray<FVector2D> FyHLine = {
    		FVector2D(EdgeMargin, FyPointScreen.Y),
			FVector2D(Size.X - EdgeMargin, FyPointScreen.Y)
		};
    	FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 2,
			AllottedGeometry.ToPaintGeometry(),
			FyHLine,
			ESlateDrawEffect::None,
			FLinearColor(1.f, 0.8f, 0.f, 1.f),
			true,
			1.0f
		);

        // Text for Fy value
        FString FyLabel = FString::Printf(TEXT("Fy: %.2f"), result.Uy);
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 4,
            AllottedGeometry.ToPaintGeometry(FyPointScreen + FVector2D(10.f, 0.f), FVector2D(1.f, 1.f)),
            FText::FromString(FyLabel),
            FCoreStyle::GetDefaultFontStyle("Regular", 10.f),
            ESlateDrawEffect::None,
            FLinearColor(1.f, 0.8f, 0.f, 1.f)
        );

        // Circle for Mz at (alpha, Mz)
        FVector2D MzPointScreen = WorldToScreen(FVector2D(CurrentSlipPoint.Y, result.Mz_normalized), Size);
        MzPointScreen.X = FMath::Clamp(MzPointScreen.X, EdgeMargin, Size.X - EdgeMargin);
        MzPointScreen.Y = FMath::Clamp(MzPointScreen.Y, EdgeMargin, Size.Y - EdgeMargin);
        FSlateDrawElement::MakeBox(
            OutDrawElements,
            LayerId + 3,
            AllottedGeometry.ToPaintGeometry(MzPointScreen - FVector2D(4.f, 4.f), FVector2D(8.f, 8.f)),
            FCoreStyle::Get().GetBrush("WhiteBrush"),
            ESlateDrawEffect::None,
            FLinearColor(0.f, 0.8f, 0.6f, 1.f) // Mz color
        );
    	TArray<FVector2D> MzHLine = {
    		FVector2D(EdgeMargin, MzPointScreen.Y),
			FVector2D(Size.X - EdgeMargin, MzPointScreen.Y)
		};
    	FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 2,
			AllottedGeometry.ToPaintGeometry(),
			MzHLine,
			ESlateDrawEffect::None,
			FLinearColor(0.f, 0.8f, 0.6f, 1.f),
			true,
			1.0f
		);

        // Text for Mz value
        FString MzLabel = FString::Printf(TEXT("Mz: %.2f"), result.Mz_normalized);
        FSlateDrawElement::MakeText(
            OutDrawElements,
            LayerId + 4,
            AllottedGeometry.ToPaintGeometry(MzPointScreen + FVector2D(10.f, 0.f), FVector2D(1.f, 1.f)),
            FText::FromString(MzLabel),
            FCoreStyle::GetDefaultFontStyle("Regular", 10.f),
            ESlateDrawEffect::None,
            FLinearColor(0.f, 0.8f, 0.6f, 1.f)
        );
    }

	return LayerId + 4;
}

FReply STireGraphWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsInteractive && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsPanning = true;
		LastMousePos = MouseEvent.GetScreenSpacePosition();
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}
	return FReply::Unhandled();
}

FReply STireGraphWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsInteractive && MouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsPanning)
	{
		bIsPanning = false;
		return FReply::Handled().ReleaseMouseCapture();
	}
	return FReply::Unhandled();
}

FReply STireGraphWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (bIsInteractive && bIsPanning)
	{
		FVector2D Delta = MouseEvent.GetScreenSpacePosition() - LastMousePos;
		PanOffset += FVector2D(Delta.X / Zoom, -Delta.Y / Zoom); // move world offset
		LastMousePos = MouseEvent.GetScreenSpacePosition();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply STireGraphWidget::OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(!bIsInteractive)
		return FReply::Unhandled();
	float Scroll = MouseEvent.GetWheelDelta();
	Zoom = FMath::Clamp(Zoom * (1.f + Scroll * 0.1f), 10.f, 10000.f);

	return FReply::Handled();
}

void STireGraphWidget::UpdateRuntimeData(float Kappa, float Alpha, float Fx, float Fy, float Mz)
{
}
