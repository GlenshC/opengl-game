/* 
    See end of file for license information

    gc_vector is light and header-only dynamic array (vector like) library for C
    Made with performance, and safety in mind (i guess)
    Sadly I don't like the idea of negative indices
    So instead this API gives you full control of the vector struct

    Why? 
    Because I love C too much.
    
    
    EXAMPLE USAGE:

    #define GC_VECTOR_SHORT_NAMES               // uses the short macro definitions
    #include "gc_vector_macro.h"

    typedef gcv_t(int) intPtr;                  // define a vector struct typedef
    struct intPtr gcv_struct(int);              // the same as above but plain struct
    int main(void)
    {
        intPtr *arr0 = gcv_create(arr);         // create and initialize the vector 
        struct intPtr *arr1 = gcv_create(arr);  // same as above

        gcv_push(arr0, 10);                     // appends to last
        gcv_push(arr0, 12);    
        int val = gcv_pop(arr0);                // remove and get last element

        for (int i =0; i < gcv_size(arr0); i++)
        {
            gcv_at(arr0,i);
        }
    }
*/

#ifndef GC_VECTOR_H
#define GC_VECTOR_H

#include <stdlib.h>


// THIS IS THE FUNCTION DOCUMENTATION
#ifdef GC_VECTOR_SHORT_NAMES

#define gcv_t(type)                 gc_vector_t(type)           // defines a vector struct (for typedef's)
#define gcv_struct(type)            gc_vector_struct(type)      // defines a vector struct (manual)
#define gcv_create(v)               gc_vector_create(v)         // mallocs and initialize vector struct
#define gcv_init(v)                 gc_vector_init(v)           // initialize vector struct              
#define gcv_push(v, value)          gc_vector_push(v, value)    // add item to the back of vector 
#define gcv_pop(v)                  gc_vector_pop(v)            // remove the last item and returns it
#define gcv_at(v, i)                gc_vector_at(v, i)          // no bounds checking get value at index
#define gcv_at_safe(v, i)           gc_vector_at_safe(v, i)     // get value at index (bounds checking)
#define gcv_size(v)                 gc_vector_size(v)           // get current size of the vector
#define gcv_empty(v)                gc_vector_empty(v)        // checks if the array is empty
#define gcv_isempty(v)              gc_vector_isempty(v)        // checks if the array is empty
#define gcv_max(v)                  gc_vector_max(v)            // get the current max capacity of the vector
#define gcv_data(v)                 gc_vector_data(v)           // get the data array
#define gcv_free(v)                 gc_vector_free(v)           // frees the array and vector struct
#define gcv_type(v)                 gc_vector_type(v)           // returns a element with that type (for sizeof)

#endif

#define gc_vector_t(type)           struct {size_t n, m; type *data;}
#define gc_vector_struct(type)      {size_t n, m; type *data;}
#define gc_vector_create(v)         malloc(sizeof(*v)); gc_vector_init(v)
#define gc_vector_init(v)           ((v)->n = 0, (v)->m = 0, (v)->data = NULL)
#define gc_vector_push(v, value)    (_gc_vector_grow(v), (v)->data[(v)->n++] = (value))
#define gc_vector_pop(v)            (gc_vector_isempty(v)) ? (v)->data[--(v)->n] : 0
#define gc_vector_empty(v)          ((v)->n = 0)
#define gc_vector_at(v, i)          ((v)->data[(i)])
#define gc_vector_at_safe(v, i)     (_gc_inbounds(v,i) ? ((v)->data[(i)]) : gc_vector_type(v))
#define gc_vector_isempty(v)        ((v)->n != 0)
#define gc_vector_size(v)           ((v)->n)
#define gc_vector_max(v)            ((v)->m)
#define gc_vector_data(v)           ((v)->data)
#define gc_vector_free(v)           (free((v)->data),free(v))
#define gc_vector_type(v)           (*(v)->data)

// checks if array needs to resize and resizes if so
#define _gc_vector_grow(v)          (_gc_mustgrow(v,4) ? ((v)->data = gc_vector_grow_internals((v)->data, &(v)->m, sizeof(*(v)->data))) : 0)

#define _gc_inbounds(v,i)           ((0 <= i) && (i < (v)->n))      // checks if an index is in range
#define _gc_mustgrow(v, em)         ((v)->n + em >= (v)->m)     // checks if the array needs to grow
#define _gc_calc_nextsize(m)        ((m) ? m << 1 : 16)         // gets the next new size (usually powers of 2)

#define gc_binceil64(x) (--(x), (x) |= (x) >> 1, (x) |= (x) >> 2, (x) |= (x) >> 4, (x) |= (x) >> 8, (x) |= (x) >> 16, (x) |= (x) >> 32, ++(x))

void *gc_vector_grow_internals(void *data, size_t *currentM, const size_t typeSize)
{
    size_t newSize = _gc_calc_nextsize(*currentM);
    gc_binceil64(newSize);
    void *mem = realloc(data, typeSize * newSize);
    if (mem)
    {
        *currentM = newSize;
        return mem;
    }
    return data; // returns old data
}
#endif


/* 
MIT License

Copyright (c) 2025 GlenshC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */