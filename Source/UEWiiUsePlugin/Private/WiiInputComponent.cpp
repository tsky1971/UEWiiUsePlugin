// Fill out your copyright notice in the Description page of Project Settings.

#include "UEWiiUsePluginPrivatePCH.h"
#include "WiiInputComponent.h"

// @third party code - BEGIN
#include "AllowWindowsPlatformTypes.h"
#include "wiiuse.h"
#include "HideWindowsPlatformTypes.h"
// @third party code - END


void UWiiInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWiiInputComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UWiiInputComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWiiInputComponent::OnComponentCreated()
{
	//BindAction("WiiButtonA", EInputEvent::IE_Pressed, this, )
}

void UWiiInputComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	
}
