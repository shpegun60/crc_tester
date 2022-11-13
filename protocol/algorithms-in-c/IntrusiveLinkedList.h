/* Copyright (C) 2017, kylinsage <kylinsage@gmail.com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef INTRUSIVE_LINKED_LIST_H
#define INTRUSIVE_LINKED_LIST_H

#include <stdlib.h>

/* Offset of member MEMBER in a struct of type TYPE. */
#ifndef offsetof
#   define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/*
 * Circular Intrusive Doubly Linked List Implementation.
 * The next reference inside a node can be viewed as a link or pointer to
 * another node.  We define that the POSITION of current node is the pointer to
 * the previous node. We can traverse the list starting at the head. The tail
 * node is a special node, where the next pointer is always pointing or linking
 * to a null reference, indicating the end of the list.
 * Dummy header is used because they help with the implementation.
 */

typedef struct _IntrusiveDListNode {
    struct _IntrusiveDListNode *prev;
    struct _IntrusiveDListNode *next;

} IntrusiveDListNode;

typedef IntrusiveDListNode IntrusiveDList;

/**
 * idlist_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define idlist_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-offsetof(type, member)))

/**
 * idlist_for_each - iterate over a list
 * @position:  used as a loop cursor.
 * @head: the head for your list.

 * NOTE:  We can change list elements of a list when use this macro, but
 * we should not change links or pointers of the list.
 */
#define idlist_for_each(position, head) \
    for(IntrusiveDListNode *position = (head); \
    position->next != (head); \
    position = position->next)

/**
 * idlist_init - initialize a list
 * @head: the head for your list.
 */
static inline void idlist_init(IntrusiveDList * head)
{
    head->prev = head->next = head;
}

/**
 * idlist_insert_back - insert a node at a specific position
 * @position: list position to add it after
 * @element: pointer to element
 */
void idlist_insert_back(IntrusiveDListNode * position,
                       IntrusiveDListNode * node);

/**
 * dlist_remove - remove a node at a specific position
 * @position: list position to remove it after
 *
 * NOTE: The node is deleted and retruns the pointer to element.
 */
void idlist_remove_back(IntrusiveDListNode * position);

/**
 * idlist_size - count size of list nodes, not including dummy head
 * @head: list to be counted
 */
static inline size_t idlist_size(IntrusiveDList * head)
{
    size_t count = 0;
    idlist_for_each(position, head) {
        count++;
    }
    return count;
}

/**
 * idlist_is_empty - returns true if list is empty
 * @q: list to be check
 */

#define idlist_is_empty(q) ((q)->prev == (q) && (q)->next == (q))

/**
 * idlist_destroy - destroy list node containers, not including dummy head
 * @head: list to be destroyed
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 * @destroy:    the destructor of the struct wherein link is embedded.
 *
 * NOTE: It is the responsibilities of programmers to mamage the memory of
 * dummy head.
 */
#define idlist_destroy(head, type, member, destroy) \
{ \
    IntrusiveDListNode *position = head; \
    while(position->next != (head)) { \
        type *element = idlist_entry(position->next, type, member); \
        idlist_remove_back(position); \
        destroy(element); \
    } \
}

/*
 * Intrusive Singly Linked List Implementation.
 * The next reference inside a node can be viewed as a link or pointer to
 * another node.  We define that the POSITION of current node is the pointer to
 * the previous node. We can traverse the list starting at the head. The tail
 * node is a special node, where the next pointer is always pointing or linking
 * to a null reference, indicating the end of the list.
 * Dummy header is used because they help with the implementation.
 */

typedef struct _IntrusiveSListNode {
    struct _IntrusiveSListNode *next;

} IntrusiveSListNode;

typedef IntrusiveSListNode IntrusiveSList;

/**
 * islist_entry - get the struct for this entry
 * @ptr:    the &struct list node pointer.
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define islist_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-offsetof(type, member)))

/**
 * islist_entry_of_position - get the struct for this entry
 * @position:    the &struct list position (pointer to previous node).
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 */
#define islist_entry_of_position(position, type, member) \
    islist_entry(position->next, type, member)

/**
 * islist_for_each - iterate over a list
 * @position:  used as a loop cursor.
 * @head: the head for your list.

 * NOTE:  We can change list elements of a list when use this macro, but
 * we should not change links or pointers of the list.
 */
#define islist_for_each(position, head) \
    for(IntrusiveSListNode *position = (head); \
    position->next != NULL; \
    position = position->next)

/**
 * islist_init - initialize a list
 * @head: the head for your list.
 */
static inline void islist_init(IntrusiveSList * head)
{
    head->next = NULL;
}

/**
 * islist_insert_back - insert a node at a specific position
 * @position: list position to add it after
 * @element: pointer to element
 */
void islist_insert_back(IntrusiveSListNode * position,
                       IntrusiveSListNode * node);

/**
 * islist_remove - remove a node at a specific position
 * @position: list position to remove it after
 *
 * NOTE: The node is deleted and retruns the pointer to element.
 */
void islist_remove_back(IntrusiveSListNode * position);

/**
 * islist_size - count size of list nodes, not including dummy head
 * @head: list to be counted
 */
static inline size_t islist_size(IntrusiveSList * head)
{
    size_t count = 0;
    islist_for_each(position, head) {
        count++;
    }
    return count;
}

/**
 * islist_is_empty - returns true if list is empty
 * @q: list to be check
 */
#define islist_is_empty(s) ((s)->next == NULL)

/**
 * islist_destroy - destroy list node containers, not including dummy head
 * @head: list to be destroyed
 * @type:    the type of the struct this is embedded in.
 * @member:    the name of the list_struct within the struct.
 * @destroy:    the destructor of the struct wherein link is embedded.
 *
 * NOTE: It is the responsibilities of programmers to mamage the memory of
 * dummy head.
 */
#define islist_destroy(head, type, member, destroy) \
{ \
    IntrusiveSListNode *position = head; \
    while(position->next) { \
        type *element = islist_entry(position->next, type, member); \
        islist_remove_back(position); \
        destroy(element); \
    } \
}

#endif /* INTRUSIVE_LINKED_LIST_H */
