// Fill out your copyright notice in the Description page of Project Settings.


#include "GDALDatasetFactory.h"
#include "Logging/StructuredLog.h"
#include "UnrealGDAL.h"
#include "GDALDataset.h"

UGDALDatasetFactory::UGDALDatasetFactory()
{
	this->Formats.Append(GetDriverFileExtensions());
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

TArray<FString> UGDALDatasetFactory::GetDriverFileExtensions()
{
	TArray<FString> Drivers;
	int32 DriverCount = GDALGetDriverCount();
	for (int32 i = 0; i < DriverCount; i++)
	{
		GDALDriver* Driver = GDALDriver::FromHandle(GDALGetDriver(i));

		FString DriverExtension = Driver->GetMetadataItem(GDAL_DMD_EXTENSION);
		FString DriverShortName = GDALGetDriverLongName(Driver);

		if (!DriverExtension.IsEmpty() && !DriverShortName.IsEmpty())
		{
			FString DriverName = DriverExtension + FString{ ";" } + DriverShortName;
			Drivers.Add(DriverName);
		}

		
	}
	

	return Drivers;
}
