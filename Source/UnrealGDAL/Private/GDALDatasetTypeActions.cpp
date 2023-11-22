
#include "GDALDatasetTypeActions.h"
#include "GDALDataset.h"


UClass* FGDALDatasetTypeActions::GetSupportedClass() const
{
	return UGDALDataset::StaticClass();
}

FText FGDALDatasetTypeActions::GetName() const
{
	return INVTEXT("GDAL Dataset");
}

FColor FGDALDatasetTypeActions::GetTypeColor() const
{
	return FColor::Green;
}

uint32 FGDALDatasetTypeActions::GetCategories()
{
	return EAssetTypeCategories::Media;
}

bool FGDALDatasetTypeActions::IsImportedAsset() const
{
	return true;
}

