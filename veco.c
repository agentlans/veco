#include <stdlib.h>

#include "veco.h"

const int header_size = sizeof(veco) - sizeof(FILE*);

// Moves to the place in the file containing the vector with index i
void veco_seek(veco* d, long i) {
    long place = header_size + (d->dim * i) * d->elem_size;
    fseek(d->data, place, SEEK_SET);
}

// Returns index of the vector at the current position
long veco_where(const veco* d) {
    return (ftell(d->data) - header_size) / (d->dim * d->elem_size);
}

// Updates the number of vectors in the file using data from the veco object.
// In other words, a sync operation.
void veco_update_size(veco* d) {
    long ind = veco_where(d);
    // Go to the beginning of the file
    fseek(d->data, 0, SEEK_SET);
    // Update the current value of n in the file
    fwrite(&(d->n), sizeof(long), 1, d->data);
    // Go back to where we were before the operation
    veco_seek(d, ind);
}

// Updates the number of vectors in the veco object by reading from its underlying file.
// If there's more than one object pointing to the same file, call this sub
// to update the veco object.
void veco_sync(veco* d) {
    long ind = veco_where(d);
    // Go to the beginning of the file
    fseek(d->data, 0, SEEK_SET);
    // Read the value of n
    fread(&(d->n), sizeof(long), 1, d->data);
    // Go back to where we were before the operation
    veco_seek(d, ind);
}

// Overwrites the current place in the file with the given vectors
void veco_write(veco* d, const void* vecs, long num_vecs) {
    long ind = veco_where(d);
    fwrite(vecs, d->elem_size * d->dim, num_vecs, d->data);
    // Update the number of vectors if necessary
    if (ind + num_vecs > d->n) {
        d->n = ind;
        veco_update_size(d);
    }
}

// Reads num_vecs vectors from the current place in the file into vecs.
// vecs must have at least as much memory to hold num_vecs vectors.
void veco_read(void* vecs, veco* d, long num_vecs) {
    fread(vecs, d->elem_size * d->dim, num_vecs, d->data);
}

// Creates and returns a file to hold n vectors of dimension dim
// where each component of the vector has size elem_size bytes.
// n can be set to 0. It will automatically increase when vectors are written into the file.
veco* veco_new(const char* filename, long n, int dim, int elem_size) {
    veco* d = malloc(sizeof(veco));
    if (!d) return NULL;
    // Open a new binary file
    FILE* f = fopen(filename, "w+b");
    if (!f) {
        free(d);
        return NULL;
    }
    // Write the file header
    fwrite(&n, sizeof(long), 1, f);
    fwrite(&dim, sizeof(int), 1, f);
    fwrite(&elem_size, sizeof(int), 1, f);
    // Write zeros
    char zero = 0;
    long total_size = n*dim*elem_size;
    for (long c = 0; c < total_size; ++c) {
        fwrite(&zero, 1, 1, f);
    }
    fflush(f);
    // Return the struct
    *d = (veco) {n, dim, elem_size, f};
    veco_seek(d, 0); // go to beginning
    return d;
}

// Opens an existing file and returns the object representing it.
// Must do veco_close when done.
veco* veco_open(const char* filename) {
    veco* d = malloc(sizeof(veco));
    if (!d) return NULL;
    // Open existing binary file
    FILE* f = fopen(filename, "r+b");
    if (!f) {
        free(d);
        return NULL;
    }
    // Read the file header
    fread(&(d->n), sizeof(long), 1, f);
    fread(&(d->dim), sizeof(int), 1, f);
    fread(&(d->elem_size), sizeof(int), 1, f);
    // Return the struct
    d->data = f;
    return d;
}

// Closes an existing file and frees the object representing it.
void veco_close(veco* d) {
    fclose(d->data);
    free(d);
}
