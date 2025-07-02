/**
 * @file lv_conf.h
 * Configuration file for v9.3.0
 */

/*
 * Copy this file as `lv_conf.h`
 * 1. simply next to `lvgl` folder
 * 2. or to any other place and
 *    - define `LV_CONF_INCLUDE_SIMPLE`;
 *    - add the path as an include path.
 */

/* clang-format off */
#if 1 /* Set this to "1" to enable content */

#ifndef LV_CONF_H
#define LV_CONF_H

/*====================
   COLOR SETTINGS
 *====================*/

/** Color depth: 1 (I1), 8 (L8), 16 (RGB565), 24 (RGB888), 32 (XRGB8888) */
#define LV_COLOR_DEPTH 16

/*=========================
   STDLIB WRAPPER SETTINGS
 *=========================*/

/** Possible values
 * - LV_STDLIB_BUILTIN:     LVGL's built in implementation
 * - LV_STDLIB_CLIB:        Standard C functions, like malloc, strlen, etc
 * - LV_STDLIB_MICROPYTHON: MicroPython implementation
 * - LV_STDLIB_RTTHREAD:    RT-Thread implementation
 * - LV_STDLIB_CUSTOM:      Implement the functions externally
 */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_CLIB
#define LV_USE_STDLIB_STRING    LV_STDLIB_CLIB
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_CLIB

/*===================
   OPERATING SYSTEM
 *==================*/

/** Select an operating system to use. Possible options:
 * - LV_OS_NONE
 * - LV_OS_PTHREAD
 * - LV_OS_FREERTOS
 * - LV_OS_CMSIS_RTOS2
 * - LV_OS_RTTHREAD
 * - LV_OS_WINDOWS
 * - LV_OS_CUSTOM
 */
#define LV_USE_OS   LV_OS_FREERTOS

/*==================
   MEMORY SETTINGS
 *==================*/

/** Size of the memory available for `lv_malloc()` in bytes (>= 2kB) */
#define LV_MEM_SIZE (64 * 1024U)          /*[bytes]*/

/** Size of the memory expand for `lv_malloc()` in bytes */
#define LV_MEM_POOL_EXPAND_SIZE 0

/*===================
   HAL SETTINGS
 *==================*/

/** Default display refresh, input device read and animation step period. */
#define LV_DEF_REFR_PERIOD  33      /*[ms]*/

/** Default display dimension */
#define LV_DPI_DEF 130     /*[px/inch]*/

/*========================
   RENDERING CONFIGURATION
 *========================*/

/** Buffer size for the partial rendering (drawing buffer) in pixels.
 * When this buffer is used for rendering, it can be smaller than the display, but there should be at least 1/10 screen-sized buffer.
 * The buffer is initially allocated as a single buffer, but it can be split into multiple smaller buffers by the lv_display_set_buffers() function.
 * The buffer is dedicated for the default display.
 * */
#define LV_DRAW_BUF_SIZE (480 * 40)

/** Default number of draw buffers per display */
#define LV_DRAW_BUF_COUNT 2

/*=================
   WIDGET SETTINGS
 *================*/

/** Documentation of the widgets: https://docs.lvgl.io/master/widgets/index.html */

#define LV_USE_LABEL        1
#if LV_USE_LABEL
    #define LV_LABEL_TEXT_SELECTION 1   /*Enable selecting text of the label*/
    #define LV_LABEL_LONG_TXT_HINT 1    /*Store some extra info in labels to speed up drawing of very long texts*/
    #define LV_LABEL_WAIT_CHAR_COUNT 3  /*The count of wait chart*/
#endif

#define LV_USE_BTN          1

#define LV_USE_IMAGE        1   /*Requires: lv_label*/

#define LV_USE_TEXTAREA     1
#if LV_USE_TEXTAREA != 0
    #define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500    /*ms*/
#endif

#define LV_USE_TABVIEW      1

#define LV_USE_CALENDAR     1

/*==================
 * THEME SETTINGS
 *==================*/

/** A simple, impressive and very complete theme */
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT
    /** 0: Light mode; 1: Dark mode */
    #define LV_THEME_DEFAULT_DARK 0
    /** 1: Enable grow on press */
    #define LV_THEME_DEFAULT_GROW 1
    /** Default transition time in [ms] */
    #define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/** A very simple theme that is a good starting point for a custom theme */
#define LV_USE_THEME_SIMPLE 1

/*==================
 * LAYOUTS
 *==================*/

/** A layout similar to Flexbox in CSS. */
#define LV_USE_FLEX 1

/** A layout similar to Grid in CSS. */
#define LV_USE_GRID 1

/*==================
 * 3RD PARTY LIBRARIES
 *==================*/

/** File system interfaces for common APIs */

/*API for fopen, fread, etc*/
#define LV_USE_FS_STDIO 1
#if LV_USE_FS_STDIO
    #define LV_FS_STDIO_LETTER 'A'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_STDIO_PATH ""         /*Set the working directory. File/directory paths will be appended to it. Use "/" if not required.*/
    #define LV_FS_STDIO_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for open, read, etc*/
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
    #define LV_FS_POSIX_LETTER 'A'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_POSIX_PATH ""         /*Set the working directory. File/directory paths will be appended to it. Use "/" if not required.*/
    #define LV_FS_POSIX_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for CreateFile, ReadFile, etc*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
    #define LV_FS_WIN32_LETTER  'A'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_WIN32_PATH ""          /*Set the working directory. File/directory paths will be appended to it. Use "/" if not required.*/
    #define LV_FS_WIN32_CACHE_SIZE 0     /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*API for FATFS (needs to be added separately). Uses f_open, f_read, etc*/
