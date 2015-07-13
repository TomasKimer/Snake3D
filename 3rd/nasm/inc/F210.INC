;NASM include file version 1.3 by Tamas Kaproncai [tomcat@szif.hu]

;Kodak DC200/DC210/DC240/DC280 Software Development Kit
;AUTHORS: Takeshi Domen, Eastman Kodak Japan R&D Center
;CREATION DATE: Mar./24/1999, LAST MODIFICATION: Jun./17/1999
;Version 1.0.1600

;Error Codes Summary

DC210_NoErr                      EQU     0 ;Normal successful completion.
DC210_BadArgErr                  EQU -1000 ;Invalid call parameter found.
DC210_CameraNotConnected         EQU -1002 ;Attempted to open a camera not connected to the host.
DC210_CameraFunctionNotSupported EQU -1003 ;This functionality is not supported by the current opened camera type.
DC210_CancelCallback             EQU -1004 ;Operation terminated by callback function.
DC210_CommunicationErr           EQU -1006 ;Error communicating with the camera.
DC240_ImageProcessErr            EQU -1009 ;Error occurred during image processing.
DC210_MallocErr                  EQU -1011 ;Unable to allocate required memory.
DC210_NotOpenErr                 EQU -1013 ;Attempted to use camera or driver before it was opened.
DC210_ReOpenErr                  EQU -1016 ;Unable to open because it is already open. Please close first.
DC210_SDKVersionErr              EQU -1019 ;Compatibility error while opening the driver.
DC210_SystemErr                  EQU -1020 ;General low-level operating system error found.
DC210_MemoryFullErr              EQU -1022 ;The camera is unable to take picture because the image memory is full.
DC210_CardNotInserted            EQU -1027 ;Memory card is not inserted to the camera.
DC240_NotNativeFile              EQU -1028 ;The specified image is not created by DC240 or DC280.
DC240_USBOpenErr                 EQU -1029 ;The USB open error
DC280_CameraInStoringData        EQU -1030 ;The camera is storing the image data.
DC280_CameraBatteryEmpty         EQU -1031 ;Camera battery is empty to take a picture.

;Generic Constant Symbols

DC210_CamIDLength EQU (32+1)
;This symbol defines valid length of Camera ID.

;DC200/DC210 Constant Symbols

DC210_SDKVersion EQU 0x0205
;This symbol defines valid versions of SDK that can be used with this include file.

DC210_DeleteAll EQU -1
;This symbol defines valid parameter for deleting pictures from the camera.
;This can be used in the DC210_DeletePicture function.

DC210_ThumbWidth EQU 96
;This symbol defines valid width of DC210 thumbnail.

DC210_ThumbHeight EQU 72
;This symbol defines valid height of DC210 thumbnail.

;DC240/DC280 Constant Symbols

DC240_SDKVersion EQU 0x0100
;This symbol defines valid versions of SDK that can be used with this header.

DC240_ThumbWidth EQU 160
;This symbol defines valid width of DC240 thumbnail.

DC240_ThumbHeight EQU 120
;This symbol defines valid height of DC240 thumbnail.

DC280_ThumbWidth EQU 160
;This symbol defines valid width of DC280 thumbnail.

DC280_ThumbHeight EQU 106
;This symbol defines valid height of DC280 thumbnail.

;Generic Data Structures

%DEFINE DC210_PortNum DWORD
;DC210_PortNum is used by DC2x0_OpenCamera to connect to the camera.
;The application needs to set one of them when it calls DC2x0_OpenCamera.
;If the apprication sets DC210_PortNumAuto, the SDK returns actual port number
;when it connects to the camera.
;In the case of USB, the SDK always returns DC210_PortNumAuto.

DC210_PortNumAuto EQU 0 ;The SDK automatically searches the com port.
DC210_PortNum1    EQU 1 ;Port Number is 1.
DC210_PortNum2    EQU 2 ;Port Number is 2.
DC210_PortNum3    EQU 3 ;Port Number is 3 that is only for Windows.
DC210_PortNum4    EQU 4 ;Port Number is 4 that is only for Windows.
DC210_PortNum5    EQU 5 ;Port Number is 5 that is only for Windows.
DC210_PortNum6    EQU 6 ;Port Number is 6 that is only for Windows.
DC210_PortNum7    EQU 7 ;Port Number is 7 that is only for Windows.
DC210_PortNum8    EQU 8 ;Port Number is 8 that is only for Windows.
DC210_PortNum9    EQU 9 ;Port Number is 9 that is only for Windows.

%DEFINE DC210_BitRate DWORD
;DC210_BitRate is used by DC2x0_OpenCamera to connect to the camera.
;The application needs to set one of them when it calls DC2x0_OpenCamera.
;If the apprication sets DC210_BitRateAuto, the SDK returns actual bit rate
;when it connects to the camera.
;However, to the USB communication, the SDK always returns DC210_BitRateAuto.

DC210_BitRateAuto   EQU 0 ;The SDK automatically searches the camera communication bit rate.
DC210_BitRate9600   EQU 1 ;The communication bit rate is 9600bps.
DC210_BitRate19200  EQU 2 ;The communication bit rate is 19200bps.
DC210_BitRate38400  EQU 3 ;The communication bit rate is 38400bps.
DC210_BitRate57600  EQU 4 ;The communication bit rate is 57600bps.
DC210_BitRate115200 EQU 5 ;The communication bit rate is 115200bps.

%DEFINE DC210_CameraType DWORD
%DEFINE DC210_CameraTypePtr DWORD
;DC210_CameraType is used to identify the type of camera that is opened by the
;routine DC2x0_OpenCamera.

DC210_DC200     EQU 105 ;The camera is a Kodak Digital Science DC200 camera.
DC210_DC210ZOOM EQU 106 ;The camera is a Kodak Digital Science DC210 Zoom camera.
DC210_DC240     EQU 107 ;The camera is a Kodak DC240 camera.
DC210_DC280     EQU 108 ;The camera is a Kodak DC280 camera.

%DEFINE DC210_ImgType DWORD
;DC210_ImgType is used in the DC210_ImgProcData.

DC210_RGBImage24 EQU 1 ;The output data is a 24-bit RGB image.
DC210_RGBImage32 EQU 2 ;The output data is a 32-bit RGB Image.

%DEFINE DC210_RotateImg DWORD
;DC210_RotateImg is used in the DC210_ImgProcData.

DC210_Rotate0   EQU 0 ;The output image is not rotated.
DC210_Rotate90  EQU 1 ;The output image is rotated 90 degrees counter-clockwise.
DC210_Rotate180 EQU 2 ;The output image is rotated 180 degrees counter-clockwise.
DC210_Rotate270 EQU 3 ;The output image is rotated 270 degrees counter-clockwise.

%DEFINE DC210_ProgressStatus DWORD
;DC210_ProgressStatus is used by the DC210_ProgressProc function to describe
;the status for the call.

