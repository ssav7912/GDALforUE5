# UnrealGDAL: Unreal Engine GDAL plugin

This is a fork of TensorWork's UnrealGDAL Plugin, which upgrades the plugin to contemporary (UE5.3) engine versions and simplifies the dependency chain (removing ue4cli and ue4-conan) to make incorporating GDAL source builds easier.

This plugin provides access to the [GDAL/OGR](https://gdal.org/) C++ API inside the Unreal Engine, allowing Unreal projects and plugins to easily import and manipulate geospatial data. In addition to providing access to the full GDAL/OGR C++ API surface, the plugin bundles the header-only [mergetiff](https://github.com/adamrehn/mergetiff-cxx) library for providing [convenient smart pointer types](./Source/UnrealGDAL/Public/SmartPointers.h), and includes [interoperability functionality](./Source/UnrealGDAL/Public/GDALHelpers.h) for accessing the GDAL/OGR API using native Unreal Engine datatypes.

## Contents

- [Installation](#installation)
  - [Using release binaries](#using-release-binaries)
  - [Building everything from source](#building-everything-from-source)
- [Usage](#usage)
- [Legal](#legal)


## Installation
Clone the repository to your Plugins directory. 


### Using release binaries

You can download release binaries for the plugin from the releases page, so you can simply copy the plugin directory into your project or Unreal Engine installation and start using it immediately. Release binaries are currently provided for 64-bit versions of Windows.

### Building everything from source
This plugin is effectively a container for linking against GDAL, and no longer has any built-in rules for a GDAL source build. Instead, you are expected to build GDAL separately and install the binaries to the root directory of the GDAL module (Where `GDAL.build.cs` is contained).

GDAL should produce the following folders in your specified install directory (`CMAKE_INSTALL_PREFIX`):
```
bin/
include/
lib/
share/
```
Copy these directly into the GDAL module directory (or even tell CMAKE to install them there).

Any dependencies should be placed into `dependencies/`, with includes in `dependencies/include`. The number of dependencies will
depend on your specific source build. The distributed build ships with PROJ, GEOS, NetCDF, libtiff, webp, zlib, libcurl, lerc and jpeg62, sourced from OSGEO.


## Usage

**Step 1: List the UnrealGDAL plugin as a dependency.** In the descriptor file (`.uproject` or `.uplugin`) for any project or plugin which will consume the GDAL/OGR API, add the following lines:

```json
"Plugins": [
  {
    "Name": "UnrealGDAL",
    "Enabled": true
  }
]
```

**Step 2: List the GDAL and UnrealGDAL modules as build dependencies.** In the module rules file (`.Build.cs`) for any C++ code module which will consume the GDAL/OGR API, add the following lines:

```csharp
PublicDependencyModuleNames.AddRange(
  new string[]
  {
    "GDAL",
    "UnrealGDAL"
  }
);
```

**Step 3: Ensure the UnrealGDAL module is loaded at startup and GDAL is correctly configured.** In the source file containing the [StartupModule()](https://docs.unrealengine.com/en-US/API/Runtime/Core/Modules/IModuleInterface/StartupModule/index.html) method of the module implementation class for any C++ code module which will consume the GDAL/OGR API, add the following lines:

```cxx
//Add this line at the top of the source file
#include "UnrealGDAL.h"

//Add these lines to your StartupModule() method
void YourModuleNameGoesHere::StartupModule()
{
  FUnrealGDALModule* UnrealGDAL = FModuleManager::Get().LoadModulePtr<FUnrealGDALModule>("UnrealGDAL");
  UnrealGDAL->InitGDAL();
}
```

This will ensure the UnrealGDAL module is loaded into memory, all GDAL drivers are registered and GDAL is able locate its bundled data files, which are necessary for certain functionality (e.g. manipulating spatial reference systems) to work correctly. **This code must be added to every C++ code module which will consume the GDAL/OGR API in order to ensure correct behaviour.**


## Legal

Copyright &copy; 2020, TensorWorks Pty Ltd. Licensed under the MIT License, see the file [LICENSE](./LICENSE) for details.

Binary releases of this plugin contain the following third-party libraries, which are covered by their respective licenses:

- [mergetiff-cxx](https://github.com/adamrehn/mergetiff-cxx) ([MIT License](https://github.com/adamrehn/mergetiff-cxx/blob/master/LICENSE))
- [GDAL](https://gdal.org/) ([X/MIT License](https://github.com/OSGeo/gdal/blob/master/gdal/LICENSE.TXT))
- [GEOS](https://trac.osgeo.org/geos) ([LGPL-2.1 License](https://github.com/libgeos/geos/blob/master/COPYING))
- [PROJ](https://proj.org/) ([X/MIT License](https://github.com/OSGeo/PROJ/blob/master/COPYING))
- [netCDF](https://www.unidata.ucar.edu/software/netcdf/) ([2018 Unidata](https://docs.unidata.ucar.edu/netcdf-c/current/copyright.html))
- [libtiff](http://www.libtiff.org/) ([Sam Leffler & SGI] (http://www.libtiff.org/misc.html))
- [webp](https://developers.google.com/speed/webp) ([Google](https://www.webmproject.org/license/software/))
- [zlib](https://www.zlib.net/manual.html) ([Jean-loup Gailly & Mark Adler](https://www.zlib.net/manual.html))
- [libcurl](https://curl.se/libcurl/) ([curl license](https://curl.se/docs/copyright.html))
- [Lerc](https://github.com/Esri/lerc) ([Apache 2.0](https://github.com/Esri/lerc/blob/master/LICENSE))
- [jpeg62](https://gnuwin32.sourceforge.net/packages/jpeg.htm) ([Independent JPEG Group License] (https://spdx.org/licenses/IJG.html))

GDAL and PROJ are linked statically, whereas GEOS is linked dynamically due to the [static linking requirements of the LGPL License](https://www.gnu.org/licenses/gpl-faq.en.html#LGPLStaticVsDynamic) and mergetiff is a header-only library. 