;NASM include file version 1.2 by Tamas Kaproncai [tomcat@szif.hu]

;Kodak Digital Camera DC220/DC260/DC265 Software Development Kit
;AUTHORS: Takahisa Amada, Eastman Kodak Japan R&D Center
;CREATION DATE: Aug./22/97, LAST MODIFICATION: Mar./25/99
;Version 1.0.0600

;Error Code Summary

SC_NoErr                      EQU     0 ;Normal successful completion.
SC_BadArgErr                  EQU -1000 ;Invalid call parameter found.
SC_CameraNotConnected         EQU -1002 ;Attempted to open a camera not connected to the host.
SC_CameraFunctionNotSupported EQU -1003 ;This functionality is not support- ed by the current opened camera type.
SC_CancelCallback             EQU -1004 ;Operation terminated by callback function.
SC_CommunicationErr           EQU -1006 ;Error communicating with the camera.
SC_ImageProcessErr            EQU -1009 ;Error occurred during image processing.
SC_MallocErr                  EQU -1011 ;Unable to allocate required memory.
SC_NotOpenErr                 EQU -1013 ;Attempted to use camera or driver before it was opened.
SC_ReOpenErr                  EQU -1016 ;Unable to open because it is already open. Please close first.
SC_SDKVersionErr              EQU -1019 ;Compatibility error while opening the driver.
SC_SystemErr                  EQU -1020 ;General low-level operating system error found.
SC_MemoryFullErr              EQU -1022 ;The camera is unable to take picture because the image memory is full.
SC_CardNotInserted            EQU -1027 ;Memory card is not inserted to the camera.
SC_NotNativeFile              EQU -1028 ;The specified image is not created by DC220 or DC260.

%DEFINE OSErr WORD

;Definition of Constants

SC_SDKVersion EQU 0x0106
DC265_SDKVersion EQU 0x0122
;This symbol defines valid versions of SDK which can be used with this
;include file.

SC_CamIDLength EQU (31+1)
;This symbol defines valid length of CamId which should be allocated
;in SC_Camera and SC_Config structures.

;Generic Data Structures

STRUC SC_Camera
.PortNum         RESD 1 ;Try to connect to a camera on the port indexed by this value. The types of ports available on a system will vary. This field should be initialized before calling SC_OpenCamera.
.BitRate         RESD 1 ;Try to connect to a camera with this value. This field should be initialized before calling SC_OpenCamera. The SDK also sets this field with actual bit rate to the camera.
.CamType         RESD 1 ;The type of camera opened. This field should be initialized before calling SC_OpenCamera.
.DeviceType      RESD 1 ;The connection device type. This field should be initialized to one of the value of SC_DeviceType before calling SC_OpenCamera.
.CameraOpenFlag  RESD 1 ;SC_OpenCamera sets TRUE and SC_CloseCamera sets FALSE.
.LatestError     RESD 1 ;The latest error in the SDK is kept in 4 bytes value. The high 2 bytes means the function number that is T.B.D. The low 2 bytes means the error number that is T.B.D.
.InternalUseOnly RESD 1 ;The application should never access this field. The SDK library stores camera-specific internal data in it.
ENDSTRUC
%DEFINE SC_CameraPtr DWORD
;SC_Camera contains information specific to an open camera. It is used
;to identify which camera should be used.

%DEFINE SC_CameraType DWORD
%DEFINE SC_CameraTypePtr DWORD
;Returned from the SDK to the application after calling SC_OpenCamera.
;Indicates the current camera type. This field should be initialized to
;a valid SC_CamType before calling SC_OpenCamera.

SC_CameraTypeAuto EQU 0   ;If this value is set by the application before calling SC_OpenCamera, the SDK automatically obtain the actual camera type and returns it in SC_Camera.CamType.
SC_DC260          EQU 110 ;The camera is a Kodak SCMain.
SC_DC220          EQU 111 ;The camera is a Kodak SCHigh.
SC_DC265          EQU 112 ;The camera is a Kodak DC265.

