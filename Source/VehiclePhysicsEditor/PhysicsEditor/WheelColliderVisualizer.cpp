

#include "WheelColliderVisualizer.h"

#include "Components/SphereComponent.h"
#include "VehiclePhysics/Vehicles/Physics/WheelCollider.h"

void FWheelColliderVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                 FPrimitiveDrawInterface* PDI)
{
	FComponentVisualizer::DrawVisualization(Component, View, PDI);
	const UWheelCollider* WheelCollider = Cast<UWheelCollider>(Component);
	if (WheelCollider)
	{
		
		const FVector SuspensionOrigin = WheelCollider->GetComponentLocation();
		
		FVector ForwardDirection = WheelCollider->GetForwardVector();
		FVector UpDirection = WheelCollider->GetUpVector();
		const FVector CircleLocation = SuspensionOrigin - WheelCollider->GetRestLenght() * WheelCollider->GetUpVector();
		
		DrawCircle(PDI, CircleLocation,
			ForwardDirection,
			UpDirection,
			FColor::Green,
			WheelCollider->GetWheelRadius(),
			64,
			SDPG_Foreground);

		const FVector SuspensionEnd = SuspensionOrigin + -WheelCollider->GetUpVector() * (WheelCollider->GetWheelRadius() + WheelCollider->GetRestLenght());
		PDI->DrawLine(SuspensionOrigin,
			SuspensionEnd,
				FColor::Green,
				SDPG_Foreground);
	}
}