DC210_ProgressInit       EQU 0 ;Tells the application to create a status indicator display.
DC210_ProgressUpdate     EQU 1 ;Tells the application to update the status indicator display.
DC210_ProgressEnd        EQU 2 ;Tells the application to close the status indicator display.
DC210_ProgressIncomplete EQU 3 ;Tells the application to terminate incompletely and to close the status indicator display.

%DEFINE DC210_ProgressProc DWORD
;DC210_ProgressProc is a function pointer that is used as a callback to inform
;the application of the progress of the current operation.
;Typical usage of this function type is to display the indicator.

%MACRO DC210_ProgressProcStart 0
PUSH EBP
MOV EBP,ESP
%ENDMACRO

%MACRO DC210_ProgressProcEnd 0
POP EBP
RETN 12
%ENDMACRO

%DEFINE DC210_ProgressProc.Status EBP+8
%DEFINE DC210_ProgressProc.PercentComplete EBP+12
%DEFINE DC210_ProgressProc.RefCon EBP+16

STRUC DC210_Camera
.PortNum         RESD 1 ;The SDK sets the field with actual port number.
.BitRate         RESD 1 ;The SDK sets the field with actual bit rate to the camera.
.CamType         RESD 1 ;This is returned from SDK to the application after calling DC2x0_OpenCamera. And indicates the current camera type.
.CameraOpenFlag  RESD 1 ;DC2x0_OpenCamera sets TRUE and DC210_CloseCamera sets FALSE.
.InternalUseOnly RESD 1 ;The application should never access this field.
ENDSTRUC
%DEFINE DC210_CameraPtr DWORD
;DC210_Camera contains information specific to an open camera.
;It is used to identify which camera should be used.

STRUC DC210_ImgProcData
.RowBytes    RESD 1 ;This is only for Macintosh environment. Define the number of bytes between the beginning of one row of image data and the beginning of the next row.
.ImageType   RESD 1 ;This is one of DC210_ImgType definitions. If the camera is DC240 or DC280, the SDK ignores this value.
.RotateAngle RESD 1 ;This is one of DC210_RotateImg definitions. If the camera is DC240 or DC280, the SDK ignores this value.
ENDSTRUC
%DEFINE DC210_ImgProcDataPtr DWORD
;DC210_ImgProcData is used by DC210_TakePicture, DC2x0_GetPictureInfo,
;DC2x0_GetNativePictInfo and DC240_ProcNativePicture to process image.

STRUC DC210_ProgressCB
.ProgFunc RESD 1 ;A pointer to the progress callback function. If progress information is not needed, this pointer can be set to NULL.
.RefCon   RESD 1 ;A long value to a reference constant that will be passed to the ProgFunc. This is useful in multithread applications to identify the thread that made the call.
ENDSTRUC
%DEFINE DC210_ProgressCBPtr DWORD
;DC210_ProgressCB is a structure that defines a callback routine that can be
;used by DC2x0_GetNativePictue, DC240_ProcNativePicture, DC240_GetFileFromCamera
;and DC240_SetFileToCamera.

STRUC DC210_ImageIOCB
.Buffer     RESD 1 ;A pointer to a buffer to use for I/O operation. The application has to acquire whole image area.
.BufferSize RESD 1 ;Number of bytes to transfer.
ENDSTRUC
%DEFINE DC210_ImageIOCBPtr DWORD
;DC210_ImageIOCB is used by DC240_GetNativePictInfo, DC2x0_GetNativePicture,
;DC240_ProcNativePicture, DC240_GetFileFromCamera and DC240_SetFileToCamera.
;This is used to tell the SDK the address and the size of image I/O buffer.

STRUC DC210_Time
.tm_sec   RESW 1 ;Seconds after the minute[0..59]
.tm_min   RESW 1 ;Minutes after the hour[0..59]
.tm_hour  RESW 1 ;Hours since midnight[0..23]
.tm_mday  RESW 1 ;Day of the month[1..31]
.tm_mon   RESW 1 ;Months since January[0..11]
.tm_year  RESW 1 ;Case of DC200/DC210 camera, Years since 1997. Case of DC240/DC280 camera, Years since 1998.
.tm_wday  RESW 1 ;Days since Sunday[0..6]. -1 = Unknown.
.tm_yday  RESW 1 ;Days since January[0..365]. DC240/DC280 SDK does't support tm_yday.
.tm_isdst RESW 1 ;Daylight Savings Time flag. 1 = Daylight Savings Time in effect. 0 = Daylight Savings Time not in effect. -1 = Unknown. DC240/DC280 SDK does't support tm_isdst.
ENDSTRUC
%DEFINE DC210_TimePtr DWORD
;DC210_Time is used to set/get the time and date to from the camera.

STRUC DC210_Status
.Time              RESB DC210_Time_size+2 ;Current Time.
.FirmwareVersion   RESD 1 ;Version of camera firmware.
.NumPictTaken      RESW 1 ;Number of pictures in the camera.
.NumPictRemaining  RESW 1 ;Number of pictures that can still be taken and stored in the camera.
.MemPictTaken      RESW 1 ;Case of DC200/DC210/DC240/DC280 camera, this is always set as -1.
.MemPictRemaining  RESW 1 ;Case of DC200/DC210/DC240/DC280 camera, this is always set as -1.
.CardPictTaken     RESW 1 ;Number of pictures in the camera Memory card. If Memory card is not inserted into the camera, this is set as -1.
.CardPictRemaining RESW 1 ;Number of pictures that can still be taken and stored in the camera Memory card. If Memory card is not inserted into the camera, this is set as -1.
.ThumbWidth        RESW 1 ;Width of a thumbnail if the current camera settings are used.
.ThumbHeight       RESW 1 ;Height of a thumbnail if the current camera settings are used.
.PictWidth         RESD 1 ;Width of an image if the current camera settings are used.
.PictHeight        RESD 1 ;Height of an image if the current camera settings are used.
ENDSTRUC
%DEFINE DC210_StatusPtr DWORD
;DC210_Status contains status information that is common to typical digital
;cameras. It is used DC210_GetStatus.

STRUC DC210_Config
.Time  RESB DC210_Time_size+2 ;Current time stored in the camera.
.CamId RESD 1 ;CamID is text string that identifies the camera. This string must be allocated by the application, and it must be at least DC210_CamIDLength bytes in length.
ENDSTRUC
%DEFINE DC210_ConfigPtr DWORD
;DC210_Config contains generic configuration information that is common to
;typical digital cameras. DC210_GetConfig and DC210_SetConfig use it.
;Most configuration fields are camera specific, so they are included in the
;camera specific data structures.

