class ActionResetKeyIdCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(10);
	}
};

class ActionResetKeyId: ActionLockUnlockCar
{	
	void ActionResetKeyId()
	{
        m_CallbackClass = ActionResetKeyIdCB;        
	}
		
	override string GetText()
	{
		return "#mck_action_resetkey";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!target) 
			return false;

		if (!target.GetObject())
			return false;

		
		Transport transport = Transport.Cast(target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(target.GetParent());
			if(!transport)
			{
				return false;
			}
		}
		
		if (transport)
		{
        	MCK_ResetKey resetKey = MCK_ResetKey.Cast(item);
			if(resetKey)
			{
				return true;
			}
			
        	MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
			if(carKey && carKey.m_MCKId == MCK_Transport.GetTransportKeyId(transport))
			{
				if(!MCK_Transport.HasMCKAssigned(transport) || !g_Game.GetMCKConfig().Get_CanPlayersResetKey())
				{
					return false;
				}
				return true;   
			}
         
		}

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(action_data.m_Target.GetParent());
		}
		if( transport )
		{          
			MCK_Transport.ResetTransport(transport);
			MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(action_data.m_MainItem);            
            if(carKey)
            {
				carKey.ResetKey();
			}
			MCK_LogPlayerActivity(player, "reset key for vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");
		}
	}    
};