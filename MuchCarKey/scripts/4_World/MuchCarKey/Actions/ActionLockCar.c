class ActionLockCar: ActionLockUnlockCar
{			
	override string GetText()
	{
		return "#mck_action_lock";
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

		MCK_MasterKey masterKey = MCK_MasterKey.Cast(item);
		if (transport && masterKey)
		{
			if (!MCK_Transport.IsLocked(transport) && MCK_Transport.CanDoAction(transport))
				return true;
		}

        MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
		if (transport && carKey)
		{
			if (MCK_Transport.HasMCKAssigned(transport) && MCK_Transport.CanDoAction(transport) && carKey.m_MCKId == MCK_Transport.GetTransportKeyId(transport))
				return true;
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
            MCK_Transport.LockTransport(transport);		
			MCK_LogPlayerActivity(player, "locked vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");		
		}
	}
};