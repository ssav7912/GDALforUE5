#include "Misc/AutomationTest.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"
#include "GDALDatasetFactory.h"
#include "GDALDataset.h"

class UGDALDatasetHelper
{
	
public:
	static bool DatasetImportTestHelper(FAutomationTestBase* TestHarness, const FString& FilePath)
	{
		UGDALDatasetFactory* Factory = NewObject<UGDALDatasetFactory>();
		bool OutCancelled;
		UObject* Created = Factory->ImportObject(UGDALDataset::StaticClass(), GetTransientPackage(), "AUS_GeoBuf", RF_Transient, FilePath, nullptr, OutCancelled);

		TestHarness->TestNotNull(TEXT("Factory Creates Object from file path"), Created);
		UGDALDataset* Dataset = Cast<UGDALDataset>(Created);
		TestHarness->TestNotNull(TEXT("Created Object is a UGDALDataset"), Dataset);
		if (Dataset)
		{
			TestHarness->TestNotNull(TEXT("Underlying dataset is not null"), Dataset->Dataset.Get());
			TestHarness->TestNotNull(TEXT("Underlying VSI File Pointer is not null"), Dataset->UnderlyingFileHandle);
			return true;

		}
		else {
			return false;
		}

	}
};
	
IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGDALDatasetLoadGeoBuf, "UnrealGDAL.UGDALDataset.LoadGeoBuf",
                                 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool UGDALDatasetLoadGeoBuf::RunTest(const FString& Parameters)
{
	FString Filepath = FPaths::Combine(FPaths::ProjectPluginsDir(), "UnrealGDAL", "Resources", "testfiles", "AUS");
	FString FlatGeoBuf = FPaths::Combine(Filepath, "AUS_GeoBuf.fgb");

		
	// Make the test pass by returning true, or fail by returning false.
	return UGDALDatasetHelper::DatasetImportTestHelper(this, FlatGeoBuf);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGDALDatasetLoadGeoJson, "UnrealGDAL.UGDALDataset.LoadGeoJson",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool UGDALDatasetLoadGeoJson::RunTest(const FString& Parameters)
{
	FString Filepath = FPaths::Combine(FPaths::ProjectPluginsDir(), "UnrealGDAL", "Resources", "testfiles", "AUS");
	FString GeoJson = FPaths::Combine(Filepath, "AUS_GeoJson.geojson");

		
	// Make the test pass by returning true, or fail by returning false.
	return UGDALDatasetHelper::DatasetImportTestHelper(this, GeoJson);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGDALDatasetLoadGeoTif, "UnrealGDAL.UGDALDataset.LoadGeoTif", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool UGDALDatasetLoadGeoTif::RunTest(const FString& Parameters)
{
	FString Filepath = FPaths::Combine(FPaths::ProjectPluginsDir(), "UnrealGDAL", "Resources", "testfiles");
	FString GeoTif = FPaths::Combine(Filepath, "us_noaa_vertconw.tif");

	return UGDALDatasetHelper::DatasetImportTestHelper(this, GeoTif);
}