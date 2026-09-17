#pragma once

#include "ComponentVisualizer.h"

struct FWishbonePose;

/**
 * Draws a UWishboneSuspension corner in the editor viewport.
 *
 * Everything comes from UWishboneSuspension::GetPose, never from the component's
 * private kinematics, so the viewport cannot disagree with what the physics will
 * do at the same angle. That matters here more than usual: a component visualizer
 * runs with no physics at all, so the drawing IS the only feedback a hardpoint
 * edit gives you.
 *
 * Note this is a separate implementation from the runtime debug draw
 * (WishboneSuspensionDebug.cpp) rather than a shared one, because the two write
 * to different sinks - a FPrimitiveDrawInterface here, the world line batcher
 * there - and draw different things. In the editor there are no forces, no
 * contact patch and no readout worth showing; there is only geometry. The palette
 * IS shared, via WishbonePalette.
 */
class FWishboneSuspensionVisualizer : public FComponentVisualizer
{
private:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View,
		FPrimitiveDrawInterface* PDI) override;

	/** One pose, at a given brightness and line weight. */
	static void DrawPose(const FWishbonePose& Pose, FPrimitiveDrawInterface* PDI,
		float Brightness, float Thickness, bool bDrawWheel);
};
