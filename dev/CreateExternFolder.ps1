Param
(    
    [parameter(Mandatory=$true)]
    [String]
    $outputDir
)


#Copy include
$source = "${PSScriptRoot}/../source/Client"
$destination = "$outputDir/include"
New-Item -ItemType Directory -Path $destination
Copy-Item $source\*.h $destination

# Copy bin
$source = "${PSScriptRoot}/../Bin"
$destination = "$outputDir/bin"
New-Item -ItemType Directory -Path $destination
Copy-Item $source\"BugTrapU-x64.dll" $destination
Copy-Item $source\"BugTrapU-x64.pdb" $destination

# Copy lib
$source = "${PSScriptRoot}/../Bin"
$destination = "$outputDir/lib"
New-Item -ItemType Directory -Path $destination
Copy-Item $source\"BugTrapU-x64.lib" $destination