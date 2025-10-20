modded class DayZPlayerImplementThrowing
{
	override void HandleThrowing(HumanInputController pHic, HumanCommandWeapons pHcw, EntityAI pEntityInHands, float pDt)
	{
		if (pEntityInHands)
		{
			if (pEntityInHands.GetType() == "CGMod")
				return;
			else
				super.HandleThrowing(pHic,pHcw, pEntityInHands, pDt);
		}
		else
			super.HandleThrowing(pHic,pHcw, pEntityInHands, pDt);
	}
};
