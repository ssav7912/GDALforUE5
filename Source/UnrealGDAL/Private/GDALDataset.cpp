// Fill out your copyright notice in the Description page of Project Settings.


#include "GDALDataset.h"

#include "GDALHelpers.h"


UGDALDataset* UGDALDataset::MakeFromBuffer(const TArray<uint8>& Buffer, CPLErr& Err)
{

	GDALDataset* Dataset;
	VSILFILE* Handle;
	FString FileName;
	if (AttemptLoadFromBuffer(Buffer, FileName, Dataset, Handle) == CE_None)
	{
		UGDALDataset* NewDataset = NewObject<UGDALDataset>();
		NewDataset->UnderlyingFileName = FileName;
		NewDataset->UnderlyingFileHandle = Handle;
		NewDataset->UnderlyingBuffer = Buffer;
		NewDataset->Dataset = GDALDatasetRef(Dataset);
		//NewDataset->PostLoad();
		return NewDataset;
	}

	return nullptr;
}

void UGDALDataset::PostLoad()
{
	UObject::PostLoad();
	GDALDataset* NewDataset;
	VSILFILE* FileHandle;
	FString Empty;
	if (AttemptLoadFromBuffer(this->UnderlyingBuffer, Empty, NewDataset, FileHandle) == CPLErr::CE_None)
	{
		this->UnderlyingFileName = Empty;
		this->UnderlyingFileHandle = FileHandle;
		this->Dataset = GDALDatasetRef(NewDataset);
	}
	
}



void UGDALDataset::BeginDestroy()
{
	VSIFCloseL(this->UnderlyingFileHandle);
	VSIUnlink(StringCast<ANSICHAR>(*this->UnderlyingFileName).Get());
	UObject::BeginDestroy();
}

void UGDALDataset::PostDuplicate(bool bDuplicateForPIE)
{
	UObject::PostDuplicate(bDuplicateForPIE);

	GDALDataset* OutDataset;
	FString OutFilename;
	VSILFILE* OutFileHandle;
	
	if (AttemptLoadFromBuffer(this->UnderlyingBuffer, OutFilename, OutDataset, OutFileHandle) == CE_None)
	{
		this->UnderlyingFileHandle = OutFileHandle;
		this->UnderlyingFileName = OutFilename;
		this->Dataset = GDALDatasetRef(OutDataset);
	}
}


CPLErr UGDALDataset::AttemptLoadFromBuffer(TArray<uint8> Buffer, FString& Filename, GDALDataset*& OutDataset, VSILFILE*& OutFileHandle)
{
	if (Filename.IsEmpty())
	{
		Filename = "/vsimem/" + FGuid::NewGuid().ToString();
	}
	const char*	FilenameANSI = StringCast<ANSICHAR>(*Filename).Get();
	VSILFILE* Handle = VSIFileFromMemBuffer(FilenameANSI, Buffer.GetData(), Buffer.Num(), false);

	if (Handle)
	{
		GDALDataset* Opened = GDALDataset::Open(FilenameANSI, GDAL_OF_VERBOSE_ERROR | GDAL_OF_VECTOR | GDAL_OF_RASTER | GDAL_OF_GNM);
		CPLErr OutErr = CPLGetLastErrorType();

		if (OutErr == CPLErr::CE_None && Opened)
		{
			OutDataset = Opened;
			OutFileHandle = Handle;

			return CE_None;
		}
		else
		{
			VSIFCloseL(Handle);
			VSIUnlink(FilenameANSI);
			return OutErr;
		}
		
	}
	else
	{
		return CPLGetLastErrorType();
	}
}
