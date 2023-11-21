#pragma once

#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "GDALHeaders.h"
#include "proj.h"

DECLARE_LOG_CATEGORY_EXTERN(LogUnrealGDAL, Display, All);

//proj search paths. Not sure if PROJ expects this to live or not, hence why it's static.
static const FTCHARToUTF8 ProjShare = { FPaths::Combine(FPaths::ProjectDir(), "Binaries", "Data", "GDAL", "proj")};
static const char* const PJ_DIR[1] = { ProjShare.Get()};

class UNREALGDAL_API FUnrealGDALModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	//Configures the GDAL data directory and registers all GDAL drivers
	//This function must be called by any module that consumes the GDAL/OGR API and must be inlined under Windows due to DLL boundary issues
	FORCEINLINE void InitGDAL()
	{
		//Point GDAL to the path containing its runtime data files
		FString dataDir = FPaths::ConvertRelativePathToFull(FPaths::Combine(FPaths::ProjectDir(), "Binaries","Data", "GDAL", "gdal"));
		CPLSetConfigOption("GDAL_DATA", TCHAR_TO_UTF8(*dataDir));
		
		//Register all GDAL format drivers
		GDALAllRegister();
		
		//Install our GDAL error handler
		CPLSetErrorHandler(FUnrealGDALModule::GDALErrorHandler);

		
		//set PROJ search path
		proj_context_set_search_paths(PJ_DEFAULT_CTX, 1, PJ_DIR);
	}
	
	//Our custom error handler for logging errors emitted by GDAL
	static void GDALErrorHandler(CPLErr err, int num, const char* message);
private:
	TSharedPtr<class IAssetTypeActions> Action;
};