#define LV_USE_FS_FATFS  0
#if LV_USE_FS_FATFS
    #define LV_FS_FATFS_LETTER 'A'     /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
    #define LV_FS_FATFS_CACHE_SIZE 0    /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*PNG decoder library*/
#define LV_USE_PNG 0

/*BMP decoder library*/
#define LV_USE_BMP 0

/*JPG + split JPG decoder library*/
#define LV_USE_JPEG 0

/*GIF decoder library*/
#define LV_USE_GIF 0

/*QR code library*/
#define LV_USE_QRCODE 0

/*Barcode code library*/
#define LV_USE_BARCODE 0

/*==================
 * EXAMPLES
 *==================*/

/** Enable the examples to be built with the library */
#define LV_BUILD_EXAMPLES 1

/*==================
 * DEMOS
 *==================*/

/** Show some widget. It might be required to increase `LV_MEM_SIZE` */
#define LV_USE_DEMO_WIDGETS 1
#if LV_USE_DEMO_WIDGETS
    #define LV_DEMO_WIDGETS_SLIDESHOW 0
#endif

/** Demonstrate the usage of encoder and keyboard */
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 1

/** Benchmark your system */
#define LV_USE_DEMO_BENCHMARK 1

/** Stress test for LVGL */
#define LV_USE_DEMO_STRESS 1

/** Music player demo */
#define LV_USE_DEMO_MUSIC 1
#if LV_USE_DEMO_MUSIC
    #define LV_DEMO_MUSIC_SQUARE    0
    #define LV_DEMO_MUSIC_LANDSCAPE 0
    #define LV_DEMO_MUSIC_ROUND     0
    #define LV_DEMO_MUSIC_LARGE     0
    #define LV_DEMO_MUSIC_AUTO_PLAY 0
#endif

/** Vector graphics demo */
#define LV_USE_DEMO_VECTOR_GRAPHIC 0

/** Flex layout demo */
#define LV_USE_DEMO_FLEX_LAYOUT 1

/** Demonstrate scroll settings */
#define LV_USE_DEMO_SCROLL 1

/*==================
 * OTHERS
 *==================*/

/** 1: Set default group if `NULL` is passed as a parameter to a function and it is required to tell which group to use. */
#define LV_USE_GROUP 1
#if LV_USE_GROUP
    #define LV_GROUP_DEF_CB 1  /*Enable default group event callback*/
#endif  /*LV_USE_GROUP*/

/** 1: Enable the `lv_indev_get_vect()` function */
#define LV_USE_INDEV_VECTOR 0

/** 1: Enable the `lv_obj_property*` functions */
#define LV_USE_OBJ_PROPERTY 0

/** 1: Enable `lv_obj_id_builtin_*` functions */
#define LV_USE_OBJ_ID_BUILTIN 0

/** Enable the observer pattern */
#define LV_USE_OBSERVER 1

/*==================
 * ASSERT
 *==================*/

/** Enable asserts if an operation is failed or an invalid data is found.
 * If LV_USE_LOG is enabled an error message will be printed on failure */
#define LV_USE_ASSERT_NULL          1   /*Check if the parameter is NULL. (Very fast, recommended)*/
#define LV_USE_ASSERT_MALLOC        1   /*Checks is the memory is successfully allocated or no. (Very fast, recommended)*/
#define LV_USE_ASSERT_STYLE         0   /*Check if the parameter is a valid `lv_style_t` variable or not. (Fast)*/
#define LV_USE_ASSERT_MEM_INTEGRITY 0   /*Check the integrity of `lv_mem` after critical operations. (Slow)*/
#define LV_USE_ASSERT_OBJ           0   /*Check if the parameter is a valid `lv_obj_t` variable or not. (Slow)*/

/*==================
 * LOG SETTINGS
 *==================*/

/** 1: Enable the log module */
#define LV_USE_LOG      0
#if LV_USE_LOG

    /** How important log should be added:
     * LV_LOG_LEVEL_TRACE       A lot of logs to give detailed information
     * LV_LOG_LEVEL_INFO        Log important events
     * LV_LOG_LEVEL_WARN        Log if something unwanted happened but didn't cause a problem
     * LV_LOG_LEVEL_ERROR       Only critical issue, when the system may fail
     * LV_LOG_LEVEL_USER        Only logs added by the user
     * LV_LOG_LEVEL_NONE        Do not log anything
     */
    #define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

    /** 1: Print the log with 'printf';
     * 0: User need to register a callback with `lv_log_register_print_cb()`*/
    #define LV_LOG_PRINTF 0

    /** Enable/disable LV_LOG_TRACE in modules that produces a huge number of logs */
    #define LV_LOG_TRACE_MEM        1
    #define LV_LOG_TRACE_TIMER      1
    #define LV_LOG_TRACE_INDEV      1
    #define LV_LOG_TRACE_DISP_REFR  1
    #define LV_LOG_TRACE_EVENT      1
    #define LV_LOG_TRACE_OBJ_CREATE 1
    #define LV_LOG_TRACE_LAYOUT     1
    #define LV_LOG_TRACE_ANIM       1
    #define LV_LOG_TRACE_CACHE      1

#endif  /*LV_USE_LOG*/

/*==================
 * TIMER SETTINGS
 *==================*/

/** 1: Enable `lv_timer_handler()` to be called periodically and automatically manage all timers. */
#define LV_USE_TIMER 1
#if LV_USE_TIMER
    /** Handle a timer in `lv_timer_handler()` after this time without handling any timer (in milliseconds) */
    #define LV_TIMER_HANDLER_TIMEOUT 1000
#endif

#endif /*LV_CONF_H*/

#endif /*End of "Content enable"*/
