class ActionPackVehicleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(2);
	}
}

class ActionPackVehicle : ActionContinuousBase
{
	void ActionPackVehicle()
	{
		m_CallbackClass = ActionPackVehicleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		CGMod garage = CGMod.Cast(item);
		if (garage)
		{
			m_Text = "Store Vehicle: " + CGModVehiclesHashTable.GetDisplayName(target.GetObject().GetType().Hash());
		}
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (target)
		{
			if (target.GetObject())
			{
				if (target.GetObject().IsTransport())
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		CGMod Garage = CGMod.Cast(action_data.m_MainItem);
		Transport tVeh = Transport.Cast(action_data.m_Target.GetObject());
		
		if (tVeh && !Garage.m_Stored)
			Garage.CaptureEx(tVeh);
		else
			return;
	}
}

class ActionUnPackVehicleCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(2);
	}
}

class ActionUnPackVehicle : ActionContinuousBase
{
	void ActionUnPackVehicle()
	{
		m_CallbackClass = ActionUnPackVehicleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override void OnActionInfoUpdate(PlayerBase player, ActionTarget target, ItemBase item)
	{
		CGMod garage = CGMod.Cast(item);
		if (garage)
		{
			string displayName = CGModVehiclesHashTable.GetDisplayName(garage.GetStoredHash());
			m_Text = "Release Vehicle: " + displayName;
		}
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		CGMod GarageAC = CGMod.Cast(player.GetItemInHands());
		
		if (GarageAC.m_Stored)
			return true;
		else
			return false;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		vector MyPosition = action_data.m_Player.GetPosition();
		MyPosition = MyPosition + (action_data.m_Player.GetDirection() * 3);
		CGMod Garage = CGMod.Cast(action_data.m_MainItem);
		Garage.ReleaseEx(MyPosition);
	}
}

class CGMod extends ItemBase
{
	ref array<string> savedAttachmentTypes = new array<string>;
	ref array<EntityAI> savedAttachments = new array<EntityAI>;
	
	private Transport m_StoredVehicle;
	int m_StoredVehicleTypeHash;
	bool m_Stored;
	protected PlayerBase PlrBase;
	protected int m_GarageSaveVersion;
	protected Transport m_Vehicle;
	protected int m_CarKeyHash;
	
	void CGMod()
	{
		RegisterNetSyncVariableBool("m_Stored");
		RegisterNetSyncVariableInt("m_StoredVehicleTypeHash");
	}
	
	void GetAttachmentsEx(Transport vehicle)
    {
        for (int i = 0; i < vehicle.GetInventory().AttachmentCount(); i++)
        {
			EntityAI VA = vehicle.GetInventory().GetAttachmentFromIndex(i);
			
			if (VA)
			{
            	savedAttachmentTypes.Insert(vehicle.GetInventory().GetAttachmentFromIndex(i).GetType());
			}
        }
    }
	
	void GetAttachmentsSEX(Transport vehicle)
    {
        for (int a = 0; a < vehicle.GetInventory().AttachmentCount(); a++)
        {
			EntityAI VAX = vehicle.GetInventory().GetAttachmentFromIndex(a);
			
			if (VAX)
			{
            	savedAttachments.Insert(VAX);
			}
        }
    }
	
    bool InventoryIsEmpty(Transport vehicle)
	{
		GetAttachmentsSEX(vehicle);
		if (vehicle.GetInventory().GetCargo() != NULL)
		{
			if (vehicle.GetInventory().GetCargo().GetItemCount() > 0)
				return false;
			
			foreach(EntityAI EAIAttachment : savedAttachments)
			{
				if (EAIAttachment)
				{
					if (EAIAttachment.GetInventory().GetCargo() != NULL)
					{
						if (EAIAttachment.GetInventory().GetCargo().GetItemCount() > 0)
							return false;
					}
					
					if (vehicle.GetInventory().FindAttachmentByName("shoulder") || vehicle.GetInventory().FindAttachmentByName("Shoulder"))
            			return false;
				}
			}
		}
		
		return true;
	}
	
	bool VehicleHasNoPassengers(Transport vehicle)
	{
		int crewSizeEx = vehicle.CrewSize();
		for (int cm = 0; cm < crewSizeEx; cm++)
		{
			if (vehicle.CrewMember(cm))
				return false;
		}
		
		return true;
	}
	
