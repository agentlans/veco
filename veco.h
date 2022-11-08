/*******************************************************************************
* Copyright 2022 Alan Tseng
* 
* This program is free software: you can redistribute it and/or modify it under 
* the terms of the GNU Lesser General Public License as published by the Free 
* Software Foundation, either version 3 of the License, or (at your option) any 
* later version.
* 
* This program is distributed in the hope that it will be useful, but WITHOUT 
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more 
* details.
* 
* You should have received a copy of the GNU Lesser General Public License 
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/

#ifndef _VECO
#define _VECO

#include <stdio.h>

// Keeps a large collection of vectors on disk.
// Allows random access to any vector in the collection.

// n vectors of size dim stored in the data file
// where each element is elem_size bytes large.

// Each row is one vector.
typedef struct {
    long n;
    int dim;
    int elem_size;
    FILE* data;
} veco;

veco* veco_new(const char* filename, long n, int dim, int elem_size);
veco* veco_open(const char* filename);
void veco_sync(veco* d);
void veco_close(veco* d);

long veco_where(const veco* d); // current index
void veco_seek(veco* d, long i); // go to index
// Read and write vectors from the current location
void veco_read(void* vecs, veco* d, long num_vecs);
void veco_write(veco* d, const void* vecs, long num_vecs);

#endif
