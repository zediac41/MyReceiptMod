modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
    {
        super.RegisterOneHanded(pType, pBehavior);
        pType.AddItemInHandsProfileIK("CGMod", "dz/anims/workspaces/player/player_main/props/player_main_1h_fruit.asi", pBehavior, "dz/anims/anm/player/ik/gear/zucchini_fresh.anm");
    }
};