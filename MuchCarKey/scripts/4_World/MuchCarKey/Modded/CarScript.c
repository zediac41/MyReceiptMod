modded class CarScript
{
	bool m_IsCKLocked;
	bool m_HasCKAssigned;
	int m_CarKeyId;
	int m_CarScriptId;
	int m_LastInteractedWithUnixTime;	
	bool m_HadPlayerInteraction;
	string m_OriginalOwnerId;
	string m_OriginalOwnerName;
	private bool m_Initialised;//client only
	bool m_DeletionInitialisedByMod;

	private static const string	MCK_LOCK_SOUND = "MCK_LockVehicle_SoundSet";
	private static const string	MCK_UNLOCK_SOUND = "MCK_UnlockVehicle_SoundSet";
	bool mck_PlaySound_LockUnlock = false;

	void CarScript()
	{
		m_IsCKLocked = false;
		m_CarKeyId = -1;
		m_CarScriptId = -1;
		m_LastInteractedWithUnixTime = -1;
		m_HadPlayerInteraction = false;
		m_DeletionInitialisedByMod = false;
		m_OriginalOwnerId =  "";
		m_OriginalOwnerName = "";
		m_Initialised = false;
		RegisterNetSyncVariableBool("m_IsCKLocked");
		RegisterNetSyncVariableBoolSignal("mck_PlaySound_LockUnlock");
		RegisterNetSyncVariableInt("m_CarKeyId", int.MIN, int.MAX - 1);
		RegisterNetSyncVariableInt("m_CarScriptId", int.MIN, int.MAX - 1);
	}  


	bool MCK_IsLocked()
	{
		return m_IsCKLocked;
	}

	void ResetVehicle()
	{
		m_CarKeyId = -1;
		m_IsCKLocked = false;
		m_HadPlayerInteraction = false;
		m_OriginalOwnerName = "";
		m_OriginalOwnerId =  "";
		SetLastInteractedWithTimeToNow();
		SetMCKLifetime();
		SetSynchDirty();
	}

	void ResetLifetime()
	{
		SetLastInteractedWithTimeToNow();
		m_HadPlayerInteraction = true;
		SetMCKLifetime();
	}

	void UnlockVehicle()
	{
		m_IsCKLocked = false;
		SetSynchDirty();
		ResetLifetime();
	}

	bool MCK_HasKeyAssigned()
	{
		return m_CarKeyId > -1;
	}

	void LockVehicle()
	{
		m_IsCKLocked = true;
		SetSynchDirty();
		ResetLifetime();
	}
	
	void MCK_SetHeadlights(bool headlightsOn)
	{
		m_HeadlightsOn = headlightsOn;
		SetSynchDirty();

		UpdateLights();
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		if (mck_PlaySound_LockUnlock)
		{		
			MCK_Transport.PlayLockUnlockSound_Client(this, MCK_LOCK_SOUND, MCK_UNLOCK_SOUND);
		}
	}
	override void OnEngineStart()
	{
		super.OnEngineStart();
		SetLastInteractedWithTimeToNow();
		SetMCKLifetime();
	}

	override void EEKilled(Object killer)
	{
		if (GetGame().IsServer())
		{
			if(!m_EngineDestroyed)
			{
				MCK_Transport.EEKilledLog(this, killer);
			}
		}
		super.EEKilled(killer);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (GetGame().IsServer())
		{
			if(m_DeletionInitialisedByMod)
			{
				return;
			}
			MCK_Transport.EEDeleteLog(this);
		}
	}

	override void DeferredInit()
	{
		super.DeferredInit();	
		DoMCKInitialize();
	}

	void DoMCKInitialize()
	{					
		if (!GetGame().IsServer())
			return;
		if(m_Initialised)
		{
			return;
		}			
		SetMCKLifetime();
		if (m_CarScriptId < 0)
		{
			m_CarScriptId = g_Game.GetMCKConfig().GetNextVehicleStoreID();
			SetLastInteractedWithTimeToNow();
		}
		int remainingTime = MCK_Transport.GetRemainingTimeTilDespawn(this, MCK_Transport.GetElapsedTimeSinceInteraction(m_LastInteractedWithUnixTime), m_HadPlayerInteraction);
		MCK_LogActivity(GetDisplayName() + " (ID: " + m_CarScriptId + " - pos " + GetPosition() + ") initialized." + " Remaining lifetime: " + SecondsToDays(remainingTime));
		SetSynchDirty();
		m_Initialised = true;
	}

	override bool NameOverride(out string output)
	{
		if(m_OriginalOwnerName != "")
		{
			string DisplayName;
			GetGame().ObjectGetDisplayName(this, DisplayName);	
			output = m_OriginalOwnerName + "'s " + DisplayName;
			return true;
		}
		return super.NameOverride(output);
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		Param4<bool, bool, int, int> mck_CarScriptData = new Param4<bool, bool, int, int>(m_IsCKLocked, m_HasCKAssigned, m_CarKeyId, m_CarScriptId);
		ctx.Write(mck_CarScriptData);
		ctx.Write(m_LastInteractedWithUnixTime);
		ctx.Write(m_HadPlayerInteraction);
		ctx.Write(m_OriginalOwnerName);
		ctx.Write(m_OriginalOwnerId);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		Param4<bool, bool, int, int> mck_CarScriptData = new Param4<bool, bool, int, int>(false, false, 0, 0);
		if (ctx.Read(mck_CarScriptData))
		{
			m_IsCKLocked = mck_CarScriptData.param1;
			m_HasCKAssigned = mck_CarScriptData.param2;
			m_CarKeyId = mck_CarScriptData.param3;
			m_CarScriptId = mck_CarScriptData.param4;
		}

		if (!ctx.Read(m_LastInteractedWithUnixTime))
			SetLastInteractedWithTimeToNow();
		if (!ctx.Read(m_HadPlayerInteraction))
			m_HadPlayerInteraction = false;
		if (!ctx.Read(m_OriginalOwnerName))
			m_OriginalOwnerName = "";
		if (!ctx.Read(m_OriginalOwnerId))
			m_OriginalOwnerId = "";
		
		SetSynchDirty();

		return true;
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		DoMCKInitialize();
	}
	
	override void EEOnCECreate()
	{
		super.EEOnCECreate();
		DoMCKInitialize();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();		
		if(!m_Initialised)
		{
			return;
		}	
		MCK_Transport.HandleLifetime(this, MCK_Transport.GetElapsedTimeSinceInteraction(m_LastInteractedWithUnixTime), m_HadPlayerInteraction);
	}

	void SetMCKLifetime()
	{		
		if(g_Game.GetMCKConfig().IsLifetimeMaintenanceEnabled())
		{			
			SetLifetime(g_Game.GetMCKConfig().Get_MaxLifetime());
		}
	}

	int GetLastInteractedWithTime()
	{
		if(m_LastInteractedWithUnixTime <= 0)
		{
			SetLastInteractedWithTimeToNow();
		}
		return m_LastInteractedWithUnixTime;
	}

	void SetLastInteractedWithTimeToNow()
	{
		m_LastInteractedWithUnixTime = CF_Date.Now(false).GetTimestamp();
	}

	override bool CanDisplayAttachmentSlot(int slot_id)
	{
		if (!super.CanDisplayAttachmentSlot(slot_id))
			return false;

		return !MCK_IsLocked();
	}

	override bool CanDisplayAttachmentCategory(string category_name)
	{
		if (!super.CanDisplayAttachmentCategory(category_name))
			return false;

		return !MCK_IsLocked();
	}
	
	//Don't use this. Use SetSynchDirty(); directly
	void SynchronizeValues()
	{
		if (GetGame().IsServer())
			SetSynchDirty();
	}

	bool CheckOpenedDoors()
	{
		TStringArray attachmentNames = new TStringArray;
		ConfigGetTextArray("attachments", attachmentNames);
		foreach (string att : attachmentNames)
		{
			if (CarDoor.Cast(FindAttachmentBySlotName(att)))
			{
				int doorState = GetCarDoorsState(att);
				if (doorState == CarDoorState.DOORS_OPEN || doorState == CarDoorState.DOORS_MISSING)
					return true;
			}
			
		}

		return false;
	}

	bool HasDoors()
	{
		TStringArray attachmentNames = new TStringArray;
		ConfigGetTextArray("attachments", attachmentNames);
		foreach (string att : attachmentNames)
		{
			if (CarDoor.Cast(FindAttachmentBySlotName(att)))
				return true;
		}

		return false;
	}

	bool CanDoAction()
	{			
		if (HasDoors())
			return !MCK_IsLocked() && !CheckOpenedDoors();
		else
			return !MCK_IsLocked();

		return false;
	}

	bool ShouldShowInv()
	{
		if (HasDoors())
			return !MCK_IsLocked() && CheckOpenedDoors();
		else
			return !MCK_IsLocked();

		return false;
	}

	override bool IsInventoryVisible()
	{
		bool superIsVisible = super.IsInventoryVisible();
		if(!g_Game.GetMCKConfig().Get_HideInventory())
		{
			if(!superIsVisible)
			{
				return false;
			}
			return !MCK_IsLocked();
		}
		return ShouldShowInv() && superIsVisible;
	}
};