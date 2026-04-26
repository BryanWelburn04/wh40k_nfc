# wh40k_nfc
Warhammer/Skylanders hybrid

-- COMPILE AND RUN -- UPDATED!
cmake --build . ; .\wh40k_nfc.exe

-- ACR112U SDK SETUP --
-SDK is included on the USB stick
-Copy files locally then eject USB
-Go to SDK\ACR122U-sdk-v2.00\ACR122U SDK v2.00 and run autorun.exe
-Probably good idea to read through "getting start" section
-Then:
    -install smart card reader driver (defult loc)
    -install SDK components (defult loc)
    -(smart card reader tool didn't work for me, app couldn't launch)
    -(didn't bother with Adobe PDF viewer)
-ACR122U SDK uses Windows’ built-in Smart Card API, (no extra library is required.) so  only <winscard.h> is needed

-- DOCUMENTATION AND SAMPLE CODE --
Will be under C:\Program Files (x86)\Advanced Card Systems Ltd\ACR122U NFC SDK (I also added them to repo)

-- QT --
Using QT Community 6.11.0 
Set up steps:
    - Download from...https://www.qt.io/development/download-qt-installer-oss
    - Set system variable path for C:\Qt\6.11.0\mingw_64\bin
    - Set as dev kit in VS code (cntl + shift + p -> Cmake: Select a kit -> GCC...mingw_64)
    - Reconfigure (cntl + shift + p -> Cmake: Configure)
    - Rebuild (cntl + shift + p -> Cmake: Build)
