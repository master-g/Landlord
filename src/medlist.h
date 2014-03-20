/*
 *  medlist.h
 *  Landlord
 *
 *  Created by Master.G on 14-3-30.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_MEDLIST_H_
#define LANDLORD_MEDLIST_H_

typedef enum
{
    MLPayload_NONE = 0
    
} MEDListPayloadType;

typedef struct medlist_t
{
    void *payload;
    struct medlist_t *next;
    
} medlist_t;


/* search function */
typedef int (*MEDListFunc_Find)(void* payload, void* context);

/* destroy function */
typedef void (*MEDListFunc_Delete)(void *payload);

/* functions */

medlist_t *MEDList_Create(void);

void MEDList_Destroy(medlist_t **l, MEDListFunc_Delete delfunc);

int MEDList_Length(medlist_t *l);

void MEDList_PushFront(medlist_t **l, medlist_t *node);

void MEDList_PushBack(medlist_t **l, medlist_t *node);

void MEDList_Remove(medlist_t **l, medlist_t *node);

medlist_t *MEDList_Find(medlist_t *l, void *context, MEDListFunc_Find finder);

void MEDList_Test(void);

#endif /* LANDLORD_MEDLIST_H_ */
