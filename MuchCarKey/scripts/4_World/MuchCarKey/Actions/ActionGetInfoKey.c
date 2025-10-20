class ActionGetInfoKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(1);
	}
};

class ActionGetInfoKey: ActionLockUnlockCar
{	
	void ActionGetInfoKey()
	{
		ActionLockUnlockCar();
        m_CallbackClass = ActionGetInfoKeyCB;        
	}
		
	override string GetText()
	{
		return "#mck_action_getinfo";
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

        MCK_InfoKey infoKey = MCK_InfoKey.Cast(item);
		if (infoKey && transport)
		{
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
			MCK_InfoKey infoKey = MCK_InfoKey.Cast(action_data.m_MainItem);            
            if(infoKey)
            {          
                DisplayReport(action_data.m_Player, transport);
            }
		}

	}

	override void OnFinishProgressClient( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Transport transport = Transport.Cast(action_data.m_Target.GetObject());
		if(!transport)
		{
			transport = Transport.Cast(action_data.m_Target.GetParent());
		}
		if( transport )
		{
			MCK_InfoKey infoKey = MCK_InfoKey.Cast(action_data.m_MainItem);            
            if(infoKey)
            {          
				string clipboard = MCK_Transport.GetTransportId(transport).ToString();
				GetGame().CopyToClipboard(clipboard);
            }
		}
	}

	void DisplayReport(PlayerBase player, Transport transport)
	{
		SendMCKMessageToClient(player, "Vehicle Report:");
		SendMCKMessageToClient(player, "Vehicle: " + transport);
		SendMCKMessageToClient(player, "Vehicle key id: " + MCK_Transport.GetTransportKeyId(transport));
		SendMCKMessageToClient(player, "Vehicle id: " + MCK_Transport.GetTransportId(transport));
		SendMCKMessageToClient(player, "Vehicle original owner: " + MCK_Transport.GetOriginalOwnerName(transport));
		SendMCKMessageToClient(player, "Vehicle original owner id: " + MCK_Transport.GetOriginalOwnerId(transport));
		SendMCKMessageToClient(player, "Last time interacted: " + TimestampToString(MCK_Transport.GetLastInteractedWithTime(transport)));
		SendMCKMessageToClient(player, "Remaining lifetime: " + SecondsToDays(MCK_Transport.GetRemainingTimeTilDespawnTransport(transport)));
		//SendMCKMessageToClient(player, "Max lifetime from types: " + SecondsToDays(transport.GetLifetimeMax()));
	}
	
	void SendMCKMessageToClient(Object reciever, string message)
	{
		//Printing to server too
		MCK_LogActivity(message);
		PlayerBase man;
		if (GetGame().IsServer() && Class.CastTo(man, reciever) && reciever.IsAlive() && message != "")
		{
			Param1<string> reportParam = new Param1<string>("");
			reportParam.param1 = message;
			GetGame().RPCSingleParam(man, ERPCs.RPC_USER_ACTION_MESSAGE, reportParam, true, man.GetIdentity());
		}
	}
};