// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class WiiUseLibrary : ModuleRules
{
	public WiiUseLibrary(TargetInfo Target)
	{
		Type = ModuleType.External;
		
		Definitions.Add("WIIUSE_STATIC=1");
		Definitions.Add("WIIUSE_WIN32=1");

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "lib"));
			PublicAdditionalLibraries.Add("wiiuse.lib");
			//PublicAdditionalLibraries.Add("C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/x64/hid.lib");
			PublicAdditionalLibraries.Add("hid.lib");
		}
		
	}
}
