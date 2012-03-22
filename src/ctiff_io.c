/* ctiff_io.c - A TIFF image writing library for spectroscopic data.
 *
 * Created by Ryan Orendorff <ro265@cam.ac.uk> 18/03/12 16:52:58
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

#include "ctiff_io.h"
#include "ctiff_types.h"
#include "ctiff_util.h"
#include "ctiff_error.h"
#include "ctiff_settings.h"
#include "ctiff_write.h"
#include "ctiff_data.h"

#include <stdlib.h>  // malloc
#include <string.h>  // memset
#include <tiffio.h>  // libTIFF (preferably 3.9.5+)


CTIFF CTIFFNewFile(const char* output_file)
{
  CTIFF                      ctiff = (CTIFF) malloc(sizeof(struct CTIFF_s));
  CTIFF_dir               *def_dir = (CTIFF_dir*) malloc(sizeof(CTIFF_dir));
  CTIFF_dir_style           *style = &def_dir->style;
  CTIFF_basic_metadata     *b_meta = &def_dir->basic_meta;
  CTIFF_extended_metadata  *e_meta = &def_dir->ext_meta;

  // TODO: If output_file == NULL, write to tmp location.
  if ((ctiff->tiff = TIFFOpen(output_file, "w")) == NULL){
    FREE(ctiff);
    return NULL;
  }

  // Set root level information
  ctiff->output_file     = output_file;
  ctiff->num_dirs        = 0;
  ctiff->num_page_styles = 1;
  ctiff->strict          = true;

  // Safer to write as soon as possible in case the image data disappears.
  ctiff->write_every_num = 1;
  ctiff->num_unwritten   = 0;

  ctiff->first_dir = NULL;
  ctiff->last_dir  = NULL;
  ctiff->write_ptr = NULL;

  // Set def dir def data pointers
  def_dir->timestamp = NULL;
  def_dir->data      = NULL;
  def_dir->next_dir  = NULL;
  def_dir->refs      = 0;

  // Set basic def dir style.
  style->black_is_min = true;
  style->x_resolution = 72;
  style->y_resolution = 72;

  // Set basic metadata
  b_meta->artist = NULL;
  b_meta->copyright = NULL;
  b_meta->make = NULL;
  b_meta->model = NULL;
  b_meta->software = NULL;
  b_meta->image_desc = NULL;

  // Set extended metadata
  e_meta->data = NULL;

  ctiff->def_dir = def_dir;
  return ctiff;
}


int CTIFFCloseFile(CTIFF ctiff)
{
  if (ctiff == NULL) return ECTIFFNULL;

  TIFFClose(ctiff->tiff);
  __CTIFFFreeFile(ctiff);

  return 0;
}

