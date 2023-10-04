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

The engine bundles `PROJ` and `sqlite3` as a module already, and you can tell CMAKE to link against it by pointing it to the engine path:
```
-DPROJ_LIBRARY_RELEASE = Path/To/Engine/Install/Engine/Plugins/Runtime/GeoReferencing/Source/ThirdParty/vcpkg-installed/overlay-x64-windows/lib/proj.lib
-DPROJ_INCLUDE_DIR =  Path/To/Engine/Install/Engine/Plugins/Runtime/GeoReferencing/Source/ThirdParty/vcpkg-installed/overlay-x64-windows/include
-DSQLite3_LIBRARY = Path/To/Engine/Install/Engine/Plugins/Runtime/GeoReferencing/Source/ThirdParty/vcpkg-installed/overlay-x64-windows/lib/sqlite3.lib
-DSQLite3_INCLUDE_DIR = Path/To/Engine/Install/Engine/Plugins/Runtime/GeoReferencing/Source/ThirdParty/vcpkg-installed/overlay-x64-windows/include
``` 
Additionally, GDAL should be built as a Shared Lib: `-DBUILD_SHARED_LIBS=true`.

GDAL should produce the following folders in your specified install directory (`CMAKE_INSTALL_PREFIX`):
```
bin/
include/
lib/
share/
```
Copy these directly into the GDAL module directory (or even tell CMAKE to install them there).

The engine bundled version of SQLite3 does not have the RTree extension, and so `-DOGR_ENABLE_DRIVER_SQLITE` should be `false` so that SQLite datasets (e.g. GPKG) don't attempt to use it. Link against your own build of SQLite to enable this driver. 


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

GDAL and PROJ are linked statically, whereas GEOS is linked dynamically due to the [static linking requirements of the LGPL License](https://www.gnu.org/licenses/gpl-faq.en.html#LGPLStaticVsDynamic) and mergetiff is a header-only library. The source code for all of these libraries is bundled with binary releases of the plugin under the [ThirdParty](./ThirdParty) directory.
