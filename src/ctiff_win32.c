/* ctiff_win32.c - A TIFF image writing library for spectroscopic data.
 *
 * Created by Ryan Orendorff <ro265@cam.ac.uk>
 * Date: 18/03/12 16:55:19
 *
 * The Laser Analytics Tiff Writer University of Cambridge (camtiff) is a
 * library designed to, given an input 16 bit 3D array and some additional
 * comments, produce a TIFF image stack. It is designed to work with a piece
 * of LabVIEW software within the Laser Analytics group codenamed Apollo, a
 * front end for acquiring spectroscopic images.
 *
 * Copyright (GPL V3):
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if defined(WIN32) && defined(LIB)
#include "ctiff.h"
#endif

