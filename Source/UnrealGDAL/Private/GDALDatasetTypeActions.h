#pragma once

#include "Internationalization/Text.h"
#include "Math/Color.h"
#include "UObject/Class.h"
#include "AssetTypeActions_Base.h"


class FGDALDatasetTypeActions : public FAssetTypeActions_Base
{
	virtual UClass* GetSupportedClass() const override;
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	virtual bool IsImportedAsset() const override;
};