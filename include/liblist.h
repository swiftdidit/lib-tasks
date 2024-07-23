#ifndef __LIB_HEADER__
#define __LIB_HEADER__

#include <stdio.h>
#include <stdbool.h>

/*
    -> #foreach(void *i, List *list);
    -> Example usage:
    List *list = NewList(NULL);

    foreach(void *i, list) {
        printf("%d\n", *(int*)i);  // Assuming 'i' is an integer pointer.
        // Do something with 'i'...
        // You can also use the following keywords as you would expect from a
   for loop. These keywords are 'continue', 'break', 'return'.
    }
*/
#define foreach(element, list)                                    \
  for (size_t keep = 1, count = 0; keep && count != (list)->size; \
       keep = !keep, count++)                                     \
    for (element = (list)->data[count]; keep; keep = !keep)

typedef struct List {
  void **data;
  size_t size;
  size_t capacity;
} List;

/*
 TODO:
   Just ideas here..
*/

// Creates a new empty List
List *NewList(List *optional);

// Free memory allocated by List
void ListFreeMemory(List *list);

// Add an element to the List
void ListAdd(List *list, void *data);

// Add an array of elements to the List
void ListAddArray(List *list, void **data);

/*
    -> #ListAddAll(List *dest, List* ...)
    -> Adds all List* struct's from the provided variadic arguments to the end
   of this list. \ Basically combining the provided lists... into the 'dest'
   List* struct
    -> Example usage:

    // let's pretend that you already have data inside these lists...
    List *list1 = NewList(NULL);
    List *list2 = NewList(NULL);

    List *theUltimateList = NewList(NULL);
    ListAddAll(theUltimateList, list1, list2);
*/
void ListAddAll(List *dest, ...);

// Set an element to the specified index and return the element
void *ListSet(List *list, unsigned int index, void *data);

// Remove the element from the List by index and return the element
void *ListRemoveIndex(List *list, unsigned int index);

// Remove the element from the List and return the element
void *ListRemove(List *list, void *data);

// Removes all of the elements from this list.
void ListClear(List *list);

// Returns true if the list contains the element passed, or else false.
bool ListContains(List *list, void *data);

/*
    -> #ListFilter()
    -> Returns a new list consisting of the elements of this provided list
   that match the provided predicate.
    -> Example usage:

    bool IsEven(void *data) {
        int value = *((int *)data);
        return (value % 2 == 0);
    }

    List *evenNumbers = ListFilter(list, IsEven);

    printf("Even numbers in the 'evenNumbers' #List:\n");
    for (int i = 0; i < evenNumbers->size; i++) {
        printf(" - %d\n", *((int *)ListGet(evenNumbers, i)));
    }
*/
List *ListFilter(List *list, bool (*predicate)(void *));

/* Returns the first occurrence of the element in the list,
 based on the provided predicate, or NULL if not found. */
void *ListFind(List *list, bool (*predicate)(void *));

/* Sort the elements of the list according to the provided predicate */
void ListSort(List *list, int (*compare)(const void *, const void *));

/* Shuffle the list's elements */
void ListShuffle(List *list);

/* Gets element at index or (THROWS EXCEPTION if index is out of bounds) and
 * return it. */
void *ListGet(List *list, unsigned int index);

/* Return the element at index or(THROWS EXCEPTION if data is NULL and will
 * return -1) */
int ListIndexOf(List *list, void *data);

// Removes last element from List and returns it.
void *ListPop(List *list);

// Gets first element from List and returns it.
void *ListFirst(List *list);

// Gets last element from List and returns it.
void *ListLast(List *list);

// Gets next element after passed (void *) data parameter and return it.
void *ListNext(List *list, void *data);

// Get previous element before passed (void *) data parameter and return it.
void *ListPrev(List *list, void *data);

// Returns the number of elements from the given List.
size_t ListSize(List *list);

// Returns true if the map is empty, false otherwise.
bool isListEmpty(List *list);

// Copy elements from source List to destination List
void ListCopy(List *source, List *dest);

#endif  //__LIB_HEADER__