STRUC DC210_PictInfo
.PictNumber    RESW 1 ;Picture number of the image in the camera. Case of DC240/DC280 camera, this is always set as -1.
.ThumbWidth    RESW 1 ;Thumbnail width (pixels).
.ThumbHeight   RESW 2 ;Thumbnail height (pixels).
.ThumbPtr      RESD 1 ;A pointer to the thumbnail image. The application must allocate and deallocate memory space for thumbnail image and set its address here.
.PictWidth     RESD 1 ;Picture width (pixels).
.PictHeight    RESD 1 ;Picture height (pixels).
.PictFileSize  RESD 1 ;Finished file size (bytes).
.PictMinBufIn  RESD 1 ;Minimum buffer size to allocate for image data input. Case of DC240/DC280 camera, this is always set as -1.
.PictMinBufOut RESD 1 ;Minimum buffer size to allocate for image data output. Case of DC240/DC280 camera, this is always set as -1.
.TimeStamp RESB DC210_Time_size+2 ;Date and time when the picture was taken.
ENDSTRUC
%DEFINE DC210_PictInfoPtr DWORD
;DC210_PictInfo contains dimensions and compressed size of a specified image
;along with the thumbnail for the image. This information can be used for buffer
;allocation when retrieving images(Flash Pix, JPEG or EXIF) from a camera.

;DC200/DC210 Data Structures

%DEFINE DC210_TimerMode DWORD
;DC210_TimerMode is used to determine whether to take a picture immediately
;or to wait 10 seconds.

DC210_TimerNoDelay EQU 0 ;Take a picture immediately.
DC210_Timer10Sec   EQU 1 ;Take a picture after 10 seconds. When Memory card is not inserted to the camera or the camera is unable to store a picture because the Memory is full, this value can't be set.

%DEFINE DC210_FlashMode DWORD
;DC210_FlashMode is used to control the current flash mode of the DC200/DC210
;camera.

DC210_FlashAuto          EQU 0 ;The camera will automatically determine if it should flash.
DC210_FlashOn            EQU 1 ;The camera will flash.
DC210_FlashOff           EQU 2 ;The camera will not flash.
DC210_RedEyeAuto         EQU 3 ;The camera will automatically determine if it should flash with red-eye adjustment. This value can't be set, when focus is set as DC210_CloseupFocus.
DC210_RedEyeOn           EQU 4 ;The camera will flash with red-eye adjustment. This value can't be set, when focus is set as DC210_CloseupFocus.

%DEFINE DC210_PictureMode DWORD
;DC210_PictureMode is used to control the image capture mode of the camera.

DC210_BestQuality   EQU 1 ;The camera will create a best quality image.
DC210_BetterQuality EQU 2 ;The camera will create a better quality image.
DC210_GoodQuality   EQU 3 ;The camera will create a good quality image.

%DEFINE DC210_PictureSize DWORD
;DC210_PictureSize is used to control the image capture mode of the camera.

DC210_PictSize640x480  EQU 0 ;The camera will create a image by 640(W) x 480(H).
DC210_PictSize1152x864 EQU 1 ;The camera will create a image by 1152(W) x 864(H).

%DEFINE DC210_CommunicationMode DWORD
;DC210_CommunicationMode is returned by the SDK to the application as a
;DC210_SpecificStatus information.

DC210_SerialMode EQU 0 ;The camera communication mode is Serial mode.
DC210_IrDAMode   EQU 1 ;The camera communication mode is IrDA mode. Case of DC210 camera, this value is never returned.

%DEFINE DC210_VideoFormat DWORD
;DC210_VideoFormat is returned by the SDK to the application as a
;DC210_SpecificConfig information.

DC210_NTSCFormat EQU 0 ;The camera video out setting is NTSC video format.
DC210_PALFormat  EQU 1 ;The camera video out setting is PAL video format.

%DEFINE DC210_FileFormat DWORD
;DC210_FileFormat is used to decide the finished file format in the camera.

DC210_JPEGFormat     EQU 3 ;The image will be stored by JPEG file format.
DC210_FlashPixFormat EQU 4 ;The image will be stored by FlashPix file format.

%DEFINE DC210_BatteryStatus DWORD
;DC210_BatteryStatus is returned by the SDK to the application as a
;DC210_SpecificStatus information.

DC210_BatteryOK          EQU 0 ;The battery is full power condition. When the AC adapter is used, this value is returned to the application.
DC210_BatteryWeak        EQU 1 ;The battery is in a low power condition.
DC210_BatteryEmpty       EQU 2 ;The battery is almost empty power condition.

%DEFINE DC210_ZoomPosition DWORD
;DC210_ZoomPosition is returned by the SDK to the application as a
;DC210_TakePictInfo information.

DC210_ZoomReserved      EQU -1 ;This is only for DC200 and if the application sets this value when the DC210ZOOM is connected, the SDK returns error code to the application.
DC210_Zoom29mm          EQU  4 ;The zoom value is approximately 29mm.
DC210_Zoom34mm          EQU  3 ;The zoom value is approximately 34mm.
DC210_Zoom41mm          EQU  2 ;The zoom value is approximately 41mm.
DC210_Zoom51mm          EQU  1 ;The zoom value is approximately 51mm.
DC210_Zoom58mm          EQU  0 ;The zoom value is approximately 58mm.
DC210_ZoomMacroPosition EQU  5 ;The zoom is macro setting. This is read-only and when the user sets the focus as DC210_CloseupFocus, it is set automatically.

%DEFINE DC210_FocusMode DWORD
;DC210_FocusMode is used to control the focus of the lens. The camera can focus for
;close-up pictures, or it can automatically focus using multiple zones.

DC210_FocusReserved  EQU -1 ;This is only for DC200 and if the application sets this value when the DC210ZOOM is connected, the SDK returns error code to the application.
DC210_MultiZoneFocus EQU  0 ;Automatically focus using multiple focus zones.
DC210_CloseupFocus   EQU  2 ;Focus mode is macro.

STRUC DC210_PictName
.PictName RESB 13 ;The one picture name.
.PictSize RESD 1 ;The one picture size.
ENDSTRUC
%DEFINE DC210_PictNamePtr DWORD
;DC210_PictName holds all picture names and sizes in the camera.

STRUC DC210_SpecificStatus
.BatteryLevel      RESD 1 ;One of the values of the DC210_BatteryStatus definition.
.ACAdapter         RESD 1 ;TRUE if an AC adapter is connected. FALSE if an AC adapter is not connected or unknown.
.CardInserted      RESD 1 ;TRUE if the Memory card is inserted into the camera. FALSE if the Memory card is not inserted into the camera.
.CardFormatted     RESD 1 ;TRUE if the inserted Memory card is formatted. FALSE if the inserted Memory card is not formatted. FALSE if the Memory card is not inserted.
.FlashStatus       RESD 1 ;TRUE if the flash is charged. FALSE if the flash is not charged.
.NumImages         RESW 1 ;The number of images in Memory card.
.BestRemaining     RESW 1 ;The minimum number of additional best quality images that can be stored in Memory card.
.BetterRemaining   RESW 1 ;The minimum number of additional better quality images that can be stored in Memory card.
.GoodRemaining     RESW 1 ;The minimum number of additional good quality images that can be stored in Memory card.
.CommunicationMode RESD 1 ;One of the values of the DC210_CommunicationMode definition.
ENDSTRUC
%DEFINE DC210_SpecificStatusPtr DWORD
;DC210_SpecificStatus holds all DC200/DC210 specific status information.
;Status information is data that can be retrieved from the camera
;and cannot be changed by theapplication.