%DEFINE SC_PortNum DWORD
;SC_PortNum is used by SC_OpenCamera to connect to the camera.
;The application needs to set one of these when it calls SC_OpenCamera.
;If the application sets SC_BitRateAuto, the SDK returns the actual
;port number when it connects to the camera. In the case of IrDA
;communication, the SDK returns the virtual COM port number.
;In the case of USB, the SDK always returns SC_PortNumAuto.

SC_PortNumAuto EQU 0 ;If this value is set by the application before calling SC_OpenCamera, the SDK searches the communication port for the camera automatically. Then returns it in SC_Camera.PortNum.
SC_PortNum1    EQU 1 ;Number port is 1.
SC_PortNum2    EQU 2 ;Number port is 2.
SC_PortNum3    EQU 3 ;Number port is 3 that is only for Windows.
SC_PortNum4    EQU 4 ;Number port is 4 that is only for Windows.
SC_PortNum5    EQU 5 ;Number port is 5 that is only for Windows.
SC_PortNum6    EQU 6 ;Number port is 6 that is only for Windows.
SC_PortNum7    EQU 7 ;Number port is 7 that is only for Windows.
SC_PortNum8    EQU 8 ;Number port is 8 that is only for Windows.
SC_PortNum9    EQU 9 ;Number port is 9 that is only for Windows.

%DEFINE SC_BitRate DWORD
;SC_BitRate is used by SC_OpenCamera to connect to the camera.
;The application needs to set one of these when it calls SC_OpenCamera.
;If the application sets SC_BitRateAuto, the SDK returns the actual bit
;rate when it connects to the camera. For IrDA and USB communications,
;the SDK always returns SC_BitRateAuto when it connects to the camera.

SC_BitRateAuto    EQU 0     ;If this value is set by the application before calling SC_OpenCamera, the SDK searches the maximum communication baud rate for the camera automatically and returns it in SC_Camera.BitRate.
SC_BitRate9600    EQU 96    ;Communication bit rate is 9600bps.
SC_BitRate19200   EQU 192   ;Communication bit rate is 19200bps.
SC_BitRate38400   EQU 384   ;Communication bit rate is 38400bps.
SC_BitRate57600   EQU 576   ;Communication bit rate is 57600bps.
SC_BitRate115200  EQU 1152  ;Communication bit rate is 115200bps.

%DEFINE SC_DeviceType DWORD
;SC_DeviceType is used by SC_OpenCamera to connect to the camera.
;The application needs to set one of these when it calls SC_OpenCamera.
;If the application sets SC_AutoDevice, the SDK returns the actual
;device type when it connects to the camera.

SC_AutoDevice   EQU 0 ;If the application sets this value, the SDK automatically searches the device type. When the SDK connects to the camera, it returns the device type to the application by using DeviceType of SC_Camera.
SC_SerialDevice EQU 1 ;The communication is established via serial device to the camera.
SC_IrDADevice   EQU 2 ;The communication is established via IrDA device to the camera.
SC_USBDevice    EQU 3 ;The communication is established via USB device to the camera.

%DEFINE SC_ProgressStatus DWORD
;SC_ProgressStatus is used by the SC_ProgressCB functions to describe
;the call status.

SC_ProgressInit       EQU 0 ;Tells the application to create a status indicator display.
SC_ProgressUpdate     EQU 1 ;Tells the application to update the status indicator display.
SC_ProgressEnd        EQU 2 ;Tells the application to close the status indicator display.
SC_ProgressIncomplete EQU 3 ;Tells the application to terminate incomplete- ly and to close the status indicator display.

%DEFINE SC_ProgressProc DWORD
;SC_ProgressProc is a function pointer used as a callback to inform
;the application of the progress of the current operation.
;Typical usage of this function type is to display the indicator.

%MACRO SC_ProgressProcStart 0
PUSH EBP
MOV EBP,ESP
%ENDMACRO

%MACRO SC_ProgressProcEnd 0
POP EBP
RETN 12
%ENDMACRO

