#include "CarDebugHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"

ACarDebugHUD::ACarDebugHUD()
{
	DebugHistory1.MaxSamples = 200;  // Adjust for the number of data points
	DebugHistory2.MaxSamples = 100;
}

void ACarDebugHUD::AddDebugValue(float Value, int32 index, FString Label)
{
	switch (index)
	{
		case 0:DebugHistory1.AddSample(Value);
		DebugString1 = Label; break;
		case 1:DebugHistory2.AddSample(Value);
		DebugString2 = Label; break;
		case 2:DebugHistory3.AddSample(Value);
		DebugString3 = Label; break;
	default: break;
	}
}

void ACarDebugHUD::DrawHUD()
{
	Super::DrawHUD();

    if (!Canvas)
    {
        return;
    }
    
    // --- Setup the graph area parameters ---
	FVector2D View = FVector2D::ZeroVector;
	GEngine->GameViewport->GetViewportSize(View);
    float GraphX = View.X * 0.01f;    // Left margin
    float GraphY = View.Y * 0.01f;   // Top margin
    float GraphWidth = View.X/3.5f;
    float GraphHeight = View.Y/3.5f;
	
	DrawGraph(GraphX, GraphY, GraphWidth, GraphHeight, DebugHistory1, DebugString1);
	DrawGraph(GraphX, GraphY + GraphHeight, GraphWidth, GraphHeight, DebugHistory2, DebugString2);
	DrawGraph(GraphX, GraphY + GraphHeight * 2, GraphWidth, GraphHeight, DebugHistory3, DebugString3);

}

void ACarDebugHUD::DrawGraph(float GraphX, float GraphY, float GraphWidth, float GraphHeight,
	FDebugFloatHistory DebugHistory, FString Label)
{

	// --- Draw the Background ---
	{
		// A semi-transparent black background to make the graph stand out.
		FLinearColor BackgroundColor(0.f, 0.f, 0.f, 0.8f);
		FCanvasTileItem BackgroundTile(FVector2D(GraphX, GraphY), FVector2D(GraphWidth, GraphHeight), BackgroundColor);
		BackgroundTile.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(BackgroundTile);
	}

	// --- Draw the Border ---
	{
		FLinearColor BorderColor = FLinearColor::White;
		float BorderThickness = 2.0f;

		// Top border
		Canvas->K2_DrawLine(FVector2D(GraphX, GraphY),
		                    FVector2D(GraphX + GraphWidth, GraphY),
		                    BorderThickness, BorderColor);
		// Bottom border
		Canvas->K2_DrawLine(FVector2D(GraphX, GraphY + GraphHeight),
		                    FVector2D(GraphX + GraphWidth, GraphY + GraphHeight),
		                    BorderThickness ,BorderColor);
		// Left border
		Canvas->K2_DrawLine(FVector2D(GraphX, GraphY),
		                    FVector2D(GraphX, GraphY + GraphHeight),
		                    BorderThickness, BorderColor);
		// Right border
		Canvas->K2_DrawLine(FVector2D(GraphX + GraphWidth, GraphY),
		                    FVector2D(GraphX + GraphWidth, GraphY + GraphHeight),
		                    BorderThickness, BorderColor);
	}

	// --- Draw the Zero Value Line ---
	{
		// Calculate the Y position that corresponds to the value zero.
		// The graph maps values so that:
		// Y = GraphY + GraphHeight - ((Value - MinValue) / (MaxValue - MinValue)) * GraphHeight
		float ZeroValue = 0.f;
		float YZero = GraphY + GraphHeight - ((ZeroValue - DebugHistory.MinValue) / (DebugHistory.MaxValue - DebugHistory.MinValue)) * GraphHeight;

		// Draw a yellow horizontal line at the zero level.
		Canvas->K2_DrawLine(FVector2D(GraphX, YZero),
		                    FVector2D(GraphX + GraphWidth, YZero),
		                    1.0f, FLinearColor::Yellow);
	}
	// --- Draw the Middle Value Line ---
	{
		// Calculate the Y position that corresponds to the value zero.
		// The graph maps values so that:
		// Y = GraphY + GraphHeight - ((Value - MinValue) / (MaxValue - MinValue)) * GraphHeight

		// Draw a yellow horizontal line at the zero level.
		Canvas->K2_DrawLine(FVector2D(GraphX + GraphWidth / 2.f, GraphY),
							FVector2D(GraphX + GraphWidth / 2.f, GraphY + GraphHeight),
							1.0f, FLinearColor::Yellow);
	}

	// --- Draw the Graph ---
	if (DebugHistory.GetNumSamples() < 2)
	{
		return; // Not enough data to draw a graph
	}


	// Retrieve the samples.
	TArray<float> Samples = DebugHistory.GetSamples();
	int32 NumSamples = Samples.Num();
	FString Text = FString::Printf(TEXT("%.6f"), Samples[NumSamples/2]);
	Text = FString::Printf(TEXT("%s %.6f"), *Label, Samples[0]);
	FFontRenderInfo FontInfo;
	Canvas->K2_DrawText(
		GEngine->GetLargeFont(),
		Text,
		FVector2D(GraphX, GraphY),
		FVector2D(3.f,3.f), FColor::Yellow);

	// For scaling the graph, use the pre-defined min/max values.
	float MinVal = DebugHistory.MinValue;
	float MaxVal = DebugHistory.MaxValue;

	// Draw the lines connecting the samples.
	for (int32 i = 1; i < NumSamples; i++)
	{
		float X1 = GraphX + ((i - 1) * (GraphWidth / (float)NumSamples));
		float Y1 = GraphY + GraphHeight - ((Samples[i - 1] - MinVal) / (MaxVal - MinVal) * GraphHeight);
        
		float X2 = GraphX + (i * (GraphWidth / (float)NumSamples));
		float Y2 = GraphY + GraphHeight - ((Samples[i] - MinVal) / (MaxVal - MinVal) * GraphHeight);

		FCanvasLineItem LineItem(FVector2D(X1, Y1), FVector2D(X2, Y2));
		LineItem.LineThickness = 4.f;
		LineItem.SetColor(FLinearColor::Red);
		Canvas->DrawItem(LineItem);
	}
}