STRUC DC210_TakePictInfo
.FileFormat   RESD 1 ;One of the values of the DC210_FileFormat definition.
.PictSize     RESD 1 ;One of the values of the DC210_PictureSize definition.
.PictQuality  RESD 1 ;One of the values of the DC210_PictureMode definition.
.FlashMode    RESD 1 ;One of the values of the DC210_FlashMode definition.
.ExpCompValue RESW 2 ;The exposure compensation value is in the range from -2.0 E.V. to +2.0 E.V.
.TimerMode    RESD 1 ;One of the values of the DC210_TimerMode definition.
.FocusMode    RESD 1 ;One of the values of the DC210_FocusMode definition.
.ZoomPosition RESD 1 ;One of the values of the DC210_ZoomPosition definition.
ENDSTRUC
%DEFINE DC210_TakePictInfoPtr DWORD
;DC210_TakePictInfo holds all DC200/DC210 specific configuration information
;that only relates to take a picture.

STRUC DC210_SpecificConfig
.VideoFormat     RESD 1 ;One of the values of the DC210_VideoFormat definition. This value can't be changed by DC210_SetConfig function.
.TakePictInfoPtr RESD 1 ;A pointer of DC210_TakePictInfo structure.
ENDSTRUC
%DEFINE DC210_SpecificConfigPtr DWORD
;DC210_SpecificConfig holds all DC200/DC210 specific configuration information.
;Configuration information is data that can be retrieved from the camera
;and can be changed and rewritten to the camera.

STRUC DC210_SpecificPictInfo
.FileFormat   RESD 1 ;One of the values of the DC210_FileFormat definition.
.PictSize     RESD 1 ;One of the values of the DC210_PictureSize definition.
.PictQuality  RESD 1 ;One of the values of the DC210_PictureMode definition.
.FlashUsed    RESD 1 ;TRUE = Flash was used when the picture was taken. FALSE = Flash was not used.
.FlashMode    RESD 2 ;One of the values of the DC210_FlashMode definition.
.LightValue   RESQ 1 ;Measured light value when the picture was taken. The range of this value is 6.5 to 17.0 Cds by 0.125 Cds.
.FNumber      RESW 2 ;10 times the F-Number used for this picture.
.BatteryLevel RESD 1 ;One of the values of the DC210_BatteryStatus definition.
.ExposureTime RESD 1 ;The exposure time for this picture in microseconds when the picture was taken.
.ExpCompValue RESW 2 ;The exposure compensation value is in the range from -2.0 E.V. to +2.0 E.V.
.FocusMode    RESD 1 ;One of the values of the DC210_FocusMode definition.
.ZoomPosition RESD 1 ;One of the values of the DC210_ZoomPosition definition.
.ImageName    RESB 13 ;The name of the image is "DCInnnnn.ttt". nnnnn = number(1 ~ 99999), ttt = JPG or FPX. The ImageName[12] has to be NULL. The camera can include 1000 images in memory card at the same time if possible.
ENDSTRUC
%DEFINE DC210_SpecificPictInfoPtr DWORD
;DC210_SpecificPictInfo holds all DC200/DC210ZOOM camera specific information
;about a specific picture.

;DC240/DC280 Data Structures

%DEFINE DC240_TimerMode DWORD
;DC240_TimerMode is used to determine whether to take a picture immediately
;or to wait for 10 seconds.

DC240_TimerNoDelay EQU 0 ;Take a picture immediately.
DC240_Timer10Sec   EQU 1 ;Take a picture after 10 seconds.

%DEFINE DC240_FlashMode DWORD
;DC240_FlashMode is used to control the current flash mode of the DC240 camera.

DC240_FlashAuto  EQU 0 ;The camera will automatically determine if it should flash.
DC240_FlashOn    EQU 1 ;The camera will flash.
DC240_FlashOff   EQU 2 ;The camera will not flash.
DC240_RedEyeAuto EQU 3 ;The camera will automatically determine if it should flash with red-eye reduction.

%DEFINE DC240_PictureMode DWORD
;DC240_PictureMode is used to control the image capture mode of the camera.

DC240_BestQuality   EQU 1 ;The camera will create a best quality image.
DC240_BetterQuality EQU 2 ;The camera will create a better quality image.
DC240_GoodQuality   EQU 3 ;The camera will create a good quality image.

%DEFINE DC240_VideoFormat DWORD
;DC240_VideoFormat is returned by the SDK to the application as a
;DC240_SpecificStatus information. However, to the DC280 camera,
;DC240_VideoFormat can be set by the application via SDK as a
;DC280_SpecificConfig value.

DC240_NTSCFormat EQU 0 ;The camera video out setting is NTSC video format.
DC240_PALFormat  EQU 1 ;The camera video out setting is PAL video format.

%DEFINE DC240_FileFormat DWORD
;DC240_FileFormat is used to decide the finished file format in the camera.

DC240_EXIFFormat EQU 5 ;The image will be stored by EXIF file format.

%DEFINE DC240_BatteryStatus DWORD
;DC240_BatteryStatus is returned by the SDK to the application as a
;DC240_SpecificStatus information.

DC240_BatteryOK      EQU 0 ;The battery is full power condition.
DC240_BatteryWeak    EQU 1 ;The battery is in a low power condition.
DC240_BatteryEmpty   EQU 2 ;The battery is almost empty power condition.
DC240_ACAdapterOK    EQU 4 ;The AC adapter level is OK.
DC240_ACAdapterWeak  EQU 5 ;The AC adapter level is weak.
DC240_ACAdapterEmpty EQU 6 ;The AC adapter level is empty. For example, if external battery is connected and it is empty, this value is returned.

%DEFINE DC240_CommType DWORD
;DC240_CommType is used by DC240_OpenCamera to connect to the DC240 camera
;or to connect to the DC280 camera. The application needs to set one of them
;when it calls DC240_OpenCamera. If the application sets DC240_CommAuto, the
;SDK returns actual communication type when it connects to the camera.

DC240_CommAuto   EQU 0 ;If the application sets this value, the SDK automatically searches the communication type.
DC240_CommSerial EQU 1 ;The communication is established via serial device to the camera.
DC240_CommUSB    EQU 2 ;The communication is established via USB device to the camera.

%DEFINE DC240_AEMode DWORD
;DC240_AEMode is used to control taking picture(s) to the camera.

DC240_AEAuto         EQU 0 ;The AE mode is auto.
DC240_AECenterWeight EQU 1 ;The AE mode is center weight.

%DEFINE DC240_WhiteBalanceMode DWORD
;DC240_WhiteBalanceMode is used to control taking picture(s) to the camera.

DC240_WBAuto        EQU 0 ;Auto white balance adjustment.
DC240_WBFluorescent EQU 1 ;Fluorescent white balance adjustment.
DC240_WBTungsten    EQU 2 ;Tungsten white balance adjustment.
DC240_WBDaylight    EQU 3 ;Daylight white balance adjustment.

%DEFINE DC240_DateTimeStampMode DWORD
;DC240_DateTimeStampMode is used to control taking picture(s) to the camera.

