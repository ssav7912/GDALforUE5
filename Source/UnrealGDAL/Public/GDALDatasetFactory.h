// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GDALDatasetFactory.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGDAL_API UGDALDatasetFactory : public UFactory
{
	//friend class UGDALDatasetLoadGeoBuf; //for unit tests
	
	GENERATED_BODY()
	UGDALDatasetFactory();
	
	
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
};
