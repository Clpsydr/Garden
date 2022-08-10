#pragma once
#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "FloatyBoard.generated.h"

class UBuoyancyComponent;

UCLASS()
class GARDENTD_API AFloatyBoard : public APlayerCharacter 
{
	GENERATED_BODY()

public:
	AFloatyBoard();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
		float BoatTurningSpeed = 25.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Parameters")
		float BoatVelocity = 1500.f;

protected:
	UPROPERTY(VisibleDefaultsOnly)
		UStaticMeshComponent* FloatingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (Category = "Default",
		OverrideNativeName = "Buoyancy"))
		UBuoyancyComponent* Buoyancy;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SinkMode();
	
	UFUNCTION(BlueprintCallable)
	void FloatMode();

	UFUNCTION(BlueprintCallable)
	void DriveMode(ACharacter* PreviousChar);

	void MoveSide(float AxisValue) override;

	void MoveFront(float AxisValue) override;
};
