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
#include "Core.h"
#include "ModuleManager.h"
#include "IPluginManager.h"

#define LOCTEXT_NAMESPACE "FUEWiiUsePluginModule"

DEFINE_LOG_CATEGORY(LogWiiUse)

void FUEWiiUsePluginModule::StartupModule()
{

}

void FUEWiiUsePluginModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUEWiiUsePluginModule, UEWiiUsePlugin)