DC240_DTSOff                EQU 0 ;Neither the date nor the time won't be stamped on an image.
DC240_DTSYearMonDay         EQU 1 ;The date stamp is "YEAR MONTH DAY".
DC240_DTSDayMonYear         EQU 2 ;The date stamp is "DAY MONTH YEAR".
DC240_DTSMonDayYear         EQU 3 ;The date stamp is "MONTH DAY YEAR".
DC240_DTSYearMonDay_HourMin EQU 4 ;The date/time stamp is "YEAR MONTH DAY HOUR:MINUTE".
DC240_DTSDayMonYear_HourMin EQU 5 ;The date/time stamp is "DAY MONTH YEAR HOUR:MINUTE".
DC240_DTSMonDayYear_HourMin EQU 6 ;The date/time stamp is "MONTH DAY YEAR HOUR:MINUTE".

%DEFINE DC240_SharpnessMode DWORD
;DC240_SharpnessMode is used to control taking picture(s) to the camera.

DC240_SharpnessSoft     EQU -1 ;Soft level sharpness.
DC240_SharpnessStandard EQU  0 ;Standard level sharpness.
DC240_SharpnessSharp    EQU  1 ;Sharp level sharpness.

%DEFINE DC240_GetFileNameFlag DWORD
;DC240_GetFileNameFlag is used by DC240_GetAllPictureName
;to determine which type of file list is to be returned to the application.

DC240_GetAllFileNameFlag          EQU 0 ;All picture names, file names and folder names under the specific directory.
DC240_GetPictNameFlag             EQU 1 ;All picture names under the specific directory.
DC240_GetBorderNameFlag           EQU 2 ;All border file names under the specific directory.
DC240_GetOtherNameFlag            EQU 3 ;All non-picture and non-border file names under the specific directory.
DC240_GetDirNameFlag              EQU 4 ;All direcctory names(without any picture names) under the specific directory.
DC240_GetLastReviewedPictNameFlag EQU 5 ;Last reviewed picture name.
DC240_GetLastTakenPictNameFlag    EQU 6 ;Last taken picture name.
DC240_GetDPOFNameFlag             EQU 7 ;DPOF picture name.

STRUC DC240_FileName
.FileName  RESB 256 ;A file name or folder name. It has to terminate by NULL.
.FileSize  RESD 1 ;A file byte size. If the FileName is folder name, it is not set by the SDK.
.TimeStamp RESB DC210_Time_size ;Date and time when the file or the folder was created.
.Attribute RESW 1 ;It indicates an attribute of the file or the folder. Bit0(LSB): 1 if the item is protected. Bit4: 1 if the item is a folder. Bit5: 1 if the item is modified file (archive bit).
ENDSTRUC
%DEFINE DC240_FileNamePtr DWORD
;DC240_FileName holds a file information in the camera.

STRUC DC240_CardConfig
.CardInserted   RESD 1 ;TRUE if the Memory card is inserted into the camera. FALSE if the Memory card is not inserted. Case of GetSetFlag is FALSE, the SDK ignores this value.
.CardFormatted  RESD 1 ;TRUE if the inserted Memory card is formatted. FALSE if the inserted Memory card is not formatted. FALSE if the Memory card is not inserted. Case of GetSetFlag is FALSE, the SDK ignores this value.
.FormatCardFlag RESW 2 ;If FormatCardFlag is 1, the camera formats the Memory card. In this case, all of the data in the Memory card is deleted. Case of FormatCardFlag isn't 1 and GetSetFlag is TRUE, the SDK does nothing.
ENDSTRUC
%DEFINE DC240_CardConfigPtr DWORD
;DC240_CardConfig holds camera Memory card configuration information.

;DC240 Data Structures

%DEFINE DC240_PictureSize DWORD
;DC240_PictureSize is used to control the image capture mode of the camera.

DC240_PictSize640x480  EQU 0 ;The camera will create a image by 640(W) x 480(H).
DC240_PictSize1280x960 EQU 2 ;The camera will create a image by 1280(W) x 960(H).

%DEFINE DC240_ZoomPosition DWORD
;DC240_ZoomPosition is returned by the SDK to the application as a
;DC240_TakePictInfo information to the DC240.

DC240_ZoomMacroPosition EQU 7 ;The zoom is macro setting. This is read-only and when the user sets the focus as DC240_CloseupFocus, it is set automatically.
DC240_Zoom117mm         EQU 0 ;The zoom value is approximately 117mm.
DC240_Zoom104mm         EQU 1 ;The zoom value is approximately 104mm.
DC240_Zoom90mm          EQU 2 ;The zoom value is approximately 90mm.
DC240_Zoom77mm          EQU 3 ;The zoom value is approximately 77mm.
DC240_Zoom64mm          EQU 4 ;The zoom value is approximately 64mm.
DC240_Zoom52mm          EQU 5 ;The zoom value is approximately 52mm.
DC240_Zoom39mm          EQU 6 ;The zoom value is approximately 39mm.
DC240_ZoomDigital       EQU 8 ;The zoom mode is digital. If this value is set, the picture size is forced to be DC240_PictureSize640x480

%DEFINE DC240_FocusMode DWORD
;DC240_FocusMode is used to control the focus of the lens.

DC240_AutoCenterFocus EQU 0 ;Automatically focus using center focus zones.
DC240_ManualFocus     EQU 1 ;Manual focus mode.
DC240_CloseupFocus    EQU 2 ;Focus mode is macro.
DC240_InfinityFocus   EQU 3 ;Focus mode is infinity.

%DEFINE DC240_ImageEffectMode DWORD
;DC240_ImageEffectMode is used to control taking picture(s) to the camera.

DC240_IEOff               EQU 0 ;Disable image effect.
DC240_IEGrayScale         EQU 1 ;The picture will be taken with gray scale.
DC240_IESepia             EQU 2 ;The picture will be taken with sepia color.
DC240_IEDocument          EQU 4 ;The picture will be taken with document.
DC240_IEBorderComposition EQU 8 ;The picture will be taken with border composition.

STRUC DC240_SpecificStatus
.BatteryStatus      RESD 1 ;One of the values of the DC240_BatteryStatus definition.
.ACAdapter          RESD 1 ;TRUE if an AC adapter is connected. FALSE if an AC adapter is not connected or unknown.
.CardInserted       RESD 1 ;TRUE if the Memory card is inserted into the camera. FALSE if the Memory card is not inserted.
.CardFormatted      RESD 1 ;TRUE if the inserted Memory card is formatted. FALSE if the inserted Memory card is not formatted. FALSE if the Memory card is not inserted.
.FlashCharged       RESD 1 ;TRUE if the flash is charged. FALSE if the flash is not charged.
.NumBestRemaining   RESW 1 ;The minimum number of additional best quality images that can be stored in Memory card.
.NumBetterRemaining RESW 1 ;The minimum number of additional better quality images that can be stored in Memory card.
.NumGoodRemaining   RESW 1 ;The minimum number of additional good quality images that can be stored in Memory card.
.NumDirectory       RESW 1 ;The number of directories under the root directory. It indicates every combinations of the directory.
.VideoFormat        RESD 1 ;One of the values of the DC240_VideoFormat definition.
.CFVolumeID         RESB 12 ;Volume ID of CF Card(Memory Card).
ENDSTRUC
%DEFINE DC240_SpecificStatusPtr DWORD
;DC240_SpecificStatus holds specific status information.
;Status information is data that can be retrieved from the camera
;and cannot be changed by the application.

