#pragma once
#include "VehiclePhysics/Vehicles/Physics/TireParamsDataAsset.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/SOverlay.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"

class VEHICLEPHYSICS_API STireGraphWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STireGraphWidget) : _bIsInteractive(true) {}
		SLATE_ARGUMENT(TWeakObjectPtr<UTireParamsDataAsset>, Asset)
		SLATE_ARGUMENT(bool, bIsInteractive) // New argument to control interactivity
SLATE_END_ARGS()

void Construct(const FArguments& InArgs);
	void DrawBackground(const FGeometry& AllottedGeometry, FSlateWindowElementList& OutDrawElements,
	                    int32 LayerId) const;

	int32 OnPaint(const FPaintArgs& Args,
	              const FGeometry& AllottedGeometry,
	              const FSlateRect& MyCullingRect,
	              FSlateWindowElementList& OutDrawElements,
	              int32 LayerId,
	              const FWidgetStyle& InWidgetStyle,
	              bool bParentEnabled) const override;

	FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	void UpdateRuntimeData(float Kappa, float Alpha, float Fx, float Fy, float Mz);

	FVector CurrentSlipPoint = FVector(0.f, 0.f, 0.f);
	float Zoom = 150.f;

private:
	TWeakObjectPtr<UTireParamsDataAsset> Asset;
	bool bIsInteractive = true; // Controls whether panning/zooming is allowed

	//Viewport state
	FVector2D PanOffset = FVector2D::ZeroVector;

	//Interaction
	bool bIsPanning = false;
	FVector2D LastMousePos;

	// Runtime data
	float CurrentFx = 0.f;
	float CurrentFy = 0.f;
	float CurrentMz = 0.f;

	// World → Screen
	FVector2D WorldToScreen(FVector2D World, const FVector2D& Size) const
	{
		return FVector2D(
			Size.X * 0.5f + (World.X + PanOffset.X) * Zoom,
			Size.Y * 0.5f - (World.Y + PanOffset.Y) * Zoom // flip Y
		);
	}

	// Screen → World
	FVector2D ScreenToWorld(FVector2D Screen, const FVector2D& Size) const
	{
		return FVector2D(
			(Screen.X - Size.X * 0.5f) / Zoom - PanOffset.X,
			-(Screen.Y - Size.Y * 0.5f) / Zoom - PanOffset.Y
		);
	}
	
};