
using System.IO;
using UnrealBuildTool;
using System.Collections.Generic;

public class GDAL : ModuleRules
{
	public GDAL(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		//Add include directory
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

		//add shared libs.
		var libs = Directory.GetFiles(Path.Combine(ModuleDirectory, "lib"), "*" + ".lib", SearchOption.AllDirectories);
		foreach (string lib in libs)
		{
			PublicAdditionalLibraries.Add(lib);
		}

		//add dlls
		var dlls = new List<string>(Directory.GetFiles(Path.Combine(ModuleDirectory, "lib"), "*" + ".dll"));
		dlls.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "lib", "gdalplugins"), "*" + ".dll"));
		dlls.AddRange(Directory.GetFiles(Path.Combine(ModuleDirectory, "bin"), "*" + ".dll"));
        string stagingDir = Path.Combine("$(PluginDir)", "Binaries", "Data", "GDAL");
		string binaryStagingDir = Path.Combine("$(PluginDir)", "Binaries", "Win64");
        foreach (string dll in dlls)
		{
			//Console.WriteLine(dll);
			RuntimeDependencies.Add(Path.Combine(binaryStagingDir, Path.GetFileName(dll)), dll, StagedFileType.NonUFS);
		}
		//add data files
		var datafiles = Directory.GetFiles(Path.Combine(ModuleDirectory, "share", "gdal"), "*",
			SearchOption.AllDirectories);
		if (!Directory.Exists(stagingDir))
		{
			Directory.CreateDirectory(stagingDir);
		}
		foreach (string data in datafiles)
		{
			RuntimeDependencies.Add(Path.Combine(stagingDir, Path.GetFileName(data)), data, StagedFileType.NonUFS);
		}

		//stage dependencies
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "dependencies", "include"));

		var dependencydlls = Directory.GetFiles(Path.Combine(ModuleDirectory, "dependencies"), "*.dll");
		var dependencylibs = Directory.GetFiles(Path.Combine(ModuleDirectory, "dependencies"), "*.lib");

        foreach (var  dll in dependencydlls)
		{
			RuntimeDependencies.Add(Path.Combine(binaryStagingDir, Path.GetFileName(dll)), dll, StagedFileType.NonUFS);
		}
        foreach (var  lib in dependencylibs)
		{
			PublicAdditionalLibraries.Add(lib);
        }


        PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
		);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"libcurl", //link against these modules so GDAL can use them.
				"SQLiteCore", //
				"LibTiff",
				"ZLib",
			}
		);
	}
}