STRUC DC240_TakePictInfo
.FileFormat          RESD 1 ;One of the values of the DC240_FileFormat definition.
.PictureMode         RESD 1 ;One of the values of the DC240_PictureMode definition.
.PictureSize         RESD 1 ;One of the values of the DC240_PictureSize definition.
.FlashMode           RESD 1 ;One of the values of the DC240_FlashMode definition.
.ExposureLocked      RESD 1 ;TRUE = Exposure will be locked. FALSE = Exposure will not be locked.
.ExpCompValue        RESW 2 ;The exposure compensation value is in the range from -200(-2.0 E.V.) to +200 (+2.0 E.V.).
.AEMode              RESD 1 ;One of the values of the DC240_AEMode definition.
.ExposureTime        RESD 1 ;The exposure time for this picture in microseconds.
.FNumber             RESW 2 ;The aperture value. DC240 has the range from 280(F2.8) to 1600(F16).
.TimerMode           RESD 1 ;One of the values of the DC240_TimerMode definition.
.FocusMode           RESD 1 ;One of the values of the DC240_FocusMode definition.
.ManualFocusDistance RESD 1 ;Focus distance and it is in the range from 30(30cm) to 65535(65535cm). Note that 65535 equals to infinity. This field is ignored if FocusMode is DC240_AutoMultiFocus, DC240_AutoCenterFocus or DC240_CloseupFocus.
.ZoomPosition        RESD 1 ;One of the values of the DC240_ZoomPosition definition.
.WBMode              RESD 1 ;One of the values of the DC240_WhiteBalanceMode definition.
.DigitalZoomValue    RESW 1 ;The digital zoom value is in the range from 31(3.1x) to 60(6.0x). The SDK ignores this field if ZoomPosition isn't DC240_ZoomDigital.
.IEMode              RESW 1 ;Combination value of the DC240_ImageEffectMode definition.
.DTSMode             RESD 1 ;One of the values of the DC240_DateTimeStampMode definition.
.SharpnessMode       RESD 1 ;One of the values of the DC240_SharpnessMode definition.
.BorderFileName      RESB 13 ;Border file name. If BorderFileName is NULL string, the camera doesn't use border file.
ENDSTRUC
%DEFINE DC240_TakePictInfoPtr DWORD
;DC240_TakePictInfo holds specific configuration information that only relates
;to take a picture.

STRUC DC240_SpecificConfig
.TakePictInfo RESB DC240_TakePictInfo_size ;A structure of the DC240_TakePictInfo.
ENDSTRUC
%DEFINE DC240_SpecificConfigPtr DWORD
;DC240_SpecificConfig holds specific configuration information.
;Configuration information is data that can be retrieved from the camera
;and can be changed and rewritten to the camera.

STRUC DC240_SpecificPictInfo
.PictName            RESB 256 ;Picture name of the image in the camera, It has to be terminated by NULL.
.OutputImageSize     RESD 1 ;Output image size. DC210_TakePicture, DC240_GetCameraPictInfo and DC240_GetNativePictInfo return it. (bytes)
.FileFormat          RESD 1 ;One of the values of the DC240_FileFormat definition.
.PictureMode         RESD 1 ;One of the values of the DC240_PictureMode definition.
.PictureSize         RESD 1 ;One of the values of the DC240_PictureSize definition.
.FlashUsed           RESD 1 ;TRUE = Flash used. FALSE = Flash not used.
.FlashMode           RESD 1 ;One of the values of the DC240_FlashMode definition.
.ExposureLocked      RESD 1 ;TRUE = Exposure locked. FALSE = Exposure not locked.
.ExpCompValue        RESW 2 ;The exposure compensation value when the picture was taken. It is in the range from -200(-2.0 E.V.) to +200(+2.0 E.V.).
.AEMode              RESD 2 ;One of the values of the DC240_AEMode definition.
.ExposureValue       RESQ 1 ;Measured exposure value when the picture was taken. It is in EV unit.
.ExposureTime        RESD 1 ;The exposure time for this picture in microseconds.
.FNumber             RESW 2 ;The aperture value.
.FocusMode           RESD 1 ;One of the values of the DC240_FocusMode definition.
.ManualFocusDistance RESD 1 ;Focus distance when the picture was taken. It is in the range from 30(30cm) to 65535(65535cm). Note that 65535 equals to infinity. This field is ignored if FocusMode is DC240_FocusAuto or DC240_FocusCloseup.
.ZoomPosition        RESD 1 ;One of the values of the DC240_ZoomPosition definition.
.DigitalZoomValue    RESW 2 ;The digital zoom value when the picture was taken. This field is ignored if ZoomPosition isn't DC240_ZoomDigital. It is in the range from 31(3.1x) to 60(6.0x).
.WBMode              RESD 1 ;One of the values of the DC240_WhiteBalanceMode definition.
.IEMode              RESW 2 ;Combination value of the DC240_ImageEffectMode definition.
.DTSMode             RESD 1 ;One of the values of the DC240_DateTimeStampMode definition.
.SharpnessMode       RESD 1 ;One of the values of the DC240_SharpnessMode definition.
.BorderFileName      RESB 13 ;Border file name when the picture was taken. If BorderFileName is NULL string, the camera didn't use border file.
ENDSTRUC
%DEFINE DC240_SpecificPictInfoPtr DWORD
;DC240_SpecificPictInfo holds camera specific information about a specific
;picture.

;DC280 Data Structures

%DEFINE DC280_PictureSize DWORD
;DC280_PictureSize is used to control the image capture mode of the DC280
;camera.

DC280_PictSize896x592  EQU 0 ;The camera will create a image by 896(W) x 592(H).
DC280_PictSize1760x1168 EQU 2 ;The camera will create a image by 1760(W) x 1168(H).

%DEFINE DC280_ZoomPosition DWORD
;DC280_ZoomPosition is returned by the SDK to the application as a
;DC280_TakePictInfo information to the DC280.

DC280_ZoomMacroPosition EQU 7 ;The zoom is macro setting. This is read-only and when the user sets the focus as DC280_CloseupFocus, it is set automatically.
DC280_Zoom60mm          EQU 0 ;The zoom value is approximately 60mm.
DC280_Zoom54mm          EQU 1 ;The zoom value is approximately 54mm.
DC280_Zoom48mm          EQU 2 ;The zoom value is approximately 48mm.
DC280_Zoom42mm          EQU 3 ;The zoom value is approximately 42mm.
DC280_Zoom36mm          EQU 4 ;The zoom value is approximately 36mm.
DC280_Zoom33mm          EQU 5 ;The zoom value is approximately 33mm.
DC280_Zoom30mm          EQU 6 ;The zoom value is approximately 30mm.
DC280_ZoomDigital       EQU 8 ;The zoom mode is digital. If this value is set, the picture size is forced to be DC280_PictureSize896x592

