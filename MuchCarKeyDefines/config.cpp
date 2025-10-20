////////////////////////////////////////////////////////////////////
//DeRap: config.bin
//Produced from mikero's Dos Tools Dll version 9.98
//https://mikero.bytex.digital/Downloads
//'now' is Mon Oct 20 14:27:50 2025 : 'file' last modified on Fri Aug 30 01:45:05 2024
////////////////////////////////////////////////////////////////////

#define _ARMA_

class CfgPatches
{
	class MuchCarKeyDefines
	{
		requiredVersion = 0.1;
		requiredAddons[] = {};
		units[] = {};
		weapons[] = {};
	};
};
class CfgMods
{
	class MuchCarKeyDefines
	{
		dir = "MuchCarKeyDefines";
		name = "MuchCarKey";
		type = "mod";
		dependencies[] = {"GameLib","Game","World","Mission"};
		class defs
		{
			class gameLibScriptModule
			{
				value = "";
				files[] = {"MuchCarKeyDefines/scripts/Common"};
			};
			class gameScriptModule
			{
				value = "";
				files[] = {"MuchCarKeyDefines/scripts/Common"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"MuchCarKeyDefines/scripts/Common"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"MuchCarKeyDefines/scripts/Common"};
			};
		};
	};
};
