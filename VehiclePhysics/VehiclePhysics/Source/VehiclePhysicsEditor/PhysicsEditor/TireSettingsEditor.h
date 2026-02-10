#pragma once
#include "CoreMinimal.h"
// ... other includes

#include "TireSettingsEditor.generated.h" // <-- MUST be last include
UCLASS()
class UTireEditorSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Editor Preview")
	float SlipRatio = 0.f;

	UPROPERTY(EditAnywhere, Category="Editor Preview")
	float SlipAngle = 0.f;

	UPROPERTY(EditAnywhere, Category="Editor Preview")
	float CamberAngle = 0.f;
};