%DEFINE SC_ProgressProc.Status EBP+8
%DEFINE SC_ProgressProc.PercentComplete EBP+12
%DEFINE SC_ProgressProc.RefCon EBP+16

STRUC SC_ProgressCB
.ProgFunc RESD 1 ;Pointer to the progress callback function. If progress information is not needed, this pointer can be set to NULL.
.RefCon   RESD 1 ;Reference constant that will be passed to the ProgFunc. This is useful in multithreaded applications to identify the thread that made the call.
ENDSTRUC
%DEFINE SC_ProgressCBPtr DWORD
;SC_ProgressCB is a structure and defines a callback routine that
;can be used by SC_GetNativePicture, SC_ProcNativePicture,
;SC_GetCameraAudio, and SC_SetNativePicture.

STRUC SC_ImageIOCB
.Buffer     RESD 1 ;Pointer to a buffer to use for I/O operation.
.BufferSize RESD 1 ;Number of bytes to transfer.
ENDSTRUC
%DEFINE SC_ImageIOCBPtr DWORD
;SC_ImageIOCB is used by SC_GetNativePicture, SC_SetNativePicture,
;SC_GetNativePictInfo, SC_ProcNativePicture, SC_GetCameraAudio, and
;SC_GetNativeAudio, and tells the SDK the address and size of image
;I/O buffer.

STRUC SC_Status
.FirmwareVersion  RESD 1 ;Version of camera firmware.
.NumPictTaken     RESW 1 ;Number of pictures in the camera.
.NumPictRemaining RESW 1 ;Number of pictures that can still be taken and stored in the camera.
.ThumbWidth       RESW 1 ;Width of a thumbnail if the current camera settings are used.
.ThumbHeight      RESW 1 ;Height of a thumbnail if the current camera settings are used.
.PictWidth        RESD 1 ;Width of a picture if the current camera settings are used.
.PictHeight       RESD 1 ;Height of a picture if the current camera settings are used.
.NumDirectory     RESW 1 ;Number of derectories under the camera root directory. It indicates every combinations of the directory.
.NumSystemFile    RESW 1 ;Number of files under the camera system directory.
ENDSTRUC
%DEFINE SC_StatusPtr DWORD
;SC_Status contains status information that is common to general
;digital cameras and is used by SC_GetStatus.

STRUC SC_Config
.CamId RESD 1 ;A pointer to a text string that identifies the camera. This string must be allocated by the application, and it must be at least SC_CamIDLength bytes in length.
ENDSTRUC
%DEFINE SC_ConfigPtr DWORD
;SC_Config contains generic configuration information common to general
;digital cameras and is used by SC_GetConfig and SC_SetConfig.
;Most configuration fields are camera specific and are included in the
;camera-specific data structures.

STRUC SC_Time
.tm_sec  RESW 1 ;Seconds after the minute[0..59]
.tm_min  RESW 1 ;Minutes after the hour[0..59]
.tm_hour RESW 1 ;Hours since midnight[0..23]
.tm_mday RESW 1 ;Day of the month[1..31]
.tm_mon  RESW 1 ;Months since January[1..12]
.tm_year RESW 1 ;Years since 1900.
.tm_wday RESW 1 ;Days since Sunday[0..6].
ENDSTRUC
%DEFINE SC_TimePtr DWORD
;SC_Time is used to set and get the camera time and date.

STRUC SC_PictInfo
.PictName        RESB 256 ;Picture name of the image in the camera.
.ThumbWidth      RESW 1 ;Thumbnail width (pixels)
.ThumbHeight     RESW 1 ;Thumbnail height (pixels)
.ThumbPtr        RESD 1 ;Pointer to the thumbnail image. application must allocate and deallocate for this pointer. NULL = thumbnail image not requested or available.
.PictWidth       RESD 1 ;Picture width (pixels)
.PictHeight      RESD 1 ;Picture height (pixels)
.PictFileSize    RESD 1 ;Finished file size (bytes)
.OutputImageSize RESD 1 ;Output image size and it returns by SC_GetCameraPictInfo or SC_GetNativePictInfo (bytes).
.TimeStamp       RESB SC_Time_size+2 ;Time when the picture was taken.
ENDSTRUC
%DEFINE SC_PictInfoPtr DWORD
;SC_PictInfo contains dimensions and the compressed size of a specified
;image, along with the thumbnail for the image. This information can be
;used for buffer allocation when retrieving native images (FlashPix or
;EXIF) from the camera memory card.

