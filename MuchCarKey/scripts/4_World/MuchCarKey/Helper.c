class MCK_Helper
{
	static bool MCK_PlayerHasCorrectKeyInInventory(PlayerBase player, int mckId)
	{
		array<EntityAI> entities = new array<EntityAI>;
		
		player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, entities);
		
		for (int i = 0; i < entities.Count(); i++)
		{
			EntityAI item = entities.Get(i);
			if (item)
			{
				if(item.IsInherited(MCK_MasterKey))
				{
					return true;
				}
				MCK_CarKey_Base carKey = MCK_CarKey_Base.Cast(item);
				if (carKey)
				{
					if (carKey.m_MCKId == mckId)
					{	
						return true;
					}
				}
			}
		}
		return false;
	}
};