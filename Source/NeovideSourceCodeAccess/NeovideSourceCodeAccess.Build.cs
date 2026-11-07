using UnrealBuildTool;

public class NeovideSourceCodeAccess : ModuleRules
{
	public NeovideSourceCodeAccess(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"SourceCodeAccess"
		});
	}
}
