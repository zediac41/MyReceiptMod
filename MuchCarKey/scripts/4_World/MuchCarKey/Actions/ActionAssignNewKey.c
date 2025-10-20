class ActionAssignNewKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionAssignNewKey: ActionLockUnlockCar
{	
	void ActionAssignNewKey()
	{
		ActionLockUnlockCar();
        m_CallbackClass = ActionAssignNewKeyCB;        
	}
		
	override string GetText()
	{
		return "#mck_action_assign";
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

        MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
		if (carKey && carKey.GetMCKId() <= 0)
		{
			if(transport)
			{
				if(MCK_Transport.HasMCKAssigned(transport))
					return false;

				if(MCK_Transport.CanDoAction(transport))
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
			MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(action_data.m_MainItem);            
            if(carKey)
            {
                int mck_id = carKey.GenerateNewID();
				carKey.SetNewMCKId(mck_id);
				MCK_Transport.AssignNewKey(transport, mck_id, player);				
				MCK_LogPlayerActivity(player, " assigned new owner and key (ID: " + mck_id + " ) to vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");
            }
		}
	}    
};