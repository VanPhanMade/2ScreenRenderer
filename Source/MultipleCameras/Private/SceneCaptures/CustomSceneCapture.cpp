// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneCaptures/CustomSceneCapture.h"
#include "Components/SceneCaptureComponent2D.h"

ACustomSceneCapture::ACustomSceneCapture()
{
    // https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Components/USceneCaptureComponent2D/
    GetCaptureComponent2D()->bAlwaysPersistRenderingState = true;
}