%DEFINE DC280_ImageEffectMode DWORD
;DC280_ImageEffectMode is available to control taking picture(s) to the camera.

DC280_IEOff                        EQU 0 ;Disable image effect.
DC280_IEGrayScale                  EQU 1 ;The picture will be taken with gray scale.
DC280_IESepia                      EQU 2 ;The picture will be taken with sepia color.
DC280_IEDocument                   EQU 4 ;The picture will be taken with document.
DC280_IEBorderComposition          EQU 8 ;The picture will be taken with border composition.
DC280_IEGrayScaleBorderComposition EQU 9 ;The picture will be taken with gray scale and border composition.
DC280_IESepiaBorderComposition     EQU 10 ;The picture will be taken with sepia color and border composition.
DC280_IEDocumentBorderComposition  EQU 12 ;The picture will be taken with document and border composition.

%DEFINE DC280_PowerSaveMode DWORD
;DC280_PowerSaveMode is available to control the power save mode of the DC280
;camera.

DC280_PowerSaveModeDisable EQU 0 ;The power save mode of DC280 camera is disable.
DC280_PowerSaveModeEnable  EQU 1 ;The power save mode of DC280 camera is enable.

%DEFINE DC280_QuickViewMode DWORD
;DC280_QuickViewMode is available to control the quick view mode of the DC280
;camera.

DC280_QuickViewModeDisable EQU 0 ;The quick view mode of DC280 camera is disable.
DC280_QuickViewModeEnable  EQU 1 ;The quick view mode of DC280 camera is enable.

%DEFINE DC280_BeepMode DWORD
DC280_BeepModeDisable EQU 0 ;The beep mode of DC280 camera is disable.
DC280_BeepModeSelect  EQU 1 ;The beep mode of DC280 camera is select mode. It enables only for the shutter release and alert user.
DC280_BeepModeAll     EQU 2 ;The beep mode of DC280 camera is all mode. The camera makes sound whenever the user operates the camera.

%DEFINE DC280_ISOMode DWORD
;DC280_ISOMode is available to control the ISO mode of the DC280 camera.

DC280_ISOModeFixed EQU 0 ;The ISO mode of DC280 camera is fixed.
DC280_ISOModeAuto  EQU 1 ;The ISO mode of DC280 camera is auto.

%DEFINE DC280_LanguageType DWORD
;DC280_LanguageType is available to control the language type of the DC280
;camera.

DC280_EnglishType    EQU 0 ;The language type of DC280 camera is English.
DC280_JapaneseType   EQU 1 ;The language type of DC280 camera is Japanese.
DC280_FrenchType     EQU 2 ;The language type of DC280 camera is French.
DC280_GermanType     EQU 3 ;The language type of DC280 camera is German.
DC280_SpanishType    EQU 4 ;The language type of DC280 camera is Spanish.
DC280_ItalianType    EQU 5 ;The language type of DC280 camera is Italian.
DC280_PortugueseType EQU 6 ;The language type of DC280 camera is Portuguese.

%DEFINE DC280_FocusMode DWORD
;DC280_FocusMode is available to control the focus of the lens.

DC280_AutoCenterFocus EQU 0 ;Automatically focus using center focus zones.
DC280_CloseupFocus    EQU 2 ;Focus mode is macro.
DC280_InfinityFocus   EQU 3 ;Focus mode is infinity.

STRUC DC280_SpecificStatus
.BatteryStatus      RESD 1 ;One of the values of the DC240_BatteryStatus definition.
.ACAdapter          RESD 1 ;TRUE if an AC adapter is connected. FALSE if an AC adapter is not connected or unknown.
.CardInserted       RESD 1 ;TRUE if the Memory card is inserted into the camera. FALSE if the Memory card is not inserted.
.CardFormatted      RESD 1 ;TRUE if the inserted Memory card is formatted. FALSE if the inserted Memory card is not formatted. FALSE if the Memory card is not inserted.
.FlashCharged       RESD 1 ;TRUE if the flash is charged. FALSE if the flash is not charged.
.NumBestRemaining   RESW 1 ;The minimum number of additional best quality images that can be stored in Memory card.
.NumBetterRemaining RESW 1 ;The minimum number of additional better quality images that can be stored in Memory card.
.NumGoodRemaining   RESW 1 ;The minimum number of additional good quality images that can be stored in Memory card.
.NumDirectory       RESW 1 ;The number of directories under the root directory. It indicates every combinations of the directory.
.CFVolumeID         RESB 12 ;Volume ID of CF Card(Memory Card).
ENDSTRUC
%DEFINE DC280_SpecificStatusPtr DWORD
;DC280_SpecificStatus holds specific status information.
;Status information is data that can be retrieved from the camera
;and cannot be changed by the application.

STRUC DC280_TakePictInfo
.FileFormat            RESD 1 ;One of the values of the DC240_FileFormat definition.
.PictureMode           RESD 1 ;One of the values of the DC240_PictureMode definition.
.PictureSize           RESD 1 ;One of the values of the DC280_PictureSize definition. The difference field from DC240_TakePictInfo.
.FlashMode             RESD 1 ;One of the values of the DC240_FlashMode definition.
.ExposureLocked        RESD 1 ;TRUE = Exposure will be locked. FALSE = Exposure will not be locked.
.ExpCompValue          RESW 2 ;The exposure compensation value is in the range from -200(-2.0 E.V.) to +200 (+2.0 E.V.).
.AEMode                RESD 1 ;One of the values of the DC240_AEMode definition.
.ExposureTime          RESD 1 ;The exposure time for this picture in microseconds.
.FNumber               RESW 2 ;The aperture value. DC280 has the range from 280(F2.8) to 1600(F16).
.TimerMode             RESD 1 ;One of the values of the DC240_TimerMode definition.
.FocusMode             RESD 1 ;One of the values of the DC280_FocusMode definition.
.ZoomPosition          RESD 1 ;One of the values of the DC280_ZoomPosition definition.
.WBMode                RESD 1 ;One of the values of the DC240_WhiteBalanceMode definition.
.DigitalZoomValue      RESW 1 ;The digital zoom value is in the range from 31(3.1x) to 60(6.0x). The SDK ignores this field if ZoomPosition isn't DC280_ZoomDigital.
.IEMode                RESW 1 ;Combination value of the DC280_ImageEffectMode definition.
.DTSMode               RESD 1 ;One of the values of the DC240_DateTimeStampMode definition.
.SharpnessMode         RESD 1 ;One of the values of the DC240_SharpnessMode definition.
.BorderFileName        RESB 13 ;Border file name. If BorderFileName is NULL string, the camera doesn't use border file.
.ISOMode               RESD 1 ;One of the values of the DC280_ISOMode definition.
.DefaultWBMode         RESD 1 ;Power Off Default WBMode value. One of the values of the DC240_WhiteBalanceMode definition.
.DefaultIEMode         RESW 2 ;Power Off Default value of the combination of DC280_ImageEffectMode definition.
.DefaultFlashMode      RESD 1 ;Power Off Default FlashMode value. It is one of the values of the DC240_FlashMode definition.
.DefaultExpCompValue   RESW 2 ;Power Off Default ExpCompValue value The exposure compensation value is in the range from -200(-2.0 E.V.) to +200 (+2.0 E.V.).
.DefaultBorderFileName RESB 13 ;Power Off Default Border file name. If BorderFileName is NULL string, the camera doesn't use border file.
.DefaultISOMode        RESD 1 ;Power Off Default ISOMode value. It is one of the values of the DC280_ISOMode definition.
ENDSTRUC
%DEFINE DC280_TakePictInfoPtr DWORD
;DC280_TakePictInfo holds specific configuration information that only relates
;to take a picture.

