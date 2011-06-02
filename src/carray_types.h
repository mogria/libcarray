/* Autor: mogria, see README & license.txt for more info */

#ifdef _CARRAY_TYPES_H
#define _CARRAY_TYPES_H 1

/* C++ Support */
#ifdef __cplusplus
extern "C" {
#endif

/* stores all data for an Array Element */
typedef struct ArrayElement {
  int key;
  unsigned int save_index; /* index on the element is saved */
  void *value; /* pointer to the value */
  struct ArrayElement *next; /* pointer to the next Array element */
  struct ArrayElement *prev; /* pointer to the previous Array element */
} ArrayElement;

/* represents an array: saves the size and some
pointers to ArrayElements */
typedef struct Array {
  size_t elements; /* num elements in Array */
  ArrayElement *first; /* pointer to first Element & pointer given from malloc() */
  ArrayElement *last; /* pointer to last Element */
  ArrayElement *current; /* pointer to current Element */
} Array;

/* C++ Support */
#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* _CARRAY_TYPES_H */