;DC220/DC260 Data Structures

%DEFINE SC_FlashMode DWORD
;SC_FlashMode is used to control the DC220/DC260 camera flash mode.

SC_FlashOff    EQU 0 ;The camera will automatically determine if it should flash.
SC_FlashAuto   EQU 1 ;The camera will flash.
SC_FlashFill   EQU 2 ;The camera will not flash.
SC_FlashRedEye EQU 3 ;The camera will flash with red-eye reduction.

%DEFINE SC_PictureMode DWORD
;SC_PictureMode is used to control the camera Capture mode.

SC_BestQuality   EQU 1 ;The camera will create a best image quality.
SC_BetterQuality EQU 2 ;The camera will create a better image quality.
SC_GoodQuality   EQU 3 ;The camera will create a good image quality.
SC_SuperQuality  EQU 4 ;The camera will create a super image quality.

%DEFINE SC_PictureSize DWORD
;SC_PictureSize is used to control the camera Capture mode.
;Width and height are affected by the camera orientation.

SC_DC260PictSize1536x1024 EQU 0 ;The camera will create a image by 1536(W) x 1024(H).
SC_DC260PictSize1152x768  EQU 1 ;The camera will create a image by 1152(W) x 768(H).
SC_DC260PictSize768x512   EQU 2 ;The camera will create a image by 768(W) x 512(H).
SC_DC220PictSize1152x864  EQU 3 ;The camera will create a image by 1152(W) x 864(H).
SC_DC220PictSize640x480   EQU 4 ;The camera will create a image by 640(W) x 480(H).

%DEFINE SC_VideoFormat DWORD
;SC_VideoFormat is returned by the SDK to the application as
;SC_SpecificConfig information.

SC_NTSCFormat EQU 0 ;The camera video out setting is NTSC video format.
SC_PALFormat  EQU 1 ;The camera video out setting is PAL video format.

%DEFINE SC_FileFormat DWORD
;SC_FileFormat is used to decide the finished file format in the camera.

SC_JPEGFormat     EQU 0 ;The image will be stored by JPEG(EXIF2.0) file format.
SC_FlashPixFormat EQU 1 ;The image will be stored by FlashPix file format.

%DEFINE SC_BatteryStatus DWORD
;SC_BatteryStatus is returned by the SDK to the application as
;SC_SpecificStatus information.

SC_BatteryOK    EQU 0 ;The battery is full power condition. When the AC adapter is used, this value is returned to the application.
SC_BatteryWeak  EQU 1 ;The battery is at a low power condition.

%DEFINE SC_LanguageType DWORD
;SC_LanguageType is used to control the camera user interface language.

SC_English  EQU 0 ;The camera uses English for the UI
SC_Japanese EQU 1 ;The camera uses Japanese for the UI

%DEFINE SC_ZoomPosition DWORD
;SC_ZoomPosition is returned by the SDK to the application as
;SC_TakePictInfo or SC_SpecificPictInfo information.

SC_DC260Zoom37mm  EQU 0  ;The zoom of DC260 is approximately 37mm.
SC_DC260Zoom50mm  EQU 1  ;The zoom of DC260 is approximately 50mm.
SC_DC260Zoom62mm  EQU 2  ;The zoom of DC260 is approximately 62mm.
SC_DC260Zoom73mm  EQU 3  ;The zoom of DC260 is approximately 73mm.
SC_DC260Zoom85mm  EQU 4  ;The zoom of DC260 is approximately 85mm.
SC_DC260Zoom97mm  EQU 5  ;The zoom of DC260 is approximately 97mm.
SC_DC260Zoom111mm EQU 6  ;The zoom of DC260 is approximately 111mm.
SC_DC220Zoom29mm  EQU 10 ;The zoom of DC220 is approximately 29mm.
SC_DC220Zoom34mm  EQU 11 ;The zoom of DC220 is approximately 34mm.
SC_DC220Zoom41mm  EQU 12 ;The zoom of DC220 is approximately 41mm.
SC_DC220Zoom51mm  EQU 13 ;The zoom of DC220 is approximately 51mm.
SC_DC220Zoom58mm  EQU 14 ;The zoom of DC220 is approximately 58mm.

