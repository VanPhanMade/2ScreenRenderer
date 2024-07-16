// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RenderPawn.generated.h"

UCLASS()
class MULTIPLECAMERAS_API ARenderPawn : public APawn
{
	GENERATED_BODY()

public:
	ARenderPawn();

protected:
	/** APawn */
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	/** Inputs */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftCycleUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftCycleDownAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightCycleUpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightCycleDownAction;

	/** Callback functions */
	void LookActionCall(const struct FInputActionValue &Value);
	void MoveActionCall(const struct FInputActionValue &Value);
	void LeftCycleUpActionCall();
	void LeftCycleDownActionCall();
	void RightCycleUpActionCall();
	void RightCycleDownActionCall();

	/** Components */
	UPROPERTY(VisibleAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* RightRenderer;
	UPROPERTY(VisibleAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* LeftRenderer;

	UPROPERTY(VisibleAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* RightCameraBoom;
	UPROPERTY(VisibleAnywhere, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* LeftCameraBoom;

	/** Data Fields */
	UPROPERTY(EditAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	TArray<struct FPostProcessSettings> LeftPostProcessingSettings;
	UPROPERTY(VisibleAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	float CurrentLeftIndex = 0;

	UPROPERTY(EditAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	TArray<struct FPostProcessSettings> RightPostProcessingSettings;

	UPROPERTY(VisibleAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	float CurrentRightIndex = 0;

	UPROPERTY(VisibleAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	FVector MovementInput; 

	UPROPERTY(EditAnywhere, Category="Variables", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;
};
