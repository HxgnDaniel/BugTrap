
Write-Host "Build all Configurations"


$solution = "${PSScriptRoot}/../source/BugTrap.vs2022.sln"
$configuration = "Unicode Release"
$platform = "x64"

msbuild $solution /t:Rebuild /p:Configuration="$configuration" /p:Platform="$platform"

$outputDir = "${PSScriptRoot}/../output"

Remove-Item -Recurse -Force $outputDir -Confirm:$false
New-Item -ItemType Directory -Path $outputDir

. "${PSScriptRoot}\CreateExternFolder.ps1" -outputDir $outputDir


if($LASTEXITCODE -ne 0) {
    Write-Error "Error in Build"
    exit $LASTEXITCODE
}