%DEFINE SC_FocusMode DWORD
;SC_FocusMode is used to control the lens auto focus.

SC_FocusAutoMatrix EQU 1 ;The focus is auto matrix mode.
SC_FocusCenterSpot EQU 2 ;The focus is center spot mode. only for DC260
SC_FocusCloseUp    EQU 3 ;The focus is macro mode. only for DC220

%DEFINE SC_TakePictMode DWORD
;SC_TakePictMode is used to control the camera Capture mode.

SC_NormalTakePicture    EQU 0 ;Camera takes a picture in normal mode.
SC_BurstTakePicture     EQU 1 ;Camera takes pictures in burst mode. With this setting, the camera does not use the flash.
SC_TimeLapseTakePicture EQU 2 ;Camera takes pictures in time-lapse mode.

%DEFINE SC_ExposureMode DWORD
;SC_ExposureMode is used to control the camera Capture mode.

SC_AutoExposure      EQU 0 ;Auto exposure with compensation.
SC_ManualExpPriority EQU 1 ;Manual exposure has priority with manual exposure time. This mode is used for long-time exposure. If the camera is in this mode, the flash cannot be used.
SC_FNumberPriority   EQU 2 ;Aperture has priority with setting of FNumber value. This mode is used for external flash syncro (DC260 camera only). If the camera is in this mode, the build-in flash cannot be used.

%DEFINE SC_WhiteBalanceMode DWORD
;SC_WhiteBalanceMode is used to control the camera Capture mode.

SC_AutoWB        EQU 0 ;automatic white balance control is used.
SC_DaylightWB    EQU 1 ;daylight mode white balance control.
SC_FluorescentWB EQU 2 ;fluorescent mode white balance control.
SC_TungstenWB    EQU 3 ;tungsten mode white balance control.
SC_OffWB         EQU 4 ;no white balance control is used.
SC_CustomWB      EQU 5 ;custom white balance control is used.

%DEFINE SC_GetAllPictNameFlag DWORD
;SC_GetAllPictNameFlag is used by SC_GetAllPictureName to determine the
;type of file list returned to the application.

SC_AllPictName       EQU 0 ;All picture names with folder name under the camera.
SC_CameraDirPictName EQU 1 ;All picture names with folder name under the camera root directory.
SC_SystemDirPictName EQU 2 ;All file names with folder name under the camera system directory.
SC_NewTakenPictName  EQU 3 ;A new taken picture name with folder name for every capture mode.
SC_DirectoryName     EQU 4 ;All directory names without any picture names under the camera directory.
SC_MarkedPictName    EQU 5 ;All marked picture names with folder name.
SC_AllFileName       EQU 6 ;All files and folders in the camera.

STRUC SC_PictName
.PictName       RESB 256 ;Picture or directory name. It has to be terminated by NULL. This should be a full pathname. If the image is in an album folder, PictName should be /DC260_01/ALBUM/P0000003.JPG.
.PictSize       RESD 1 ;The one picture size.
.TakePictMode   RESD 1 ;One of values of SC_TakePictMode definition.
.TimeStamp      RESB SC_Time_size ;Date and time when the picture was taken.
.Attribute      RESW 1 ;It indicates an attribute of the picture. T.B.D.
.DPOF_NumPrints RESD 1 ;Number of print of the image. (DPOF information).
.DPOF_PrintType RESD 1 ;Print type (0:Standard, 1:Index). (DPOF information).
ENDSTRUC
%DEFINE SC_PictNamePtr DWORD
;SC_PictName contains information on pictures, albums, and folders.

