cd pk7
C:\stuff\utils\7z\7za.exe a %2 ..\Lithium.%1 .\* -xr!*.gitignore -xr!*.bat -xr!*.dbs -xr!*.wad.b*

cd ..\pk7_extras
C:\stuff\utils\7z\7za.exe a %2 ..\Lithium_Extras.%1 .\*

cd ..\pk7_damagebob
C:\stuff\utils\7z\7za.exe a %2 ..\Lithium_DamageBob.%1 .\*
