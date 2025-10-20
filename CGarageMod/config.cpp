////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 9.45
//https://mikero.bytex.digital/Downloads
//'now' is Sat Oct 19 19:58:59 2024 : 'file' last modified on Sat Mar 11 20:36:34 2023
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class razorsynapse
	{
		units[] = {"CGMod"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts"};
	};
};
class CfgMods
{
	class razorsynapse
	{
		type = "mod";
		author = "...";
		dir = "CGarageMod";
		name = "CGarageMod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule
			{
				files[] = {"MuchCarKey/Scripts/Common","MuchCarKeyDefines/scripts/Common","CGarageMod/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				files[] = {"MuchCarKey/Scripts/Common","MuchCarKeyDefines/scripts/Common","CGarageMod/Scripts/4_World"};
			};
			class missionScriptModule
			{
				files[] = {"MuchCarKey/Scripts/Common","MuchCarKeyDefines/scripts/Common","CGarageMod/Scripts/5_Mission"};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class CGMod: Inventory_Base
	{
		scope = 2;
		displayName = "Custom Vehicle Garage";
		descriptionShort = "ONLY 1 VEHICLE, per steering wheel. To store the vehicle: It has to have a key assigned, it has to be unlocked, inventory of the vehicle and any attachments on the vehicles must be empty, and can't have anyone in the vehicle.";
		model = "CGarageMod\3DModels\SteeringWheel.p3d";
		weight = 100;
		itemSize[] = {2,2};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 10000000;
				};
			};
		};
		soundImpactType = "grenade";
	};
};