STRUC SC_SpecificStatus
.BatteryLevel       RESD 1 ;One of the value of the SC_BatteryStatus definition.
.ACAdapter          RESD 1 ;TRUE if an AC adapter is connected. FALSE if an AC adapter is not connected or unknown.
.TakePictReady      RESD 1 ;TRUE if taking a picture is ready. FALSE if taking a picture is not ready.
.FlashStatus        RESD 1 ;TRUE if the flash is ready. FALSE if the  flash is charging or off.
.TimerRunning       RESD 1 ;TRUE if timer for taking a picture is running. FALSE if timer is not running or timer off.
.NumNewPictTaken    RESD 1 ;Number of newly taken pictures by all capture mode.
.NumPictMarked      RESD 1 ;Number of marked pictures.
.NumFiles           RESD 1 ;Number of all files and folders. Always 0 for DC220 and DC260 cameras.
.PictureCount       RESD 1 ;Total number of pictures the camera taken.
.ActiveCameraFolder RESB 256 ;Name of the active camera folder.
ENDSTRUC
%DEFINE SC_SpecificStatusPtr DWORD
;SC_SpecificStatus holds specific status information.
;Status information is data that can be retrieved from the camera and
;cannot be changed by the application.

STRUC SC_TakePictInfo
.FileFormat          RESD 1 ;One of the value of the SC_FileFormat definition.
.FlashMode           RESD 1 ;One of the value of the SC_FlashMode definition. If the TakePictMode is SC_BurstTakePicture, Flash Mode is not available.
.ExposureMode        RESD 1 ;One of the values of the SC_ExposureMode definition. When the user sets SC_ManualExpPriority or SC_FNumberPriority, the SDK ignores the FlashMode setting.
.ExpCompValue        RESW 2 ;The exposure compensation value is in the range from -2.0 E.V. to +2.0 E.V.. This is available when the setting of ExposureMode is SC_AutoExposure. This setting is not saved to the camera and is reset to 0 when the camera is turned off.
.ManualExpTime       RESD 1 ;The shutter speed value is in the range from 500000 (500 msecs) to 16000000 (16 secs). This is available when the setting of ExposureMode is SC_ManualExpPriority. The SDK ignores this field if the user does not set SC_ManualExpPriority to ExposureMode.
.FNumber             RESW 2 ;The aperture value. The DC260 camera has the range from 300 (F3.0) to 1600 (F16). This is available when the setting of ExposureMode is SC_FNumberPriority. The SDK ignores this field if the user does not set Exposure Mode to SC_FNumberPriority. This is always 400 (F4.0) for the DC220 camera.
.ManualFocusFlag     RESD 1 ;DC260 camera only. If set TRUE, the camera uses the setting of ManualFocusDistance. If set FALSE, the camera uses the setting of AutoFocusMode. This setting is not saved to the camera and is reset to FALSE when the camera is turned off.
.ManualFocusDistance RESD 1 ;DC260 camera only. The focus distance is in the range from 50 (50cm) to 65535 (65535cm). Note that 65535 equals to infinity. This field is ignored if ManualFocusFlag is FALSE.
.AutoFocusMode       RESD 1 ;One of the values of the SC_FocusMode definition.
.ZoomPosition        RESD 1 ;One of the values of the SC_ZoomPosition definition.
.WBMode              RESD 1 ;One of the values of the SC_WhiteBalanceMode.
.CustomWBRed         RESW 1 ;Custom white balance setting for red is in the range from 1000 (1.000) to 15999 (15.999). The SDK ignores this field if the WBMode is not SC_CustomWB.
.CustomWBGreen       RESW 1 ;Custom white balance setting for green is in the range from 1000 (1.000) to 15999 (15.999). The SDK ignores this field if the WBMode is not SC_CustomWB.
.CustomWBBlue        RESW 2 ;Custom white balance setting for blue is in the range from 1000 (1.000) to 15999 (15.999). The SDK ignores this field if the WBMode is not SC_CustomWB.
.OrientationFlag     RESD 1 ;TRUE if orientation mode sets to the camera. FALSE if no orientation mode sets to the camera.
.ActiveAlbum         RESB 256 ;The album name the next captured image is saved into. This value should be only album name like “VACATION” and shouldn’t be a full path name like “/DC260_01/VACATION/”.
ENDSTRUC
%DEFINE SC_TakePictInfoPtr DWORD
;SC_TakePictInfo holds specific configuration information that only
;relates to taking a picture.

