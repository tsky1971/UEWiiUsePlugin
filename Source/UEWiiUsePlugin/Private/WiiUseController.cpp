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

#include "UEWiiUsePluginPrivatePCH.h"
#include "WiiUseController.h"

// @third party code - BEGIN
#include "AllowWindowsPlatformTypes.h"
#include "wiiuse.h"
#include "HideWindowsPlatformTypes.h"
// @third party code - END

// Sets default values for this component's properties
UWiiUseController::UWiiUseController()
{
	UE_LOG(LogWiiUse, Warning, TEXT("UWiiUseController::UWiiUseController() contructor"));

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;	
	
	bIsConnected = false;
	
	WiiController.SetNum(MAX_WIIMOTES);
	for (auto& wii : WiiController) {
		wii.BatteryLevel = 0;
		wii.Button_A = false;
		wii.Button_B = false;
		wii.Button_DOWN = false;
		wii.Button_UP = false;
		wii.Button_HOME = false;
		wii.Button_LEFT = false;
		wii.Button_RIGHT = false;
		wii.Button_ONE = false;
		wii.Button_TWO = false;
		wii.ControllerId = -1;
		wii.IR_Cursor.X = 0;
		wii.IR_Cursor.Y = 0;
		wii.IR_CursorZ = 0;
		wii.Acc.X = 0;
		wii.Acc.Y = 0;
		wii.Acc.Z = 0;
		wii.AccAngle.X = 0;
		wii.AccAngle.Y = 0;
		wii.AccAngle.Z = 0;
	}
}

UWiiUseController::~UWiiUseController()
{
	UE_LOG(LogWiiUse, Warning, TEXT("UWiiUseController::~UWiiUseController() destructor"));

	WiiController.Empty();

	wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
}

bool UWiiUseController::WiimoteInit()
{	
	bool result = false;

	wiimotes = wiiuse_init(MAX_WIIMOTES);
	if (wiimotes != NULL) {
		result = true;
	}
	return result;
}

int32 UWiiUseController::WiimoteFind()
{
	int32 result = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
	if (result <= 0) {
		UE_LOG(LogWiiUse, Warning, TEXT("UWiiUseController::WiimoteFind() FAILED no Wiimotes found"));		
	}

	WiimotesFound = result;
	return result;
}

int32 UWiiUseController::WiimoteConnect()
{
	int32 result = wiiuse_connect(wiimotes, MAX_WIIMOTES);
	if (result > 0) {
		UE_LOG(LogWiiUse, Warning, TEXT("Connected to %i wiimotes (of %i found)"), result, WiimotesFound);		
		bIsConnected = true;
	}
	else {
		UE_LOG(LogWiiUse, Warning, TEXT("Failed to connect to any wiimote."));
		result = 0;
	}

	return result;
}

bool UWiiUseController::WiimoteTest()
{
	bool result = true;
	
	wiiuse_set_leds(wiimotes[0], WIIMOTE_LED_1);
	wiiuse_set_leds(wiimotes[1], WIIMOTE_LED_2);
	wiiuse_set_leds(wiimotes[2], WIIMOTE_LED_3);
	wiiuse_set_leds(wiimotes[3], WIIMOTE_LED_4);


	return result;
}


bool UWiiUseController::WiimoteTestRumpleOn(int32 wiimote)
{
	bool result = true;

	wiiuse_rumble(wiimotes[wiimote], 1);

	return result;
}

bool UWiiUseController::WiimoteTestRumpleOff(int32 wiimote)
{
	bool result = true;
	
	wiiuse_rumble(wiimotes[wiimote], 0);

	return result;
}

int32 UWiiUseController::WiimoteAnyConnected(int32 wiimotesmax)
{
	wiimote** wm = wiimotes;
	int32 result = 0;
	int32 i;

	if (!wm) {
		return result;
	}

	for (i = 0; i < wiimotesmax; i++) {
		if (wm[i] && WIIMOTE_IS_CONNECTED(wm[i])) {
			return i;
		}
	}
	return 0;
}

bool UWiiUseController::WiimoteIsButtonAPressed(int32 wiimote)
{
	struct wiimote_t* wm = wiimotes[wiimote];

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		return true;
	}
	return false;
}

bool UWiiUseController::WiimoteIsButtonAReleased(int32 wiimote)
{
	struct wiimote_t* wm = wiimotes[wiimote];

	if (IS_RELEASED(wm, WIIMOTE_BUTTON_A)) {
		return true;
	}
	return false;
}

