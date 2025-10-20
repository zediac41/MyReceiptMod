modded class BoatScript
{
	bool mck_IsLocked = false;
	int mck_KeyId = -1;
	int mck_ObjectId = -1;
	int mck_LastInteractedWithUnixTime = -1;	
	bool mck_HadPlayerInteraction = false;
	string mck_OriginalOwnerId =  "";
	string mck_OriginalOwnerName = "";
	private bool mck_Initialised = false;//client only
	bool mck_DeletionInitialisedByMod = false;

	private static const string	MCK_LOCK_SOUND = "MCK_LockVehicle_SoundSet";
	private static const string	MCK_UNLOCK_SOUND = "MCK_UnlockVehicle_SoundSet";
	bool mck_PlaySound_LockUnlock = false;

	void BoatScript()
	{
		RegisterNetSyncVariableBool("mck_IsLocked");
		RegisterNetSyncVariableBoolSignal("mck_PlaySound_LockUnlock");
		RegisterNetSyncVariableInt("mck_KeyId", -1, int.MAX - 1);
		RegisterNetSyncVariableInt("mck_ObjectId", -1, int.MAX - 1);
	} 

	bool MCK_IsLocked()
	{
		return mck_IsLocked;
	}

	void MCK_Reset()
	{
		mck_KeyId = -1;
		mck_IsLocked = false;
		mck_HadPlayerInteraction = false;
		mck_OriginalOwnerName = "";
		mck_OriginalOwnerId =  "";
		SetLastInteractedWithTimeToNow();
		MCK_SetLifetime();
		SetSynchDirty();
	}

	void MCK_ResetLifetime()
	{
		SetLastInteractedWithTimeToNow();
		mck_HadPlayerInteraction = true;
		MCK_SetLifetime();
	}

	void MCK_Unlock()
	{
		mck_IsLocked = false;
		SetSynchDirty();
		MCK_ResetLifetime();
	}

	void MCK_Lock()
	{
		mck_IsLocked = true;
		SetSynchDirty();
		MCK_ResetLifetime();
	}

	bool MCK_HasKeyAssigned()
	{
		return mck_KeyId > -1;
	}

	void MCK_SetObjectID(int newID)
	{
		mck_ObjectId = newID;
		SetSynchDirty();
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
		MCK_SetLifetime();
	}

	override void EEKilled(Object killer)
	{
		if (GetGame().IsServer())
		{			
			MCK_Transport.EEKilledLog(this, killer);
		}
		super.EEKilled(killer);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);
		if (GetGame().IsServer())
		{
			if(mck_DeletionInitialisedByMod)
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
		if(mck_Initialised)
		{
			return;
		}			
		MCK_SetLifetime();
		if (mck_ObjectId == -1)
		{
			mck_ObjectId = g_Game.GetMCKConfig().GetNextVehicleStoreID();			
			SetLastInteractedWithTimeToNow();
		}
		int remainingTime = MCK_Transport.GetRemainingTimeTilDespawn(this, MCK_Transport.GetElapsedTimeSinceInteraction(mck_LastInteractedWithUnixTime), mck_HadPlayerInteraction);
		MCK_LogTransportActivity(this,  "initialized." + " Remaining lifetime: " + SecondsToDays(remainingTime));
		SetSynchDirty();
		mck_Initialised = true;
	}

	override bool NameOverride(out string output)
	{
		if(mck_OriginalOwnerName != "")
		{
			string DisplayName;
			GetGame().ObjectGetDisplayName(this, DisplayName);	
			output = mck_OriginalOwnerName + "'s " + DisplayName;
			return true;
		}
		return super.NameOverride(output);
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		ctx.Write(mck_IsLocked);
		ctx.Write(mck_KeyId);
		ctx.Write(mck_ObjectId);

		ctx.Write(mck_LastInteractedWithUnixTime);
		ctx.Write(mck_HadPlayerInteraction);
		ctx.Write(mck_OriginalOwnerName);
		ctx.Write(mck_OriginalOwnerId);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		if (!ctx.Read(mck_IsLocked))
			mck_IsLocked = false;
		if (!ctx.Read(mck_KeyId))
			mck_KeyId = -1;
		if (!ctx.Read(mck_ObjectId))
			mck_ObjectId = -1;
		if (!ctx.Read(mck_LastInteractedWithUnixTime))
			SetLastInteractedWithTimeToNow();
		if (!ctx.Read(mck_HadPlayerInteraction))
			mck_HadPlayerInteraction = false;
		if (!ctx.Read(mck_OriginalOwnerName))
			mck_OriginalOwnerName = "";
		if (!ctx.Read(mck_OriginalOwnerId))
			mck_OriginalOwnerId = "";

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
		if(!mck_Initialised)
		{
			return;
		}	
		MCK_Transport.HandleLifetime(this, MCK_Transport.GetElapsedTimeSinceInteraction(mck_LastInteractedWithUnixTime), mck_HadPlayerInteraction);
	}

	void MCK_SetLifetime()
	{		
		if(g_Game.GetMCKConfig().IsLifetimeMaintenanceEnabled())
		{			
			SetLifetime(g_Game.GetMCKConfig().Get_MaxLifetime());
		}
	}

	int GetLastInteractedWithTime()
	{
		if(mck_LastInteractedWithUnixTime <= 0)
		{
			SetLastInteractedWithTimeToNow();
		}
		return mck_LastInteractedWithUnixTime;
	}
	
	void SetLastInteractedWithTimeToNow()
	{
		mck_LastInteractedWithUnixTime = CF_Date.Now(false).GetTimestamp();
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

	override bool IsInventoryVisible()
	{
		if(!super.IsInventoryVisible())
		{
			return false;
		}
		return !MCK_IsLocked();
	}
};