STRUC SC_TakePictModeInfo
.TakePictMode         RESD 1 ;One of the values of the SC_TakePictMode definition. This field controls the present camera Capture mode. This setting is not saved to the camera and is reset when the camera is turned off.
.NormalPictSize       RESD 1 ;One of the values of the SC_PictureSize definition. This field controls picture size in normal Capture mode.
.NormalPictQuality    RESD 1 ;One of the values of the SC_PictMode definition. This field controls picture quality in normal Capture mode.
.NormalSelfTimer      RESD 1 ;Delay time of self-timer is in the range of 0 (sec) to 60 (secs). The self-timer is on only if this value is not zero. This setting is not saved to the camera and is reset when the camera is turned off. To initiate capture with self-timer, the user physically pushes the camera Shutter button. The SC_StartCapture command does not support capturing with the self-timer.
.BurstPictSize        RESD 1 ;One of the values of the SC_PictureSize definition. This field controls picture size in burst Capture mode.
.BurstPictQuality     RESD 1 ;One of the values of the SC_PictMode definition. This field controls picture quality in burst Capture mode.
.BurstInterval        RESD 1 ;Time interval in burst Capture mode. The range of this value depends on the camera type and picture size.
.TimeLapsePictSize    RESD 1 ;One of the values of the SC_PictureSize definition. This field controls picture size in time-lapse Capture mode.
.TimeLapsePictQuality RESD 1 ;One of the values of the SC_PictMode definition. This field controls picture quality in time-lapse Capture mode.
.TimeLapseInterval    RESD 1 ;Time interval in time-lapse Capture mode. The range is 60 (60 secs) to 86400 (24 hours) for the DC220 camera, and 60 (60 secs) to 86400 (24 hours) for the DC260 camera.
.TimeLapseCount       RESD 1 ;Indicates how many pictures to take with the time-lapse setting and is in the range from 2 to 1000.
ENDSTRUC
%DEFINE SC_TakePictModeInfoPtr DWORD
;SC_TakePictModeInfo holds specific configuration information that
;relates only to taking a picture and is consistent with each Capture
;mode.

STRUC SC_SpecificConfig
.VideoFormat          RESD 1 ;One of the values of the SC_VideoFormat definition.SC_DateFormatDateFormat ;One of the value of the SC_DateFormat definition.SC_TimeFormatTimeFormat ;One of the value of the SC_TimeFormat definition.
.BatterySleepTime     RESD 1 ;This value is for the sleep timeout value when the camera is using batteries
.BatteryPowerDownTime RESD 1 ;This value is for the power down timeout value when the camera is using batteries
.ACPowerDownTime      RESD 1 ;This value is for the power down timeout value when the camera is using AC adapter
.QuickViewInterval    RESD 1 ;Duration of the quickview. The range is from 0 (0sec) to 3000(30secs). And 0 means quickview off.
.SystemSoundFlag      RESD 1 ;TRUE = System sound is on. FALSE = System sound is off.
.LanguageType         RESD 1 ;One of the values of the SC_LanguageType. This field controls the language the camera use. ----- slide show parameters
.SlideShowInterval    RESD 1 ;Duration of each frame in the slide show. The range is from 1(1sec) to 99(99secs).
.SlideShowSoundFlag   RESD 1 ;TRUE: The slide show is performed with sound. FALSE: The slide show is performed without sound.
.SlideShowLoopFlag    RESD 1 ;TRUE: The slide show is performed repeatedly. FALSE: The slide show is performed only once. ----- playback
.BurstPBInterval      RESD 1 ;Playback duration for burst.
.TimeLapsePBInterval  RESD 1 ;Playback duration for time-lapse.
.AlbumPBInterval      RESD 1 ;Playback duration for album.
ENDSTRUC
%DEFINE SC_SpecificConfigPtr DWORD
;SC_SpecificConfig holds specific configuration information.
;Configuration information is data that can be retrieved from the
;camera and can be changed and rewritten to the camera.

