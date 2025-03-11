// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartPointers.h"
#include "UObject/Object.h"
#include "GDALDataset.generated.h"

/**
 * Thin wrapper for a GDALDataset asset. This exists to allow for in-engine serialisation and management of Dataset files.
 */
UCLASS()
class UNREALGDAL_API UGDALDataset : public UObject
{
	friend class UGDALDatasetFactory;
	friend class UGDALDatasetHelper; //unit testing.
	
	GENERATED_BODY()

	virtual void PostLoad() override;
	virtual void BeginDestroy() override;
	virtual void PostDuplicate(bool bDuplicateForPIE) override;
	
	/*
	 * Returns a non-owning pointer to the underlying dataset object.
	 * You should not call Close() on this! This will be handled when the GC cleans up the wrapping UObject.
	 */
	GDALDataset* GetDataset() const { return Dataset.Get(); }

	/*
	 * Creates a Transient Dataset from a memory buffer.
	 * Returns Null on failure. 
	 */
	static UGDALDataset* MakeFromBuffer(const TArray<uint8>& Buffer, CPLErr& Err);
	
private:
	static CPLErr AttemptLoadFromBuffer(TArray<uint8> Buffer, FString& Filename, GDALDataset*& OutDataset, VSILFILE*& OutFileHandle);

	//The name of the file in the VSI file system. Not necessarily equal to the datasets name on-disk.
	UPROPERTY()
	FString UnderlyingFileName;

	UPROPERTY()
	FString SourceFilePath;

	//The handle used to interact with the dataset buffer in the virtual file system.
	//We really only need to hold onto this so we can close it when required.
	VSILFILE* UnderlyingFileHandle;
	
	UPROPERTY()
	TArray<uint8> UnderlyingBuffer;
	
	GDALDatasetRef Dataset;
};
