#ifdef SERVER
modded class ActionSwitchSeats: ActionBase
{
	override void Start( ActionData action_data )
	{
		super.Start( action_data );
		if (action_data.m_Player && action_data.m_Player.GetIdentity())
		{
			HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
			if( vehCommand )
			{				
				Transport transport = Transport.Cast(vehCommand.GetTransport());
				if (transport && g_Game.GetMCKConfig().AreExtendedLogsActivated())
				{
					int componentIndex = action_data.m_Target.GetComponentIndex();
					int crew_index = transport.CrewPositionIndex(componentIndex);
					string position = "";
					switch( crew_index )
					{
						case 0:
							position = "Driver seat";
							break;
						case 1:
							position = "CoDriver seat";
							break;
						case 2:
							position = "Passenger Left seat";
							break;
						case 3:
							position = "Passenger Right seat";
							break;
					}
					if(GetGame().IsServer())
					{
						MCK_LogPlayerActivity(action_data.m_Player, "switched seats in vehicle " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")" + " to " + position);	
					}
				}
			}				
		}
	}
};
#endif