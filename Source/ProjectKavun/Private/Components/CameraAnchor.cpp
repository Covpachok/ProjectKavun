#include "Components/CameraAnchor.h"

#include "Components/BillboardComponent.h"

UCameraAnchor::UCameraAnchor()
{
	PrimaryComponentTick.bCanEverTick = false;

	#if  WITH_EDITORONLY_DATA
	bVisualizeComponent = true;
	#endif
}

void UCameraAnchor::OnRegister()
{
	Super::OnRegister();

	#if  WITH_EDITORONLY_DATA
	if ( SpriteComponent )
	{
		SpriteComponent->SetSprite(LoadObject<UTexture2D>(
				nullptr, TEXT("/Game/Assets/Textures/T_CameraAnchor.T_CameraAnchor")));
	}
	#endif
}
