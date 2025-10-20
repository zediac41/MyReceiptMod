class ActionMCKRemoteOpenClose: ActionSingleUseBase
{
	private int m_LastActionCheck = 0;
	private int m_LastActionCheckTimer = 3000; //miliseconds

	void ActionMCKRemoteOpenClose()
	{
        m_Text = "Remote lock/unlock vehicle";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
		if (carKey)
		{
            int currentTime = GetGame().GetTime();
            if(currentTime - m_LastActionCheck > m_LastActionCheckTimer)
            {
			    return carKey.IsMCKAssigned();
            }
		}
		return false;
	}
	
	override void OnExecuteServer( ActionData action_data )
	{
		//find car nearby with id
        PlayerBase player = PlayerBase.Cast(action_data.m_Player);
        MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(action_data.m_MainItem);
        if(player && carKey)
        {
            Transport transport = carKey.FindNearbyTransport(player);
            if(!transport)
            {
                return;
            }
            MCK_Transport.PlayLockUnlockSound_Server(transport);
            CarScript carScript = CarScript.Cast(transport);
            if(carScript)
            {
                if(carScript.MCK_IsLocked())
                {
                    carScript.MCK_SetHeadlights(false);
                    carScript.MCK_SetHeadlights(true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.MCK_SetHeadlights, 200, false, false);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.MCK_SetHeadlights, 300, false, true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.MCK_SetHeadlights, 500, false, false);
                }
                else
                {
                    carScript.MCK_SetHeadlights(false);
                    carScript.MCK_SetHeadlights(true);
                    GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(carScript.MCK_SetHeadlights, 500, false, false);
                }
            }
            if(MCK_Transport.IsLocked(transport))
            {
                MCK_LogPlayerActivity(player, "locked vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");
            }
            else
            {
                MCK_LogPlayerActivity(player, "unlocked vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");
            }
            
            MCK_Transport.ToggleLockTransport(transport);		
        }
	}

    override void OnExecuteClient( ActionData action_data )
	{
        int currentTime = GetGame().GetTime();
        if(currentTime - m_LastActionCheck > m_LastActionCheckTimer)
        {
            m_LastActionCheck = currentTime;
        }
    }
};