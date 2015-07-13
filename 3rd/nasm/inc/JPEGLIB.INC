JPEG_LIB_VERSION EQU 61 ;Version 6a

%DEFINE JSAMPLE       BYTE  ;Representation of a single sample (pixel element)
%DEFINE JCOEF         WORD  ;Representation of a DCT frequency coefficient
%DEFINE JOCTET        BYTE  ;Compressed datastreams are represented as arrays
%DEFINE UINT8         BYTE  ;must hold at least the values 0..255
%DEFINE UINT16        WORD  ;must hold at least the values 0..65535
%DEFINE INT16         WORD  ;must hold at least the values -32768..32767
%DEFINE INT32         DWORD ;must hold at least signed 32-bit values
%DEFINE JDIMENSION    DWORD ;Datatype used for image dimensions
%DEFINE JSAMPROW      DWORD ;ptr to one image row of pixel samples
%DEFINE JSAMPARRAY    DWORD ;ptr to some rows (a 2-D sample array)
%DEFINE JSAMPIMAGE    DWORD ;a 3-D sample array: top index is color
%DEFINE J_COLOR_SPACE DWORD ;Known color spaces
%DEFINE J_DCT_METHOD  DWORD ;DCT/IDCT algorithm options
%DEFINE J_DITHER_MODE DWORD ;Dithering options for decompression
%DEFINE J_DITHER_MODE DWORD ;Dithering options for decompression

BITS_IN_JSAMPLE    EQU 8     ;use 8 or 12
MAXJSAMPLE         EQU 255   ;use 255 or 4095
CENTERJSAMPLE      EQU 128   ;use 128 or 2048
MAX_COMPONENTS     EQU 10    ;maximum number of image components
JPEG_MAX_DIMENSION EQU 65500 ;a tad under 64K to prevent overflows

RGB_RED            EQU 0     ;Offset of Red in an RGB scanline element
RGB_GREEN          EQU 1     ;Offset of Green
RGB_BLUE           EQU 2     ;Offset of Blue
RGB_PIXELSIZE      EQU 3     ;JSAMPLEs per RGB scanline element

DCTSIZE             EQU 8  ;The basic DCT block is 8x8 samples
DCTSIZE2            EQU 64 ;DCTSIZE squared; # of elements in a block
NUM_QUANT_TBLS      EQU 4  ;Quantization tables are numbered 0..3
NUM_HUFF_TBLS       EQU 4  ;Huffman tables are numbered 0..3
NUM_ARITH_TBLS      EQU 16 ;Arith-coding tables are numbered 0..15
MAX_COMPS_IN_SCAN   EQU 4  ;JPEG limit on # of components in one scan
MAX_SAMP_FACTOR     EQU 4  ;JPEG limit on sampling factors
C_MAX_BLOCKS_IN_MCU EQU 10 ;compressor's limit on blocks per MCU
D_MAX_BLOCKS_IN_MCU EQU 10 ;decompressor's limit on blocks per MCU

JCS_UNKNOWN   EQU 0 ;error/unspecified
JCS_GRAYSCALE EQU 1 ;monochrome
JCS_RGB       EQU 2 ;red/green/blue
JCS_YCbCr     EQU 3 ;Y/Cb/Cr (also known as YUV)
JCS_CMYK      EQU 4 ;C/M/Y/K
JCS_YCCK      EQU 5 ;Y/Cb/Cr/K

JDCT_ISLOW EQU 0 ;slow but accurate integer algorithm
JDCT_IFAST EQU 1 ;faster, less accurate integer method
JDCT_FLOAT EQU 2 ;floating-point: accurate, fast on fast HW

JDITHER_NONE    EQU 0 ;no dithering
JDITHER_ORDERED EQU 1 ;simple ordered dither
JDITHER_FS      EQU 2 ;Floyd-Steinberg error diffusion dither

