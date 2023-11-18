

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "CameraAnchor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTKAVUN_API UCameraAnchor : public USceneComponent
{
	GENERATED_BODY()

public:	
	UCameraAnchor();
	
	virtual void OnRegister() override;
};