int32 UWiiUseController::WiimotesPoll(int32 maxwiimotes)
{
	int32 result = 0;

	if (wiiuse_poll(wiimotes, MAX_WIIMOTES)) {
		/*
		*	This happens if something happened on any wiimote.
		*	So go through each one and check if anything happened.
		*/
		int i = 0;
		for (; i < MAX_WIIMOTES; ++i) {
			
			UE_LOG(LogWiiUse, Warning, TEXT("wiimotes[%i]->event=%i"), i, (int32) (wiimotes[i]->event));

			switch (wiimotes[i]->event) {
			case WIIUSE_EVENT:
				/* a generic event occurred */
				handle_event(i);
				break;

			case WIIUSE_STATUS:
				/* a status event occurred */
				handle_ctrl_status(i);
				break;

			case WIIUSE_DISCONNECT:
			case WIIUSE_UNEXPECTED_DISCONNECT:
				/* the wiimote disconnected */
				handle_disconnect(i);
				break;

			case WIIUSE_READ_DATA:
				/*
				*	Data we requested to read was returned.
				*	Take a look at wiimotes[i]->read_req
				*	for the data.
				*/
				break;

			case WIIUSE_NUNCHUK_INSERTED:
				/*
				*	a nunchuk was inserted
				*	This is a good place to set any nunchuk specific
				*	threshold values.  By default they are the same
				*	as the wiimote.
				*/
				/* wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f); */
				/* wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100); */
				UE_LOG(LogWiiUse, Warning, TEXT("Nunchuk inserted."));
				break;

			case WIIUSE_CLASSIC_CTRL_INSERTED:
				UE_LOG(LogWiiUse, Warning, TEXT("Classic controller inserted."));
				break;

			case WIIUSE_WII_BOARD_CTRL_INSERTED:
				UE_LOG(LogWiiUse, Warning, TEXT("Balance board controller inserted."));
				break;

			case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
				/* some expansion was inserted */
				handle_ctrl_status(i);
				UE_LOG(LogWiiUse, Warning, TEXT("Guitar Hero 3 controller inserted."));
				break;

			case WIIUSE_MOTION_PLUS_ACTIVATED:
				UE_LOG(LogWiiUse, Warning, TEXT("Motion+ was activated"));
				break;

			case WIIUSE_NUNCHUK_REMOVED:
			case WIIUSE_CLASSIC_CTRL_REMOVED:
			case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
			case WIIUSE_WII_BOARD_CTRL_REMOVED:
			case WIIUSE_MOTION_PLUS_REMOVED:
				/* some expansion was removed */
				handle_ctrl_status(i);
				UE_LOG(LogWiiUse, Warning, TEXT("An expansion was removed."));
				break;

			default:
				break;
			}
		}
	}

	return result;
}

// Called when the game starts
void UWiiUseController::BeginPlay()
{
	Super::BeginPlay();

	if (WiimoteInit()) {
		UE_LOG(LogWiiUse, Warning, TEXT("Wiimotes init"));
		if (WiimoteFind()) {
			UE_LOG(LogWiiUse, Warning, TEXT("Wiimotes found"));
			if (WiimoteConnect()) {
				UE_LOG(LogWiiUse, Warning, TEXT("Wiimotes connected"));
			}
		}
	}
}

// Called every frame
void UWiiUseController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsConnected) {
		WiimotesPoll(MAX_WIIMOTES);
	}
	else {
		UE_LOG(LogWiiUse, Warning, TEXT("Wiimotes not connected"));
	}
	

}

void UWiiUseController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//throw std::logic_error("The method or operation is not implemented.");
}


