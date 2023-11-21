#include "UnrealGDAL.h"
#include "GDALHelpers.h"
#include "Logging/StructuredLog.h"
#include "GDALDatasetTypeActions.h"

#define LOCTEXT_NAMESPACE "FUnrealGDALModule"

DEFINE_LOG_CATEGORY(LogUnrealGDAL);

void FUnrealGDALModule::StartupModule() {
	this->InitGDAL();

	Action = MakeShareable(new FGDALDatasetTypeActions());
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
	

}

void FUnrealGDALModule::ShutdownModule() {
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.UnregisterAssetTypeActions(Action.ToSharedRef());
	}
	
}

void FUnrealGDALModule::GDALErrorHandler(CPLErr err, int num, const char* message) {
	
	switch (err)
	{
		//narrowing conversion trips up intellisense, but is (allegedly) safe, and also
		//not my problem. (Epic Games pls fix).
		case CE_None: UE_LOGFMT(LogUnrealGDAL, Display, "{0}", message); return;
		case CE_Debug: UE_LOGFMT(LogUnrealGDAL, Warning, "{0}", message); return; 
		case CE_Warning: UE_LOGFMT(LogUnrealGDAL, Warning, "{0}", message); return;
		case CE_Failure: UE_LOGFMT(LogUnrealGDAL, Error, "{0}", message); return; 
		case CE_Fatal: 	UE_LOGFMT(LogUnrealGDAL, Error, "{0}", message); return;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealGDALModule, UnrealGDAL)