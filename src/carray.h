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
void ArrayLast(Array *array);

/* set the current element to the last element */
void ArrayRewind(Array *array);

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

/** @brief initialize the Array
 *
 * Call tbis function directly after definition!
 * Example:\n
 * Array a;\n
 * initArray(&a);\n
 * Example:\n
 * Array a = initArray(NULL);\n
 *
 * @return <Array> initialized Array
 * @param <Array *> pointer to Array which should be initialized, or NULL
 * @see ArrayFree()
 * @see ArrayReset()
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

/** @brief Call this function if you don't need your Array anymore
 *
 * Example:\n
 * Array a = initArray(NULL);\n
 * //.. some code\n
 * ArrayFree(&a);\n
 *
 * @param <Array *> pointer to array which will be freed
 * @return <int> errcode
 * @see initArray()
 * @see ArrayReset()
 */
int ArrayFree(Array *array) {
  free(array->first);
  return 0;
}

/** @brief Call this function if you want to reinitalize your Array
 *
 * Example:\n
 * Array a = initArray(NULL);\n
 * // .. some code\n
 * ArrayReset(&a);\n
 * // .. some code\n
 * ArrayFree(&a);\n
 *
 * @return <int> errcode
 * @param <Array *> pointer to Array wich will be reseted
 * @see initArray()
 * @see ArrayFree()
 */
int ArrayReset(Array *array) {
  ArrayFree(array);
  initArray(array);
  return 0;
}

/** @brief Get the key of the current ArrayElement 
 *
 * Example:\n
 * int i = 5;\n
 * Array a = initArray(NULL);\n
 * ArrayAppend(&a, &i);\n
 * ArrayRewind(&a);\n
 * ArrayKey(&a); // will return 0\n
 *
 * @return <int> key of the current Array Element
 * @param <Array *> Pointer to the Array 
 * @see ArrayCurrent()
 * @see ArrayGet()
 */
int ArrayKey(Array *array) {
  return array->current->key;
}

/** @brief Append an element at the end of the array
 *
 * Example:\n
 * int i = 5;\n
 * Array a = initArray(&a);\n
 * ArrayAppend(&a, &i); // the array has now 1 element\n
 *
 * @return <int> errcode
 * @param <Array *> Pointer to the Array
 * @param <void *> Pointer to the value
 * @see ArrayRemove()
 * @see ArraySet()
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

/** @brief Remove the current element in the Array
 *
 * Example:\n
 * int i = 5;\n
 * Array a = initArray(&a);\n
 * ArrayAppend(&a, &i); // the array has now 1 element\n
 * ArrayRewind(&a);\n
 * ArrayRemove(&a);  // the array is now empty\n
 *
 * @return errcode
 * @param <Array *> Pointer to the Array
 * @see ArrayAppend()
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
    while(i < shift_size - 1) {
      *(ptr_current + i) = *(ptr_current + i + 1);
      (ptr_current + i)->save_index--;
      (ptr_current + i)->prev--;
      if((ptr_current + i)->next != NULL)
        (ptr_current + i)->next--;
      i++;
    }
    array->first =
      realloc(array->first, sizeof(ArrayElement) * --array->elements);
  } else {
    ret = 1;
  }
  return ret;
}

/** @brief get an element, specified by the key, from the array
 *
 * Example:\n
 * int i = 5;\n
 * Array a = initArray(&a);\n
 * ArrayAppend(&a, &i);\n\n
 * ArrayGet(&a, 0); //returns a pointer to the element with the key 0
 *
 * @param <Array *> Pointer to the Array
 * @param <int> key of the element in array
 * @return <void *> Pointer to the element , or NULL if it does'nt exist
 * @see ArrayCurrent()
 * @see ArraySet()
 */
void *ArrayGet(Array *array, int key) {
    return (ArraySeekByKey(array, key) == 0) ? ArrayCurrent(array) : NULL;
}

/** @brief Set the current element to an anthoer value
 *
 * Example:\n
 * int i = 5;\n
 * int i2 = 3;\n
 * Array a = initArray(&a);\n
 * ArrayAppend(&a, &i); // the array has now 1 element\n
 * ArrayRewind(&a);\n
 * ArraySet(&a, &i2); // the first element contains now 3\n
 *
 * @todo add param "key", write function ArraySetCurrent
 */
int ArraySet(Array *array, void *value) {
  array->current->value = value;
}
 
/** @brief Select current element by Key
 *
 * @param <Array *> Pointer to the Array
 * @param <int> key
 * @return <int> errcode
 * @see ArrayGet()
 * @see ArrayNext()
 * @see ArrayPrev()
 * @see ArrayRewind()
 * @see ArrayLast()
 * @see ArrayMove()
 */
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

/** @brief get number of elements in Array
 *
 * @param <Array *> Pointer to the Array
 * @return <int> num of elements
 */
int ArrayCount(Array *array) {
  return array->elements;
}

/** @brief checks if the current element of the array is in a valid position
 *
 * @param <Array *> pointer to the Array
 * @return <int> 0 - not valid, !0 - valid
 * @see ArrayRewind()
 * @see ArrayLast()
 * @see ArrayNext()
 * @see ArrayPrev()
 */
int ArrayValid(Array *array) {
  return array->current != NULL;
}

/** @brief Rewind Array pointer to first element of the Array
 *
 * @param <Array *> Pointer to the Array
 * @see ArrayValid()
 * @see ArrayLast()
 * @see ArrayNext()
 * @see ArrayPrev()
 */
void ArrayRewind(Array *array) {
  if(array->elements > 0) {
    array->current = array->first;
  }
  return;
}

/** @brief Set the Array pointer to the last element of the Array
 *
 * @param <Array *> Pointer to the Array
 * @see ArrayValid()
 * @see ArrayRewind()
 * @see ArrayNext()
 * @see ArrayPrev()
 */
void ArrayLast(Array *array) {
  if(array->elements > 0) {
    array->current = array->last;
  }
  return;
}

/** @brief get the pointer to the value of the current element
 *
 * @param <Array *> Pointer to the Array
 * @return <void *> void pointer to the value
 * @see ArrayGet()
 */
void *ArrayCurrent(Array *array) {
  return array->current->value;
}

/** @brief Move element pointer to next element
 * @param <Array *> Pointer to the Array
 * @see ArrayValid()
 * @see ArrayLast()
 * @see ArrayRewind()
 * @see ArrayPrev()
 */
void ArrayNext(Array *array) {
    array->current = array->current->next;
}

/** @brief move element pointer to previous element
 * @param <Array *> Pointer to the Array
 * @see ArrayValid()
 * @see ArrayLast()
 * @see ArrayNext()
 * @see ArrayRewind()
 */
void ArrayPrev(Array *array) {
    array->current = array->current->prev;
}

/** @brief move the element pointer 
 *
 * @param <int> move amount
 * @param <Array *> Pointer to the Array
 * @return <int> Valid position in Array after Operation
 * @see ArrayNext()
 * @see ArrayPrev()
 * @see ArrayRewind()
 * @see ArrayLast()
 */
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

/** @brief Print the Array
 *
 * @param <Array *> the Array
 * @return <void>
 */
vid ArrayPrint(Array *array) {
  current;
  puts("\nArray (");
  for(ArrayRewind(array); ArrayValid(array); ArrayNext(array)){
    printf("\t%i => %i\n", ArrayKey(array), *(int *)ArrayCurrent(array));
  }
  array->current = ptr;
  puts(")");
}

/** @brief print all the memory needed by the Array
 * @param <Array *> the Array
 * @return <void>
 */
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

