#include <math.h>
#include <assert.h>

#include "veco.h"

int main() {
    // Have 100 vectors (double type) of dimension 10
    veco* d = veco_new("foo.bin", 100, 10, sizeof(double));
    double x[10];
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 10; ++j) {
            // Fill array with some data
            x[j] = sin(i + 13*j);
        }
        // Write the vector to file.
        veco_write(d, x, 1);
    }
    veco_close(d);

    // Open the file to read vectors from
    veco* d2 = veco_open("foo.bin");
    // Place to save the read data
    int dim = d2->dim;
    double x2[dim];
    // Loop over the file
    for (int i = 0; i < d2->n; ++i) {
        // Read into the array
        veco_read(x2, d2, 1);
        for (int j = 0; j < dim; ++j) {
            // Check that the numbers are right
            assert(x2[j] == sin(i + 13*j));
        }
    }
    // Close the file
    veco_close(d2);
    return 0;
}