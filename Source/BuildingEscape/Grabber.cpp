// no touch!

#include "BuildingEscape.h"
#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	BindInputComponent();	
}

void UGrabber::BindInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		///  bind input
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handel component"), *GetOwner()->GetName())
	}
}

 /// look for attached physics handle
void UGrabber::FindPhysicsComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handel component"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Error, TEXT("Grab Key pressed "))

	// try and grab actor with physics
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto componentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	// if hit then attach physics handle

	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			componentToGrab,
			NAME_None,
			componentToGrab->GetOwner()->GetActorLocation(),
			true // allow rotation
		);
	}


}

void UGrabber::Release() {
	PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach() {

	/// set up query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// ray-cast out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// see what we hit
	AActor* ActorHit = Hit.GetActor();

	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("hit %s"), *(ActorHit->GetName()))
	}

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}


FVector UGrabber::GetReachLineStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/// draw redline to visualize
	return PlayerViewPointLocation ;
}


FVector UGrabber::GetReachLineEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	/// draw redline to visualize
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

