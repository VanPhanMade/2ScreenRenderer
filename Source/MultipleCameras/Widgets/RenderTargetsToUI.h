// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RenderTargetsToUI.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLECAMERAS_API URenderTargetsToUI : public UUserWidget
{
	GENERATED_BODY()

public: 
 
protected: 
 
private:
	UPROPERTY(meta=(BindWidget))
	class UImage* LeftRender;

	UPROPERTY(meta=(BindWidget))
	class UImage* RightRender;
};
