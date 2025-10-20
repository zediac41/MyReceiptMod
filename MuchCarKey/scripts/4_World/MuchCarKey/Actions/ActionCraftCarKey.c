class ActionCraftCarKeyCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ActionCraftCarKey: ActionLockUnlockCar
{	
	
	ref array<string> m_MCK_Keys = { "MCK_CarKey_Base", "MCK_CarKey_Blue", "MCK_CarKey_Green", "MCK_CarKey_Red",  
	"MCK_CarKey_White", "MCK_CarKey_Yellow", "MCK_CarKey_Purple", "MCK_CarKey_Pink" };
	void ActionCraftCarKey()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_LockTargetOnUse 	= false;
        m_CallbackClass = ActionCraftCarKeyCB;        
	}
		
	override string GetText()
	{
		return "#mck_action_craft";
	}
	
	// override bool HasTarget()
	// {
	// 	return false;
	// }

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

		if(!g_Game.GetMCKConfig().Get_CanCraftKey())
			return false;

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

		Wrench wrench = Wrench.Cast(item);
		if (wrench)
		{
			return true;
		}
		

        return false;
	}
	
	override void OnFinishProgressServer( ActionData action_data )
	{	
		PlayerBase player = action_data.m_Player;
		Wrench wrench = Wrench.Cast(action_data.m_MainItem);            
		if(wrench)
		{
			MCK_CarKey_Base duplicateKey = MCK_CarKey_Base.Cast( GetGame().CreateObject(m_MCK_Keys.GetRandomElement(), player.GetPosition()) );			
			wrench.DeleteSafe();
			MCK_LogPlayerActivity(player, "crafted new key.");
		}
	}    
};