STRUC jpeg_decompress_struct
.err                      RESD 1 ;Error handler module
.mem                      RESD 1 ;Memory manager module
.progress                 RESD 1 ;Progress monitor, or NULL if none
.is_decompressor          RESD 1 ;so common code can tell which is which
.global_state             RESD 1 ;for checking call sequence validity
.src                      RESD 1 ;Source of compressed data
.image_width              RESD 1 ;nominal image width (from SOF marker)
.image_height             RESD 1 ;nominal image height
.num_components           RESD 1 ;# of color components in JPEG image
.jpeg_color_space         RESD 1 ;colorspace of JPEG image
.out_color_space          RESD 1 ;colorspace for output
.scale_num                RESD 1 ;fraction by which to scale image
.scale_denom              RESD 1
.output_gamma             RESQ 1 ;image gamma wanted in output
.buffered_image           RESD 1 ;TRUE=multiple output passes
.raw_data_out             RESD 1 ;TRUE=downsampled data wanted
.dct_method               RESD 1 ;IDCT algorithm selector
.do_fancy_upsampling      RESD 1 ;TRUE=apply fancy upsampling
.do_block_smoothing       RESD 1 ;TRUE=apply interblock smoothing
.quantize_colors          RESD 1 ;TRUE=colormapped output wanted
.dither_mode              RESD 1 ;type of color dithering to use
.two_pass_quantize        RESD 1 ;TRUE=use two-pass color quantization
.desired_number_of_colors RESD 1 ;max # colors to use in created colormap
.enable_1pass_quant       RESD 1 ;enable future use of 1-pass quantizer
.enable_external_quant    RESD 1 ;enable future use of external colormap
.enable_2pass_quant       RESD 1 ;enable future use of 2-pass quantizer
.output_width             RESD 1 ;scaled image width
.output_height            RESD 1 ;scaled image height
.out_color_components     RESD 1 ;# of color components in out_color_space
.output_components        RESD 1 ;# of color components returned
.rec_outbuf_height        RESD 1 ;min recommended height of scanline buffer
.actual_number_of_colors  RESD 1 ;number of entries in use
.colormap                 RESD 1 ;The color map as a 2-D pixel array
.output_scanline          RESD 1 ;0 .. output_height-1
.input_scan_number        RESD 1 ;Number of SOS markers seen so far
.input_iMCU_row           RESD 1 ;Number of iMCU rows completed
.output_scan_number       RESD 1 ;Nominal scan number being displayed
.output_iMCU_row          RESD 1 ;Number of iMCU rows read
.coef_bits                RESD DCTSIZE2 ;-1 or current Al value for each coef
.quant_tbl_ptrs           RESD NUM_QUANT_TBLS ;ptrs to coefficient quantization
.dc_huff_tbl_ptrs         RESD NUM_HUFF_TBLS ;ptrs to Huffman coding tables,
.ac_huff_tbl_ptrs         RESD NUM_HUFF_TBLS ;or NULL if not defined
.data_precision           RESD 1 ;bits of precision in image data
.comp_info                RESD 1 ;describes component in SOF
.progressive_mode         RESD 1 ;TRUE if SOFn specifies progressive mode
.arith_code               RESD 1 ;TRUE=arithmetic coding, FALSE=Huffman
.arith_dc_L               RESB NUM_ARITH_TBLS ;L values for DC arith-coding
.arith_dc_U               RESB NUM_ARITH_TBLS ;U values for DC arith-coding
.arith_ac_K               RESB NUM_ARITH_TBLS ;Kx values for AC arith-coding
.restart_interval         RESD 1 ;MCUs per restart interval, or 0 for no restart
.saw_JFIF_marker          RESD 1 ;TRUE iff a JFIF APP0 marker was found
.density_unit             RESB 1 ;JFIF code for pixel size units
.X_density                RESW 1 ;Horizontal pixel density
.Y_density                RESW 1 ;Vertical pixel density
.saw_Adobe_marker         RESD 1 ;TRUE iff an Adobe APP14 marker was found
.Adobe_transform          RESB 1 ;Color transform code from Adobe marker
.CCIR601_sampling         RESD 1 ;TRUE=first samples are cosited
.max_h_samp_factor        RESD 1 ;largest h_samp_factor
.max_v_samp_factor        RESD 1 ;largest v_samp_factor
.min_DCT_scaled_size      RESD 1 ;smallest DCT_scaled_size of any component
.total_iMCU_rows          RESD 1 ;# of iMCU rows in image
.sample_range_limit       RESD 1 ;table for fast range-limiting 
.comps_in_scan            RESD 1 ;# of JPEG components in this scan
.cur_comp_info            RESD MAX_COMPS_IN_SCAN ; describes components in SOS
.MCUs_per_row             RESD 1 ;# of MCUs across the image
.MCU_rows_in_scan         RESD 1 ;# of MCU rows in the image
.blocks_in_MCU            RESD 1 ;# of DCT blocks per MCU
.MCU_membership           RESD D_MAX_BLOCKS_IN_MCU ;index in cur_comp_info
.Ss                       RESD 1 ;progressive JPEG parameters for scan
.Se                       RESD 1
.Ah                       RESD 1
.Al                       RESD 1
.unread_marker            RESD 1 ;either zero or the code of a JPEG marker
.master                   RESD 1 ;Links to subobject jpeg_decomp_master
.main                     RESD 1 ;Links to subobject jpeg_d_main_controller
.coef                     RESD 1 ;Links to subobject jpeg_d_coef_controller
.post                     RESD 1 ;Links to subobject jpeg_d_post_controller
.inputctl                 RESD 1 ;Links to subobject jpeg_input_controller
.marker                   RESD 1 ;Links to subobject jpeg_marker_reader
.entropy                  RESD 1 ;Links to subobject jpeg_entropy_decoder
.idct                     RESD 1 ;Links to subobject jpeg_inverse_dct
.upsample                 RESD 1 ;Links to subobject jpeg_upsampler
.cconvert                 RESD 1 ;Links to subobject jpeg_color_deconverter
.cquantize                RESD 1 ;Links to subobject jpeg_color_quantizer
ENDSTRUC

JMSG_LENGTH_MAX   EQU 200 ;recommended size of format_message buffer
JMSG_STR_PARM_MAX EQU 80  ;max length of the message string parameter

STRUC jpeg_error_mgr
.error_exit          RESD 1 ;Error exit handler: does not return to caller
.emit_message        RESD 1 ;Conditionally emit a trace or warning message
.output_message      RESD 1 ;Routine that actually outputs a trace or error msg
.format_message      RESD 1 ;Format a msg string for the most recent JPEG error
.reset_error_mgr     RESD 1 ;Reset error state variables at start of new image
.msg_code            RESD 1 ;The msg ID code and any parameters are saved here
.msg_parm            RESB JMSG_STR_PARM_MAX ;1 string parameter or up to 8 int
.trace_level         RESD 1 ;max msg_level that will be displayed
.num_warnings        RESD 1 ;number of corrupt-data warnings
.jpeg_message_table  RESD 1 ;Library errors
.last_jpeg_message   RESD 1 ;Table contains strings 0..last_jpeg_message
.addon_message_table RESD 1 ;Non-library errors
.first_addon_message RESD 1 ;code for first string in addon table
.last_addon_message  RESD 1 ;code for last string in addon table
ENDSTRUC

JPEG_RST0 EQU 0xD0 ;RST0 marker code
JPEG_EOI  EQU 0xD9 ;EOI marker code
JPEG_APP0 EQU 0xE0 ;APP0 marker code
JPEG_COM  EQU 0xFE ;COM marker code
