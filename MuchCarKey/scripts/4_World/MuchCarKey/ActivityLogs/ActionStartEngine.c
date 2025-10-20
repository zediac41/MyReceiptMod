#ifdef SERVER
modded class ActionStartEngine
{
	override void OnEndServer(ActionData action_data)
	{
        super.OnEndServer(action_data);
		if (action_data.m_Player && action_data.m_Player.GetIdentity())
		{
			HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
			if (vehCommand)
			{
				Transport transport = Transport.Cast(vehCommand.GetTransport());
				if (transport && g_Game.GetMCKConfig().AreExtendedLogsActivated())
				{					
					MCK_LogPlayerActivity(action_data.m_Player, "started " + transport.GetDisplayName() + " (ID: " + MCK_Transport.GetTransportId(transport) + ")");
				}					
			}
		}
    }
}
#endif