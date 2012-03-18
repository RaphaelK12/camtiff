/* ctiff_types.c - A TIFF image writing library for spectroscopic data.
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

#include "ctiff_types.h"


int __CTIFFAddPage(CTIFF ctiff, CTIFF_dir *dir)
{
  if (ctiff == NULL) return ECTIFFNULL;
  if (dir == NULL) return ECTIFFNULLDIR;


  if (ctiff->first_dir == NULL){
    ctiff->first_dir = dir;
  } else {
    ctiff->last_dir->next_dir = dir;
  }

  ctiff->last_dir = dir;
  dir->refs++;

  ctiff->num_unwritten++;

  if (ctiff->num_unwritten >= ctiff->write_every_num){
    CTIFFWriteFile(ctiff);
  }

  return 0;
}

int CTIFFAddNewPage(CTIFF ctiff, const char* name, const char* ext_meta,
                 const void* page)
{
  int retval = 0;

  CTIFF_dir *new_dir;
  CTIFF_dir *def_dir;

  if (ctiff == NULL) return ECTIFFNULL;

  new_dir  = (CTIFF_dir*) malloc(sizeof(struct CTIFF_dir_s));
  def_dir  = ctiff->def_dir;

  // Not empty CTIFF
  if (ctiff->last_dir != NULL){
    if (memcmp(&ctiff->last_dir->style, &def_dir->style, sizeof(CTIFF_dir_style))){
      ctiff->num_page_styles++;
    }
  }

  memcpy(new_dir, ctiff->def_dir, sizeof(struct CTIFF_dir_s));

  new_dir->timestamp = __CTIFFGetTime();
  new_dir->ext_meta.data = __CTIFFCreateValidExtMeta(ctiff->strict, name,
                                                     ext_meta);

  new_dir->data = page;

  retval = __CTIFFAddPage(ctiff, new_dir);
  return retval;
}



void __CTIFFFreeExtMeta(CTIFF_extended_metadata *ext_meta)
{
  if (ext_meta->data == NULL) return;

  FREE(ext_meta->data);
}

void __CTIFFFreeDir(CTIFF_dir *dir)
{
  if  (dir == NULL) return;

  if (dir->refs > 1){
    dir->refs--;
  } else {
    __CTIFFFreeExtMeta(&dir->ext_meta);
    FREE(dir->timestamp);
  }
}

int __CTIFFFreeFile(CTIFF ctiff)
{
  if  (ctiff == NULL) return ECTIFFNULL;

  CTIFF_dir* tmp_dir;
  while (ctiff->first_dir != NULL){
    tmp_dir = ctiff->first_dir;
    ctiff->first_dir = tmp_dir->next_dir;
    __CTIFFFreeDir(tmp_dir);
  }

  FREE(ctiff->def_dir->ext_meta.white_space);
  FREE(ctiff->def_dir);
  FREE(ctiff);
  ctiff = NULL;
  tmp_dir = NULL;
  return 0;
}
