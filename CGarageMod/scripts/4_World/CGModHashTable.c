[CGModVehiclesHashTable()]
class CGModVehiclesHashTable
{
	static ref map<int, ref Param2<string,string>> m_Vehicles = new map<int, ref Param2<string,string>>;

	void CGModVehiclesHashTable()
	{
        int nClasses = g_Game.ConfigGetChildrenCount("CfgVehicles");
        for (int nClass = 0; nClass < nClasses; ++nClass)
        {
            string strName;
            GetGame().ConfigGetChildName("CfgVehicles", nClass, strName);

            int scope = GetGame().ConfigGetInt("CfgVehicles " + strName + " scope");
            if (scope == 0 || scope == 1)
                continue;

            //filter for transport only
            if (GetGame().IsKindOf(strName, "transport"))
            {
            	string displayName = GetGame().ConfigGetTextOut("CfgVehicles " + strName + " displayName");
            	CGModVehiclesHashTable.m_Vehicles.Insert(strName.Hash(), new Param2<string, string>(strName, displayName));
            }
        }
	}

	static string GetType(int hash)
	{
		Param2<string,string> dta = m_Vehicles.Get(hash);
		if (!dta)
			return string.Empty;

		return dta.param1;
	}

	static string GetDisplayName(int hash)
	{
		Param2<string,string> dta = m_Vehicles.Get(hash);
		if (!dta)
			return string.Empty;

		return dta.param2;
	}
}
