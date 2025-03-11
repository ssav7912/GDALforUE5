using UnrealBuildTool;

public class UnrealGDAL : ModuleRules
{
	public UnrealGDAL(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"GDAL",
				"UnrealEd"
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"AssetTools"
			}
		);
	}
}
