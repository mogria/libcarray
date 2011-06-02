/* Autor: mogria, see README & license.txt for more info */

#ifndef _CARRAY_H
#define _CARRAY_H

/* C++ Support */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> /* printf(), puts() */
#include <stdlib.h> /* malloc(), realloc(), free() */
#include "carray_types.h"

/* initalisize Array */
int initArray(Array *array);

/* Remove current element of the Array */
int ArrayRemove(Array *array);

/* Print the Array to stdout */
int ArrayPrint(Array *array);

/* Move pointer to the previous Element */
void ArrayPrev(Array *array);

/* Move pointer to the current element by the specified value */
int ArrayMove(Array *array, int by); 

/* Move pointer to the next Element */
void ArrayNext(Array *array);

/* returns a void pointer to the current element */
void *ArrayCurrent(Array *array);

/* set the current element to the last element */
int ArrayLast(Array *array);

/* set the current element to the last element */
int ArrayRewind(Array *array);

/* returns the amount of elements in the Array */
int ArrayCount(Array *array);

/* returns if the position in the array is valid */
int ArrayValid(Array *array);

/* prints the memory used by the array */
void DebugPrintArray(Array *array);

/* set the pointer to the current element to 
the element with the key given */
int ArraySeekByKey(Array *array, int key);

/* get the element, witch has the key given */
void *ArrayGet(Array *array, int key);

/* set the current element */
int ArraySet(Array *array, void *v);

/* append an element to the Array */
int ArrayAppend(Array *array, void *value);

/* get the key of the current element */
int ArrayKey(Array *array);

/* clear the array, for reuse */
int ArrayReset(Array *array);

/* free the array */
int ArrayFree(Array *array);

/* initalize the Array
Call tbis function directly after definition!
Example: 
Array a;
initArray(&a);
OR:
Array a = initArray(NULL);
*/
Array initArray(Array *array) {
  Array ret_array;
  if(array == NULL) {
    array = &ret_array;
  }
  array->last = NULL;
  array->current = NULL;
  array->first = malloc(0);
  array->elements = 0;
  return array;
}

/* Call this function if you don't need your
Array anymore
Example:
Array a = initArray(NULL);
//.. some code
ArrayFree(&a);
*/
int ArrayFree(Array *array) {
  free(array->first);
  return 0;
}

/* Call this function if you want to 
reinitalize your Array
Example:
Array a = initArray(NULL);
// .. some code
ArrayReset(&a);
// .. some code
ArrayFree(&a);
*/
int ArrayReset(Array *array) {
  ArrayFree(array);
  initArray(array);
  return 0;
}

/* Get the key of the current ArrayElement
Example:
int i = 5;
Array a = initArray(NULL);
ArrayAppend(&a, &i);
ArrayRewind(&a);
ArrayKey(&a); // will return 0
*/
int ArrayKey(Array *array) {
  return array->current->key;
}

/* Append an element at the end of the array
Example:
int i = 5;
Array a = initArray(&a);
ArrayAppend(&a, &i); // the array has now 1 element 
*/
int ArrayAppend(Array *array, void *value) {
  unsigned int index_save = array->elements;
  array->first = realloc(array->first,
                   (1 + array->elements) * sizeof(ArrayElement)
  );
  /* if(array->first == NULL) {
    puts("not enough memory!");
  }
  puts("enough memory");
  */
  ArrayElement *ptr_element = array->first + index_save;
  ptr_element->save_index = index_save;
  ptr_element->value = value;
  ptr_element->next = NULL;
  /* printf("if(array->elements(%i) > 0) {\n", array->elements); */
  if(array->elements > 0) {
    /* puts("if"); */
    array->last->next = ptr_element;
    ptr_element->prev = array->last;
    ptr_element->key = array->last->key + 1;
    array->last = ptr_element;
  } else {
    /* puts("else"); */
    array->current = array->last = ptr_element;
    ptr_element->prev = NULL;
    ptr_element->key = 0;
  }
  array->elements++;
  return 0;
}