void UWiiUseController::handle_event(int32 i)
{
	struct wiimote_t* wm = wiimotes[i];	

	UE_LOG(LogWiiUse, Warning, TEXT("EVENT [id %i] ---"), wm->unid);

	if (WiiController.IsValidIndex(i) == false) {
		UE_LOG(LogWiiUse, Warning, TEXT("Wii Remote %i is not valid"), i);
		return;
	}

	WiiController[i].ControllerId = wm->unid;

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_A)) {
		UE_LOG(LogWiiUse, Warning, TEXT("A pressed"));
		WiiController[i].Button_A = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_A)) {
		UE_LOG(LogWiiUse, Warning, TEXT("A released"));
		WiiController[i].Button_A = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_B)) {
		UE_LOG(LogWiiUse, Warning, TEXT("B pressed"));
		WiiController[i].Button_B = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_B)) {
		UE_LOG(LogWiiUse, Warning, TEXT("B released"));
		WiiController[i].Button_B = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
		WiiController[i].Button_UP = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_UP)) {
		WiiController[i].Button_UP = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) {
		WiiController[i].Button_DOWN = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_DOWN)) {
		WiiController[i].Button_DOWN = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT)) {
		WiiController[i].Button_LEFT = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_LEFT)) {
		WiiController[i].Button_LEFT = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT)) {
		WiiController[i].Button_RIGHT = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_RIGHT)) {
		WiiController[i].Button_RIGHT = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
		WiiController[i].Button_MINUS = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_MINUS)) {
		WiiController[i].Button_MINUS =false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
		WiiController[i].Button_PLUS = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_PLUS)) {
		WiiController[i].Button_PLUS = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
		WiiController[i].Button_ONE = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_ONE)) {
		WiiController[i].Button_ONE = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
		WiiController[i].Button_TWO = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_TWO)) {
		WiiController[i].Button_TWO = false;
	}

	if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME)) {
		WiiController[i].Button_HOME = true;
	}
	if (IS_RELEASED(wm, WIIMOTE_BUTTON_HOME)) {
		WiiController[i].Button_HOME = false;
	}

	///*
	//*	Pressing minus will tell the wiimote we are no longer interested in movement.
	//*	This is useful because it saves battery power.
	//*/
	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
	//	wiiuse_motion_sensing(wm, 0);
	//}

	///*
	//*	Pressing plus will tell the wiimote we are interested in movement.
	//*/
	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
	//	wiiuse_motion_sensing(wm, 1);
	//}

	//if (WiiUseUsingACC) {
		wiiuse_motion_sensing(wm, 1);
	//}
	//else {
	//	wiiuse_motion_sensing(wm, 0);
	//}

	///*
	//*	Pressing B will toggle the rumble
	//*
	//*	if B is pressed but is not held, toggle the rumble
	//*/
	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_B)) {
	//	wiiuse_toggle_rumble(wm);
	//}

	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
	//	wiiuse_set_ir(wm, 1);
	//}
	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_DOWN)) {
	//	wiiuse_set_ir(wm, 0);
	//}
	//if (WiiUseUsingIR) {
		wiiuse_set_ir(wm, 1);
	//}
	//else {
	//	wiiuse_set_ir(wm, 0);
	//}

	///*
	//* Motion+ support
	//*/
	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_ONE)) {
	//	if (WIIUSE_USING_EXP(wm)) {
	//		wiiuse_set_motion_plus(wm, 2);    // nunchuck pass-through
	//	}
	//	else {
	//		wiiuse_set_motion_plus(wm, 1);    // standalone
	//	}
	//}

	//if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_TWO)) {
	//	wiiuse_set_motion_plus(wm, 0); // off
	//}

	/* if the accelerometer is turned on then print angles */
	if (WIIUSE_USING_ACC(wm)) {
		UE_LOG(LogWiiUse, Warning, TEXT("WIIUSE_USING_ACC"));
		WiiController[i].Acc.Z = wm->orient.roll;
		WiiController[i].Acc.X = wm->orient.pitch;
		WiiController[i].Acc.Y = wm->orient.yaw;
		WiiController[i].AccAngle.Z = wm->orient.a_roll;
		WiiController[i].AccAngle.X = wm->orient.a_pitch;
		WiiController[i].AccAngle.Y = wm->orient.yaw;
	}

	/*
	*	If IR tracking is enabled then print the coordinates
	*	on the virtual screen that the wiimote is pointing to.
	*
	*	Also make sure that we see at least 1 dot.
	*/
	if (WIIUSE_USING_IR(wm)) {

		if (wm->ir.dot[0].visible) {
			WiiController[i].IR_Point1.X = wm->ir.dot[0].x;
			WiiController[i].IR_Point1.Y = wm->ir.dot[0].y;
			WiiController[i].IR_Point2.X = wm->ir.dot[1].x;
			WiiController[i].IR_Point2.Y = wm->ir.dot[1].y;
			WiiController[i].IR_Point3.X = wm->ir.dot[2].x;
			WiiController[i].IR_Point3.Y = wm->ir.dot[2].y;
			WiiController[i].IR_Point4.X = wm->ir.dot[3].x;
			WiiController[i].IR_Point4.Y = wm->ir.dot[3].y;
		}
		WiiController[i].IR_Cursor.X = wm->ir.x;
		WiiController[i].IR_Cursor.Y = wm->ir.y;
		WiiController[i].IR_CursorZ = wm->ir.z;
	}

}

void UWiiUseController::handle_ctrl_status(int32 i)
{
	struct wiimote_t* wm = wiimotes[i];

	UE_LOG(LogWiiUse, Warning, TEXT("handle_ctrl_status[id %i] -- - "), wm->unid);
}

void UWiiUseController::handle_disconnect(int32 i)
{
	struct wiimote_t* wm = wiimotes[i];

	UE_LOG(LogWiiUse, Warning, TEXT("handle_disconnect[id %i] -- - "), wm->unid);
}

FString UWiiUseController::ToString(int32 i) const
{
	return FString::Printf(TEXT("A=%i B=%i"),
		WiiController[i].Button_A,
		WiiController[i].Button_B);
}