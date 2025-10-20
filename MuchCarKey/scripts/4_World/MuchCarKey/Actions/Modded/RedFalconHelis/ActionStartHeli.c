// unsure if we need an extra setting -_- so postponing it
// #ifdef RFFSHeli_Core
// modded class ActionStartHelicopter: ActionContinuousBase
// {		
// 	private bool m_LastInventoryCheckReturnValue = false;
// 	private int m_LastInventoryCheck = 0;
// 	private int m_LastInventoryCheckTimer = 5000;

// 	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
// 	{	
// 		if(!super.ActionCondition(player, target, item))
// 		{
// 			return false;
// 		}		

// 		return MCK_StartEngineCondition(player);
// 	}

// 	bool MCK_StartEngineCondition(PlayerBase player)
// 	{
// 		MCK_EngineStartSettings settings = g_Game.GetMCKConfig().GetEngineStartSettings();
// 		if(settings)
// 		{
// 			if (player.GetCommand_Vehicle())
// 			{
// 				Transport transport = Transport.Cast(player.GetCommand_Vehicle().GetTransport());
// 				if (!transport)
// 				{
// 					return false;
// 				}

// 				if(settings.CanStartWithoutKey || (settings.CanStartUnassignedVehiclesWithoutKey && !MCK_Transport.HasMCKAssigned(transport)))
// 				{
// 					return true;
// 				}
				
// 				return MCK_CanPlayerStartEngine(player, transport);
// 			}
// 		}
// 		return true;
// 	}	

// 	bool MCK_CanPlayerStartEngine(PlayerBase player, Transport transport)
// 	{
// 		MCK_EngineStartSettings settings = g_Game.GetMCKConfig().GetEngineStartSettings();
// 		if(settings)
// 		{
// 			if(settings.CanStartWithKeyInHandOnly)
// 			{
// 				if(!player.GetItemInHands())
// 				{
// 					return false;
// 				}
// 				if(player.GetItemInHands().IsInherited(MCK_MasterKey))
// 				{
// 					return true;
// 				}
// 				MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(player.GetItemInHands());
// 				if (transport && carKey)
// 				{
// 					if (MCK_Transport.HasMCKAssigned(transport) && carKey.m_MCKId == MCK_Transport.GetTransportKeyId(transport))
// 					{	
// 						return true;
// 					}
// 				}
// 				return false;
// 			}
// 			if(settings.CanStartWithKeyInInventory)
// 			{
// 				int currentTime = GetGame().GetTime();
// 				if(currentTime - m_LastInventoryCheck > m_LastInventoryCheckTimer)
// 				{
// 					m_LastInventoryCheck = currentTime;
// 					m_LastInventoryCheckReturnValue = false;
// 					if (MCK_Transport.HasMCKAssigned(transport))
// 					{
// 						m_LastInventoryCheckReturnValue = MCK_Helper.MCK_PlayerHasCorrectKeyInInventory(player, MCK_Transport.GetTransportKeyId(transport));
// 					}
					
// 					return m_LastInventoryCheckReturnValue;
// 				}
// 				else
// 				{
// 					return m_LastInventoryCheckReturnValue;
// 				}
// 			}
// 		}
// 		return true;
// 	}
// };
// #endif
