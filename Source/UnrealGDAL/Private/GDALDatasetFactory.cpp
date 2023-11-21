// Fill out your copyright notice in the Description page of Project Settings.


#include "GDALDatasetFactory.h"
#include "Logging/StructuredLog.h"
#include "UnrealGDAL.h"
#include "GDALDataset.h"

UGDALDatasetFactory::UGDALDatasetFactory()
{
	this->Formats.Add("fgb; FlatGeoBuf");
	this->Formats.Add("geojson; GeoJSON");
	this->Formats.Add("gtiff; GeoTiff");
	this->Formats.Add("gtif; GeoTiff");
	this->SupportedClass = UGDALDataset::StaticClass();
	this->bCreateNew = false;
	this->bEditorImport = true;
}

UObject* UGDALDatasetFactory::FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled)
{
	TArray<uint8> Bytes;
	FFileHelper::LoadFileToArray(Bytes, *Filename);
	if (!Bytes.IsEmpty())
	{
		FString VSIFile = "/vsimem/" + FPaths::GetBaseFilename(Filename);

		GDALDataset* Dataset;
		VSILFILE* Handle;
		FString FileName;
		if (UGDALDataset::AttemptLoadFromBuffer(Bytes, FileName, Dataset, Handle) == CE_None)
		{
			UGDALDataset* NewDataset = NewObject<UGDALDataset>(InParent, InClass, InName, Flags);
			NewDataset->UnderlyingFileName = FileName;
			NewDataset->UnderlyingFileHandle = Handle;
			NewDataset->UnderlyingBuffer = Bytes;
			NewDataset->Dataset = GDALDatasetRef(Dataset);
			//NewDataset->PostLoad();
			NewDataset->SourceFilePath = Filename;
			return NewDataset;
		}

		
	}
	const char* OutError = CPLGetLastErrorMsg();
	UE_LOGFMT(LogUnrealGDAL, Error, "UGDALDatasetFactory: Could not import dataset {0}, GDAL reports: {1}", InName, OutError);
	return nullptr;

}

bool UGDALDatasetFactory::FactoryCanImport(const FString& Filename)
{

	FTCHARToUTF8 FileString { Filename };

	GDALDatasetRef Dataset { GDALDataset::Open(FileString.Get(), GDAL_OF_VECTOR | GDAL_OF_RASTER | GDAL_OF_READONLY | GDAL_OF_GNM) };

	if (Dataset.IsValid())
	{
		return true;
	}
	return false;
}