	void RefillVehicleFluids(CarScript CS)
	{
		if (CS)
		{
			float FuelNeeded = CS.GetFluidCapacity(CarFluid.FUEL) - (CS.GetFluidCapacity(CarFluid.FUEL) * CS.GetFluidFraction(CarFluid.FUEL));
			float OilNeeded = CS.GetFluidCapacity(CarFluid.OIL) - (CS.GetFluidCapacity(CarFluid.OIL) * CS.GetFluidFraction(CarFluid.OIL));
			float CoolantNeeded = CS.GetFluidCapacity(CarFluid.COOLANT) - (CS.GetFluidCapacity(CarFluid.COOLANT) * CS.GetFluidFraction(CarFluid.COOLANT));
			float BrakeFluidNeeded = CS.GetFluidCapacity(CarFluid.BRAKE) - (CS.GetFluidCapacity(CarFluid.BRAKE) * CS.GetFluidFraction(CarFluid.BRAKE));
			CS.Fill(CarFluid.FUEL, FuelNeeded);
			CS.Fill(CarFluid.OIL, OilNeeded);
			CS.Fill(CarFluid.COOLANT, CoolantNeeded);
			CS.Fill(CarFluid.BRAKE, BrakeFluidNeeded);
			CS.SetSynchDirty();
			CS.Synchronize();
		}
	}
	
	void CaptureEx(Transport vehicle)
	{
		PlrBase = PlayerBase.Cast(GetGame().GetPlayer());	
		CarScript CSMCK = CarScript.Cast(vehicle);
		
		if (InventoryIsEmpty(vehicle) && VehicleHasNoPassengers(vehicle))
		{
			#ifdef MuchCarKey
			if (CSMCK.m_HasCKAssigned && !CSMCK.m_IsCKLocked)
			{
				m_StoredVehicle = vehicle;
				m_StoredVehicleTypeHash = vehicle.GetType().Hash();

				#ifdef MuchCarKey
				m_CarKeyHash = CSMCK.m_CarKeyId;
				#endif
				
				m_Stored = true;
				
				SetSynchDirty();
				
				GetAttachmentsEx(vehicle);
				
				vehicle.Delete();
			}
			#endif
		}
		else
		{
			savedAttachmentTypes.Clear();
			savedAttachments.Clear();
		}
	}
	
	void ReleaseEx(vector releasePos)
	{
		if (GetGame().IsServer())
		{
			m_Stored = false;
			SetSynchDirty();
			
			//Find object type via stored hash
			string objectType = CGModVehiclesHashTable.GetType(m_StoredVehicleTypeHash);
			if (objectType == string.Empty)
			{
				Print("[GarageScript] ReleaseEx:: Fatal Error! Could not retrieve object type. Type changed?");
				return;
			}
			Object rVehicle = GetGame().CreateObject(objectType, releasePos);
			
			m_Vehicle = Transport.Cast(rVehicle);
			
			m_StoredVehicleTypeHash = 0;
			
			if (m_Vehicle)
			{
				foreach(string attachment : savedAttachmentTypes)
				{
					if (attachment)
					{
						m_Vehicle.GetInventory().CreateAttachment(attachment);
					}
				}
				
				CarScript CScript = CarScript.Cast(m_Vehicle);
				#ifdef MuchCarKey
				CScript.m_CarKeyId = m_CarKeyHash;
				CScript.m_HasCKAssigned = true;
              	CScript.SetSynchDirty();
				#endif
				RefillVehicleFluids(CScript);

				savedAttachmentTypes.Clear();
				savedAttachments.Clear();
			}
		}
	}
	
	override bool NameOverride(out string output)
	{
		if (m_Stored)
		{
			//Find name using CGModVehiclesHashTable
			string displayName = CGModVehiclesHashTable.GetDisplayName(m_StoredVehicleTypeHash);
			output = "Stored Vehicle: " + displayName;
			return true;
		}
		else
		{
			output = "Empty";
			return true;
		}
		return false;
	}

	int GetGarageSaveVersion() //Credits to WarDog 
	{
		return 1;
	}

	int GetStoredHash()
	{
		return m_StoredVehicleTypeHash;
	}
	
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		m_GarageSaveVersion = GetGarageSaveVersion();
		ctx.Write(m_GarageSaveVersion);
		ctx.Write(m_StoredVehicleTypeHash);
		ctx.Write(m_Stored);
		ctx.Write(m_CarKeyHash);
		ctx.Write(savedAttachmentTypes.Count());
		
		for (int i = 0; i < savedAttachmentTypes.Count(); i++)
			ctx.Write(savedAttachmentTypes[i]);
		
	}
	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;
		
		if (!ctx.Read(m_GarageSaveVersion))
			return false;

		if (!ctx.Read(m_StoredVehicleTypeHash))
			return false;

		if (!ctx.Read(m_Stored))
			return false;
		
		if (!ctx.Read(m_CarKeyHash))
			return false;
		
		int count;
		if (!ctx.Read(count))
			return false;

		for (int i = 0; i < count; i++)
		{	
			string element;
			if (!ctx.Read(element))
				return false;
			
			savedAttachmentTypes.Insert(element);
		}
		
		return true;
	}
	
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionPackVehicle);
		AddAction(ActionUnPackVehicle);
	}
}