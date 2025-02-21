// no touch!

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void BindInputComponent();

	void FindPhysicsComponent();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FVector GetReachLineEnd();
	FVector GetReachLineStart();

private:
	//how far in front can  i grab
	UPROPERTY(EditAnywhere)
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
	
	// RayCast and grab what is in reach;
	void Grab();

	// release object that has been grabbed
	void Release();

	const FHitResult GetFirstPhysicsBodyInReach();
	
};
