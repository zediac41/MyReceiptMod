modded class ActionDetach
{
    override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
    {
		if (target)
		{
            Transport transport = Transport.Cast(target.GetParent());
            if(transport)
            {
                if (MCK_Transport.IsLocked(transport))
                    return false;
            }
		}

        return super.ActionCondition(player, target, item);
    }
};