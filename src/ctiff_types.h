/* ctiff_types.h - A TIFF image writing library for spectroscopic data.
 *
 * Created by Ryan Orendorff <ro265@cam.ac.uk> 18/03/12 16:51:10
 *
 * The Laser Analytics Tiff Writer University of Cambridge (camtiff) is a
 * library designed to, given an input 16 bit 3D array and some additional
 * comments, produce a TIFF image stack. It is designed to work with a piece of
 * LabVIEW software within the Laser Analytics group codenamed Apollo, a front
 * end for acquiring spectroscopic images.
 *
 * Copyright (GPL V3): This program is free software: you can redistribute it
 * and/or modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CTIFF_TYPES_H

#define CTIFF_TYPES_H

#if defined(WIN32) && !defined(__WIN32)
#define __WIN32
#endif

// Windows corrections
#if defined(WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef __WIN32
  #define bool	BOOL
  #define true	1
  #define false 0
  #define __bool_true_false_are_defined   1
#else
  #include <stdbool.h> // bool type
#endif

struct tiff;

//      Pixel size = ((pixel_type & 0x0F) + 0x01) << 3
// TIFF Pixel type = (pixel_type >> 4) & 0x0F
#define CTIFF_PIXEL_TYPE_MIN 1
#define CTIFF_PIXEL_TYPE_MAX 3
enum pixel_type_e {           // LibTIFF tags
  CTIFF_PIXEL_UINT8   = 0x10, // SAMPLEFORMAT_UINT   = 1,
  CTIFF_PIXEL_UINT16  = 0x11,
  CTIFF_PIXEL_UINT32  = 0x13,
  CTIFF_PIXEL_INT8    = 0x20, // SAMPLEFORMAT_INT    = 2,
  CTIFF_PIXEL_INT16   = 0x21,
  CTIFF_PIXEL_INT32   = 0x23,
  CTIFF_PIXEL_FLOAT32 = 0x33, // SAMPLEFORMAT_IEEEFP = 3
  CTIFF_PIXEL_FLOAT64 = 0x37
};

/* TODO: Support the complex pixel data types.
 *   SAMPLEFORMAT_VOID          = 4 // Does not reflect real life signals
 *   SAMPLEFORMAT_COMPLEXINT    = 5
 *   SAMPLEFORMAT_COMPLEXIEEEFP = 6
 */


typedef struct {
  const char *artist;
  const char *copyright;
  const char *make;
  const char *model;
  const char *software;
  const char *image_desc;
} CTIFF_basic_metadata;

typedef struct {
  const char   *data;
  const char   *white_space;
} CTIFF_extended_metadata;

typedef struct CTIFF_dir_style_s {
  unsigned  int width;
  unsigned  int height;
  unsigned  int bps;
  unsigned char pixel_data_type;
           bool in_color;
           bool black_is_min;
  unsigned  int x_resolution;
  unsigned  int y_resolution;
} CTIFF_dir_style;

typedef struct CTIFF_dir_s {
          CTIFF_dir_style  style;
     CTIFF_basic_metadata  basic_meta;
  CTIFF_extended_metadata  ext_meta;
               const char *timestamp;
               const void *data;
       struct CTIFF_dir_s *next_dir;
                      int  refs;
} CTIFF_dir;

typedef struct CTIFF_s {
  struct tiff  *tiff;
  const char   *output_file;
  unsigned int  num_dirs;
  unsigned int  num_page_styles;
  bool          strict;
  unsigned int  write_every_num;
  unsigned int  num_unwritten;

  CTIFF_dir    *def_dir;
  CTIFF_dir    *first_dir;
  CTIFF_dir    *last_dir;
  CTIFF_dir    *write_ptr;
} * CTIFF;

#endif /* end of include guard: CTIFF_TYPES_H */