/* Remove the current element in the Array
Example:
int i = 5;
Array a = initArray(&a);
ArrayAppend(&a, &i); // the array has now 1 element
ArrayRewind(&a);
ArrayRemove(&a);  // the array is now empty
*/
int ArrayRemove(Array *array) {
  int ret = 0;
  if(ArrayValid(array)) {
    ArrayElement *ptr_current = array->current;
    /*if(ptr_current->prev != NULL) {
      ptr_current->prev->next = ptr_current->next;
      ArrayPrev(array);
    }
    if(ptr_current->next != NULL) {
      ptr_current->next->prev = ptr_current->prev;
      ArrayNext(array);
    }*/
    int shift_size = array->elements - (ptr_current - array->first);
    int i = 0;
    /*
    printf("%i = shift_size = %i - ((%i - %i))\n",
            shift_size,
            array->elements,
            ptr_current,
          array->first);
    */
    /* printf("shift size: %i\n", shift_size); */
    while(i < shift_size - 1) {
      /* printf("shift #%i : %p < %p\n", i, ptr_current + i, ptr_current + i + 1); */
      *(ptr_current + i) = *(ptr_current + i + 1);
      (ptr_current + i)->save_index--;
      (ptr_current + i)->prev--;
      if((ptr_current + i)->next != NULL)
        (ptr_current + i)->next--;
      i++;
    }
    array->first = realloc(array->first, sizeof(ArrayElement) * --array->elements);
  } else {
    ret = 1;
  }
  return ret;
}

void *ArrayGet(Array *array, int key) {
    return (ArraySeekByKey(array, key) == 0) ? ArrayCurrent(array) : NULL;
}

/* set the current element to an anthoer value
Example:
int i = 5;
int i2 = 3;
Array a = initArray(&a);
ArrayAppend(&a, &i); // the array has now 1 element
ArrayRewind(&a);
ArraySet(&a, &i2); // the first element contains now 3
*/
int ArraySet(Array *array, void *value) {
  array->current->value = value;
}

int ArraySeekByKey(Array *array, int key) {
  ArrayElement *ptr_before = array->current;
  int found = 0;
  ArrayRewind(array);
  while(ArrayValid(array) && !found) {
    if(ArrayKey(array) == key) {
      found = 1;
    } else {
      ArrayNext(array);
    }
  }

  if(!found) {
    array->current = ptr_before;
    return 1;
  }
  return 0;
}

int ArrayCount(Array *array) {
  return array->elements;
}

int ArrayValid(Array *array) {
  return array->current != NULL;
}

int ArrayRewind(Array *array) {
  if(array->elements > 0) {
    array->current = array->first;
  }
}

int ArrayLast(Array *array) {
  if(array->elements > 0) {
    array->current = array->last;
  }
}

void *ArrayCurrent(Array *array) {
  return array->current->value;
}

void ArrayNext(Array *array) {
    array->current = array->current->next;
}

void ArrayPrev(Array *array) {
    array->current = array->current->prev;
}

int ArrayMove(Array *array, int by) {
  void (*ptr_move_func)(Array *array);
  int i;
  if(by > 0) {
    ptr_move_func = ArrayNext;
  } else {
    ptr_move_func = ArrayPrev;
    by = -by;
  }
  
  for(i = 0; i < by && ArrayValid(array); i++) {
    ptr_move_func(array);
  }
  return ArrayValid(array);
}

/* @TODO: zahlen und co. richtig ausgeben */
int ArrayPrint(Array *array) {
  ArrayElement *ptr = array->current;
  puts("\nArray (");
  for(ArrayRewind(array); ArrayValid(array); ArrayNext(array)){
    printf("\t%i => %i\n", ArrayKey(array), *(int *)ArrayCurrent(array));
  }
  array->current = ptr;
  puts(")");
  return 0;
}

void DebugPrintArray(Array *array) {
  int i;
  printf("elements: %i\n", array->elements);
  printf(" key    | value  | s idx  | next   | addr   |prev\n");
  for( i = 0; i < array->elements; i++) {
    ArrayElement *ptr_el = array->first + i;
    printf("%8X %p %8X %p %p %p\n", ptr_el->key, ptr_el->value, ptr_el->save_index, ptr_el->next, ptr_el, ptr_el->prev);
    puts("");
  }
}

/* C++ Support */
#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* _CARRAY_H */

