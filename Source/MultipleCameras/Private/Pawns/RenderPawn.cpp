// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/RenderPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ARenderPawn::ARenderPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent) RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	RightCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("RightCameraBoom"));
	RightCameraBoom->SetupAttachment(RootComponent);
	RightCameraBoom->TargetArmLength = 0.0f;
	RightCameraBoom->bUsePawnControlRotation = true;

	LeftCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("LeftCameraBoom"));
	LeftCameraBoom->SetupAttachment(RootComponent);
	LeftCameraBoom->TargetArmLength = 0.0f;
	LeftCameraBoom->bUsePawnControlRotation = true;

	RightRenderer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("RightRenderer"));
    RightRenderer->SetupAttachment(RightCameraBoom);
	RightRenderer->CaptureSource = ESceneCaptureSource::SCS_FinalColorHDR;

	LeftRenderer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("LeftRenderer"));
    LeftRenderer->SetupAttachment(LeftCameraBoom);
	LeftRenderer->CaptureSource = ESceneCaptureSource::SCS_FinalColorHDR;
}

void ARenderPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			UE_LOG(LogTemp, Display, TEXT("Subsystem found, adding mapping context"));
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystem not found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not found"));
	}

	LeftRenderer->PostProcessSettings = LeftPostProcessingSettings.Num() == 0 ? LeftRenderer->PostProcessSettings : LeftPostProcessingSettings[0];
	RightRenderer->PostProcessSettings = RightPostProcessingSettings.Num() == 0 ? RightRenderer->PostProcessSettings : RightPostProcessingSettings[0];

}

void ARenderPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/GameFramework/APawn/
	 if (Controller != nullptr && !MovementInput.IsZero())
    {
		
		const FVector Forward = RightRenderer->GetForwardVector() * MovementInput.Y * MovementSpeed;
		const FVector Right = LeftRenderer->GetRightVector() * MovementInput.X * MovementSpeed;
		const FVector NewLocation = GetActorLocation() + Forward + Right;
        SetActorLocation(NewLocation);

        // Reset the movement input after applying it
        MovementInput = FVector::ZeroVector;
    }

}

void ARenderPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
		UE_LOG(LogTemp, Display, TEXT("Binding input actions..."));
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::LookActionCall);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::MoveActionCall);
		EnhancedInputComponent->BindAction(LeftCycleUpAction, ETriggerEvent::Triggered, this, &ThisClass::LeftCycleUpActionCall);
		EnhancedInputComponent->BindAction(LeftCycleDownAction, ETriggerEvent::Triggered, this, &ThisClass::LeftCycleDownActionCall);
		EnhancedInputComponent->BindAction(RightCycleUpAction, ETriggerEvent::Triggered, this, &ThisClass::RightCycleUpActionCall);
		EnhancedInputComponent->BindAction(RightCycleDownAction, ETriggerEvent::Triggered, this, &ThisClass::RightCycleDownActionCall);
    }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast PlayerInputComponent to UEnhancedInputComponent."));
	}

}

void ARenderPawn::LookActionCall(const FInputActionValue &Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(-LookVector.Y);
	}
	UE_LOG(LogTemp, Display, TEXT("LookAction triggered with value: %s"), *LookVector.ToString());
}

void ARenderPawn::MoveActionCall(const FInputActionValue &Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		MovementInput.X += MovementVector.X;
		MovementInput.Y += MovementVector.Y;
	}
	UE_LOG(LogTemp, Display, TEXT("MoveAction triggered with value: %s"), *MovementVector.ToString());
}

void ARenderPawn::LeftCycleUpActionCall()
{
	if(LeftPostProcessingSettings.Num() == 0) return;
	CurrentLeftIndex = CurrentLeftIndex + 1 > LeftPostProcessingSettings.Num() - 1 ? 0 : CurrentLeftIndex + 1;
	LeftRenderer->PostProcessSettings = LeftPostProcessingSettings[CurrentLeftIndex];
	UE_LOG(LogTemp, Display, TEXT("LeftCycleUpAction triggered. Current index: %f"), CurrentLeftIndex);

}

void ARenderPawn::LeftCycleDownActionCall()
{
	if(LeftPostProcessingSettings.Num() == 0) return;
	CurrentLeftIndex = CurrentLeftIndex - 1 < 0 ? LeftPostProcessingSettings.Num() - 1 : CurrentLeftIndex - 1;
	LeftRenderer->PostProcessSettings = LeftPostProcessingSettings[CurrentLeftIndex];
	UE_LOG(LogTemp, Display, TEXT("LeftCycleDownAction triggered. Current index: %f"), CurrentLeftIndex);
}

void ARenderPawn::RightCycleUpActionCall()
{
	if(RightPostProcessingSettings.Num() == 0) return;
	CurrentRightIndex = CurrentRightIndex + 1 > RightPostProcessingSettings.Num() - 1 ? 0 : CurrentRightIndex + 1;
	RightRenderer->PostProcessSettings = RightPostProcessingSettings[CurrentRightIndex];
	UE_LOG(LogTemp, Display, TEXT("RightCycleUpAction triggered. Current index: %f"), CurrentRightIndex);
}

void ARenderPawn::RightCycleDownActionCall()
{
	if(RightPostProcessingSettings.Num() == 0) return;
	CurrentRightIndex = CurrentRightIndex - 1 < 0 ? RightPostProcessingSettings.Num() - 1 : CurrentRightIndex - 1;
	RightRenderer->PostProcessSettings = RightPostProcessingSettings[CurrentRightIndex];
	UE_LOG(LogTemp, Display, TEXT("RightCycleDownAction triggered. Current index: %f"), CurrentRightIndex);
}