STRUC SC_SpecificPictInfo
.FileFormat       RESD 1 ;One of the value of the SC_FileFormat definition.
.PictSize         RESD 1 ;One of the value of the SC_PictureSize definition.
.PictQuality      RESD 1 ;One of the value of the SC_PictureMode definition.
.FlashUsed        RESD 1 ;TRUE = Flash used. FALSE = Flash not used.
.FlashMode        RESD 2 ;One of the value of the SC_FlashMode definition.
.LightValue       RESQ 1 ;Measured light value when the picture was taken. It is in EV unit.shortExpCompValue ;exposure compensation value
.ManualExpTime    RESD 1 ;exposure time in microsec.
.FNumber          RESW 2 ;100 times the F-Number used for this picture.
.FocusMode        RESD 1 ;One of the value of the SC_FocusMode definition.
.ZoomPosition     RESD 1 ;One of the value of the SC_ZoomPosition definition.
.TakePictMode     RESD 1 ;one of the value of SC_TakePictMode
.WBMode           RESD 1 ;one of the value of SC_WhiteBalanceMode
.CustomWBRed      RESW 1 ;custom white balance value for red
.CustomWBGreen    RESW 1 ;custom white balance value for green
.CustomWBBlue     RESW 2 ;custom white balance value for blue
.AudioSize        RESD 1 ;Audio size if the picture has audio recording.
.UserCopyright    RESB 32 ;user copyright string in the picture
.UserData         RESD 8 ;user data in the picture (8 unsigned long)
.UserStringRegion RESD 4 ;region code of user string in the picture
.UserString       RESB 4*32 ;user string in the picture (4 32-byte length)
.UserTag          RESD 1 ;user tag information in the picture
ENDSTRUC
%DEFINE SC_SpecificPictInfoPtr DWORD
;SC_SpecificPictInfo holds specific camera information about a picture.

;Interface Function Specification

;Camera Initialization Functions
%DEFINE SC_OpenCamera _SC_OpenCamera@8
%DEFINE SC_CloseCamera _SC_CloseCamera@4

;Camera Control Functions
%DEFINE SC_GetStatus _SC_GetStatus@12
%DEFINE SC_GetConfig _SC_GetConfig@12
%DEFINE SC_SetConfig _SC_SetConfig@12
%DEFINE SC_ControlCameraTime _SC_ControlCameraTime@12
%DEFINE SC_ControlCameraID _SC_ControlCameraID@12
%DEFINE SC_ControlTakePicture _SC_ControlTakePicture@16
%DEFINE SC_ResetCamera _SC_ResetCamera@4

;Image Management Functions
%DEFINE SC_TakePicture _SC_TakePicture@16
%DEFINE SC_DeletePicture _SC_DeletePicture@8

;Image Retrieval Functions
%DEFINE SC_GetCameraPictInfo _SC_GetCameraPictInfo@20
%DEFINE SC_GetNativePictInfo _SC_GetNativePictInfo@20
%DEFINE SC_GetNativePicture _SC_GetNativePicture@16
%DEFINE SC_ProcNativePicture _SC_ProcNativePicture@12
%DEFINE SC_GetCameraAudio _SC_GetCameraAudio@16
%DEFINE SC_GetNativeAudio _SC_GetNativeAudio@8
%DEFINE SC_GetAllPictureName _SC_GetAllPictureName@12

;Image Placement Function
%DEFINE SC_SetNativePicture _SC_SetNativePicture@16

;Album Management Functions
%DEFINE SC_MovePicturesToAlbum _SC_MovePicturesToAlbum@16
%DEFINE SC_RenameAlbum _SC_RenameAlbum@12
