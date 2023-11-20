// Fill out your copyright notice in the Description page of Project Settings.


#include "GDALDatasetFactory.h"

#include "GDALDataset.h"

UGDALDatasetFactory::UGDALDatasetFactory()
{
	this->Formats.Add("fgb; FlatGeoBuf");
	this->Formats.Add("geojson; GeoJSON");
}

UObject* UGDALDatasetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	TArray<uint8> Bytes;
	FFileHelper::LoadFileToArray(Bytes, *Filename);
	if (!Bytes.IsEmpty())
	{
		FString VSIFile = "/vsimem/" + FPaths::GetBaseFilename(Filename);
		CPLErr OutErr;
		
		if (auto Data = UGDALDataset::MakeFromBuffer(Bytes, OutErr))
		{
			Data->SourceFilePath = Filename;
			return Data;
		}
	}

	return nullptr;

}
