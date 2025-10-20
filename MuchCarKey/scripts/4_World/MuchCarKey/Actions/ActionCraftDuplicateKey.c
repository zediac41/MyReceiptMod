class ActionCraftDuplicateKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionCraftDuplicateKey: ActionLockUnlockCar
{	
	void ActionCraftDuplicateKey()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_CallbackClass = ActionCraftDuplicateKeyCB;
		m_LockTargetOnUse 	= false;
		m_Text = "#mck_action_duplicate";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if(!target) 
			return false;

		Object targetObject = target.GetObject();
		if (!targetObject)
		{			
			targetObject = target.GetParent();
			if (!target.GetParent())
			{
				return false;
			}
		}

		bool HasCorrectTarget = false;
		if(targetObject.IsInherited(MCK_KeyGrinder)) 
		{
			HasCorrectTarget = true;
		}
		if(g_Game.GetMCKConfig().CanUseAnyGrinder && !HasCorrectTarget)
		{
			if(targetObject.IsKindOf("StaticObj_Furniture_grinder")  || HasGrinderProxy(target)) 
			{
				HasCorrectTarget = true;
			}
		}		

		if(!HasCorrectTarget)
		{
			return false;
		}
		
        MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
		if (carKey)
		{
			m_Text = "#mck_action_duplicate";
			if(!carKey.IsMCKAssigned())
			{
				m_Text = "#mck_action_duplicate_error"
				if(GetGame().IsServer())
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			
			return true;            
		}

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(action_data.m_MainItem);            
		if(carKey)
		{
			int mck_id = carKey.m_MCKId;
			//carKey.DecreaseHealth( 50, false );
			MCK_CarKey_Base duplicateKey = MCK_CarKey_Base.Cast( GetGame().CreateObject(carKey.GetType(), player.GetPosition()) );			
			duplicateKey.SetNewMCKId(mck_id);
			MCK_LogPlayerActivity(player, "duplicated key (ID: " + mck_id + " )");
		}
	}    
};