/*
*	UEWiiUsePlugin
*
*	Written By:
*		Thomas Kollakowsky
*		Email: < thomas.kollakowsky (--AT--) g m a i l [--DOT--] com >
*
*	Copyright 2016
*
*	This file is part of UEWiiUsePlugin.
*
*	This program is free software; you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation; either version 3 of the License, or
*	(at your option) any later version.
*
*	This program is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*	See License File
*
*/

#pragma once

// @third party code - BEGIN
#include "AllowWindowsPlatformTypes.h"
#include "wiiuse.h"
#include "HideWindowsPlatformTypes.h"
// @third party code - END

#include "Components/ActorComponent.h"
#include "WiiUseController.generated.h"


#define MAX_WIIMOTES 4

USTRUCT(BlueprintType, Blueprintable)
struct FWiiControllerData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	int32 ControllerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	int32 UsingSpeaker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	int32 UsingIR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool LED1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool LED2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool LED3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool LED4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	float BatteryLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_A;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_B;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_UP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_DOWN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_LEFT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_RIGHT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_MINUS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_PLUS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_ONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_TWO;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	bool Button_HOME;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector2D IR_Point1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector2D IR_Point2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector2D IR_Point3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector2D IR_Point4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector2D IR_Cursor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	float IR_CursorZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector Acc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WiiUse)
	FVector AccAngle;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UEWIIUSEPLUGIN_API UWiiUseController : public UActorComponent
{
	GENERATED_BODY()


	//////////////////////////////////////////////////////////////////////////
	// Public vars
	//////////////////////////////////////////////////////////////////////////
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WiiUse)
	TArray<FWiiControllerData> WiiController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WiiUse)
	bool bIsConnected;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WiiUse)
	int32 WiimotesFound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WiiUse)
	bool WiiUseUsingIR;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = WiiUse)
	bool WiiUseUsingACC;

	//////////////////////////////////////////////////////////////////////////
	// Public functions
	//////////////////////////////////////////////////////////////////////////
public:
	// Sets default values for this component's properties
	UWiiUseController();
	virtual ~UWiiUseController();

	UFUNCTION(BluePrintCallable, Category = WiiUse)
	bool WiimoteTest();

	UFUNCTION(BluePrintCallable, Category = WiiUse)
	bool WiimoteTestRumpleOn(int32 wiimote);

	UFUNCTION(BluePrintCallable, Category = WiiUse)
	bool WiimoteTestRumpleOff(int32 wiimote);

	UFUNCTION(BluePrintCallable, Category = WiiUse)
	int32 WiimoteAnyConnected(int32 maxwiimotes);

	UFUNCTION(BluePrintCallable, Category = WiiUse)
	bool WiimoteIsButtonAPressed(int32 wiimote);
	UFUNCTION(BluePrintCallable, Category = WiiUse)
	bool WiimoteIsButtonAReleased(int32 wiimote);

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//////////////////////////////////////////////////////////////////////////
	// Protected functions
	//////////////////////////////////////////////////////////////////////////
protected:
	bool WiimoteInit();
	int32 WiimoteFind();
	int32 WiimoteConnect();
	int32 WiimotesPoll(int32 maxwiimotes);

	void handle_event(int32 i);
	void handle_ctrl_status(int32 i);
	void handle_disconnect(int32 i);

	FString ToString(int32 i) const;

protected:
	

	wiimote** wiimotes;	
};
