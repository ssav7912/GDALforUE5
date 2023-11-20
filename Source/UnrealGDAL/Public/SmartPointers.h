#pragma once

#include "GDALHeaders.h"
#include "HelperStructs.h"
#include "Templates/UniquePtr.h"

//Smart pointer type for affine transformation coefficients
typedef TUniquePtr<double[]> GeoTransformRef;

//Smart pointer types for our helper structs
typedef TUniquePtr<RasterCornerCoordinates> RasterCornerCoordinatesRef;
typedef TUniquePtr<RasterMinMax> RasterMinMaxRef;


struct VSILFILERef
{
	VSILFILERef() = delete;
	VSILFILERef(const TArray<uint8>& Buffer)
	{
		VSIFileName = CPLString("/vsimem/") + StringCast<ANSICHAR>(*FGuid::NewGuid().ToString()).Get();
		FileHandle = VSIFileFromMemBuffer(VSIFileName, const_cast<GByte*>(Buffer.GetData()), Buffer.Num(), false);
	}
	VSILFILERef(const char* Filename, VSILFILE* Handle) : VSIFileName(Filename), FileHandle(Handle) {}

	
	CPLString VSIFileName;
	VSILFILE* FileHandle;

	~VSILFILERef()
	{
		
		VSIFCloseL(FileHandle);
		VSIUnlink(VSIFileName);
	}
};

using OGRGeometryRef = TUniquePtr<OGRGeometry, mergetiff::_CustomDeleterBaseT<OGRGeometry, OGRGeometryFactory::destroyGeometry>>;
using OGRFeatureRef = TUniquePtr<OGRFeature, mergetiff::_CustomDeleterBaseT<OGRFeature, OGRFeature::DestroyFeature>>;


//Alias the smart pointer types provided by mergetiff
typedef mergetiff::CPLStringRef CPLStringRef;
typedef mergetiff::GDALDatasetRef GDALDatasetRef;
typedef mergetiff::OGRCoordinateTransformationRef OGRCoordinateTransformationRef;
typedef mergetiff::GDALInfoOptionsRef GDALInfoOptionsRef;
typedef mergetiff::GDALTranslateOptionsRef GDALTranslateOptionsRef;
typedef mergetiff::GDALWarpAppOptionsRef GDALWarpAppOptionsRef;
typedef mergetiff::GDALVectorTranslateOptionsRef GDALVectorTranslateOptionsRef;
typedef mergetiff::GDALDEMProcessingOptionsRef GDALDEMProcessingOptionsRef;
typedef mergetiff::GDALNearblackOptionsRef GDALNearblackOptionsRef;
typedef mergetiff::GDALGridOptionsRef GDALGridOptionsRef;
typedef mergetiff::GDALRasterizeOptionsRef GDALRasterizeOptionsRef;
typedef mergetiff::GDALBuildVRTOptionsRef GDALBuildVRTOptionsRef;
#if GDAL_VERSION_NUM >= GDAL_COMPUTE_VERSION(3,1,0)
typedef mergetiff::GDALMultiDimInfoOptionsRef GDALMultiDimInfoOptionsRef;
typedef mergetiff::GDALMultiDimTranslateOptionsRef GDALMultiDimTranslateOptionsRef;
#endif
