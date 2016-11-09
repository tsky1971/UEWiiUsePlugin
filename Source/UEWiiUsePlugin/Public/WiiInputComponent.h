// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// @third party code - BEGIN
#include "AllowWindowsPlatformTypes.h"
#include "wiiuse.h"
#include "HideWindowsPlatformTypes.h"
// @third party code - END

#include "Components/InputComponent.h"
#include "WiiInputComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UEWIIUSEPLUGIN_API UWiiInputComponent : public UInputComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void OnComponentCreated() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

};
