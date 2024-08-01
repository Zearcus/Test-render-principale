# build.ps1
# PowerShell script to compile FontMaker.cpp using the Microsoft C++ Compiler (cl.exe)

# Function to find cl.exe path based on Visual Studio installation
function FindCLPath {
    $vsInstallDirs = @(
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Tools\MSVC\14.29.30133\bin\Hostx64\x64\cl.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC\14.40.33807\bin\Hostx64\x64\cl.exe"
    )

    foreach ($dir in $vsInstallDirs) {
        $clPath = Get-Item $dir -ErrorAction SilentlyContinue
        if ($clPath) {
            return $clPath.FullName
        }
    }
    return $null
}

# Function to find vcvarsall.bat path based on Visual Studio installation
function FindVSVarsPath {
    $vsVarsDirs = @(
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat",
        "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"
    )

    foreach ($dir in $vsVarsDirs) {
        $varsPath = Get-Item $dir -ErrorAction SilentlyContinue
        if ($varsPath) {
            return $varsPath.FullName
        }
    }

    return $null
}

# Find cl.exe path
$clExePath = FindCLPath

# Check if cl.exe path was found
if (-not $clExePath) {
    Write-Host "cl.exe not found. Ensure Visual Studio with C++ tools is installed."
    Exit 1
}

# Find vcvarsall.bat path
$vcVarsPath = FindVSVarsPath

# Check if vcvarsall.bat path was found
if (-not $vcVarsPath) {
    Write-Host "vcvarsall.bat not found. Ensure Visual Studio with C++ tools is installed."
    Exit 1
}

# Set up the environment for Visual Studio tools
& "$vcVarsPath" x64

# Check if INCLUDE environment variable is set
if (-not $env:INCLUDE) {
    Write-Host "INCLUDE environment variable is not set. Ensure the environment is properly configured."
    Exit 1
}

# Variables for the build
$sourceFile = "FontMaker.cpp"
$outputFile = "PNGCreator.exe"
$includePath = "C:\Users\aleksi\Documents\font_maker\freetype-2.13.2\include"
$libPath = "C:\Users\aleksi\Documents\font_maker\freetype-2.13.2\lib"
$additionalLibs = "freetype.lib"

# Compile command as arguments
$compileArgs = @(
    "/EHsc",
    "/I", "`"$includePath`"",
    "/Fe`"$outputFile`"",
    "`"$sourceFile`"",
    "/link",
    "/LIBPATH:`"$libPath`"",
    "`"$additionalLibs`""
)

# Print compile command for debugging
Write-Host "Compile Command: $clExePath $compileArgs"

# Invoke compilation
& $clExePath $compileArgs

# Check if compilation was successful
if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful. Output file: $outputFile"
} else {
    Write-Host "Compilation failed with error code $LASTEXITCODE."
}