STRUC DC280_SpecificConfig
.VideoFormat   RESD 1 ;One of the values of the DC240_VideoFormat definition.
.PowerSaveMode RESD 1 ;One of the values of the DC280_PowerSaveMode definition.
.QuickViewMode RESD 1 ;One of the values of the DC280_QuickViewMode definition.
.BeepMode      RESD 1 ;One of the values of the DC280_BeepMode definition.
.LanguageType  RESD 1 ;One of the values of the DC280_LanguageType definition.
.TakePictInfo  RESB DC280_TakePictInfo_size ;A structure of the DC280_TakePictInfo.
ENDSTRUC
%DEFINE DC280_SpecificConfigPtr DWORD
;DC280_SpecificConfig holds specific configuration information.
;Configuration information is data that can be retrieved from the camera
;and can be changed and rewritten to the camera.

STRUC DC280_SpecificPictInfo
.PictName            RESB 256 ;Picture name of the image in the camera, It has to be terminated by NULL.
.OutputImageSize     RESD 1 ;Output image size. DC210_TakePicture, DC240_GetCameraPictInfo and DC240_GetNativePictInfo return it (bytes).
.FileFormat          RESD 1 ;One of the values of the DC240_FileFormat definition.
.PictureMode         RESD 1 ;One of the values of the DC240_PictureMode definition.
.PictureSize         RESD 1 ;One of the values of the DC280_PictureSize definition.
.FlashUsed           RESD 1 ;TRUE = Flash used. FALSE = Flash not used.
.FlashMode           RESD 1 ;One of the values of the DC240_FlashMode definition.
.TimerMode           RESD 1 ;One of the values of the DC240_TimerMode definition.
.ExposureLocked      RESD 1 ;TRUE = Exposure locked. FALSE = Exposure not locked.
.ExpCompValue        RESW 2 ;The exposure compensation value when the picture was taken. It is in the range from -200(-2.0 E.V.) to +200(+2.0 E.V.).
.AEMode              RESD 1 ;One of the values of the DC240_AEMode definition.
.ExposureValue       RESQ 1 ;Measured exposure value when the picture was taken. It is in EV unit.
.ExposureTime        RESD 1 ;The exposure time for this picture in microseconds.
.FNumber             RESW 2 ;The aperture value.
.FocusMode           RESD 1 ;One of the values of the DC280_FocusMode definition.
.ZoomPosition        RESD 1 ;One of the values of the DC280_ZoomPosition definition.
.DigitalZoomValue    RESW 2 ;The digital zoom value when the picture was taken. This field is ignored if ZoomPosition isn't DC280_ZoomDigital. It is in the range from 31(3.1x) to 60(6.0x).
.WBMode              RESD 1 ;One of the values of the DC240_WhiteBalanceMode definition.
.IEMode              RESW 2 ;Combination value of the DC240_ImageEffectMode definition.
.DTSMode             RESD 1 ;One of the values of the DC240_DateTimeStampMode definition.
.SharpnessMode       RESD 1 ;One of the values of the DC240_SharpnessMode definition.
.BorderFileName      RESB 13 ;Border file name when the picture was taken. If BorderFileName is NULL string, the camera didn't use border file.
.Protected           RESD 1 ;TRUE = the image file in the camera is protected. FALSE = the image file in the camera is not protected. Difference field from DC240_SpecificPictInfo.
.ISOMode             RESD 1 ;One of the values of the DC280_ISOMode definition.
.BatteryStatus       RESD 1 ;One of DC240_BatteryOK, DC240_BatteryWeak, and DC240_BatteryEmpty of the values of the DC240_BatteryStatus when the picture was taken, is set.
ENDSTRUC
%DEFINE DC280_SpecificPictInfoPtr DWORD
;DC280_SpecificPictInfo holds camera specific information about a specific
;picture.

;Interface Function Specification

;Camera Initialization Functions
%DEFINE DC210_CloseCamera _DC210_CloseCamera@4
%DEFINE DC210_OpenCamera _DC210_OpenCamera@8
%DEFINE DC240_OpenCamera _DC240_OpenCamera@16

;Camera Control Functions
%DEFINE DC210_GetStatus _DC210_GetStatus@12
%DEFINE DC210_GetConfig _DC210_GetConfig@12
%DEFINE DC210_SetConfig _DC210_SetConfig@12
%DEFINE DC210_ControlCameraTime _DC210_ControlCameraTime@12
%DEFINE DC210_ControlCameraID _DC210_ControlCameraID@12
%DEFINE DC210_ControlTakePicture _DC210_ControlTakePicture@20
%DEFINE DC210_ResetCamera _DC210_ResetCamera@4
%DEFINE DC240_ControlMemoryCard _DC240_ControlMemoryCard@12
%DEFINE DC280_SetFileProtection _DC280_SetFileProtection@12

;Image Management Functions
%DEFINE DC210_TakePicture _DC210_TakePicture@20
%DEFINE DC210_DeletePicture _DC210_DeletePicture@8
%DEFINE DC240_DeletePicture _DC240_DeletePicture@8
%DEFINE DC280_TakePictureAsync _DC280_TakePictureAsync@4

;Image Retrieval Functions
%DEFINE DC210_GetCameraPictInfo _DC210_GetCameraPictInfo@24
%DEFINE DC210_GetNativePicture _DC210_GetNativePicture@16
%DEFINE DC210_GetAllPictureName _DC210_GetAllPictureName@8
%DEFINE DC240_GetCameraPictInfo _DC240_GetCameraPictInfo@28
%DEFINE DC240_GetNativePictInfo _DC240_GetNativePictInfo@24
%DEFINE DC240_GetNativePicture _DC240_GetNativePicture@16
%DEFINE DC240_ProcNativePicture _DC240_ProcNativePicture@16
%DEFINE DC240_GetAllPictureName _DC240_GetAllPictureName@20

;File Transfer Functions
%DEFINE DC240_GetFileFromCamera _DC240_GetFileFromCamera@16
%DEFINE DC240_SetFileToCamera _DC240_SetFileToCamera@16

;Miscellaneous Functions
%DEFINE DC240_GetLastError _DC240_GetLastError@4
