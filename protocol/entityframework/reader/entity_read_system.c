/*--------------------------------------- FILE MUST BE GENERATED (DO NOT EDIT!!!) --------------------------------------------*/

#include "entity_read_system.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#ifdef USE_ENTITY_READ_SERVICE

#include "preprocessor_ctx.h"
#include "smart_assert.h"

#define EN_RD_SYS_ASSERT(expr)                  \
    if((expr) != ENTITY_OK) {                   \
        return ENTITY_ERROR;                    \
    }

EntityReadSystem_t ersys = {
    /* BEGIN GENERATION CODE */
    ENTITY_CREATE_READ_CHIELD_BODY(u8, name, 0, 1, 2, 3, &ersys.name.data, &ersys.writePool[0], 1),
    ENTITY_CREATE_READ_CHIELD_BODY(u16, name1, 0, 1, 2, 3, &ersys.name1.data, &ersys.writePool[0], 1),

    /* END GENERATION CODE */
    .writePool  = {},
    .readPool   = {}
};


static int parentPointersCheck(EntityReadParent_t* const field, PREPROCESSOR_CTX_TYPE(ctx))
{
    M_Assert_BreakSaveCheck(field == NULL || ctx == NULL, M_EMPTY, return 1, "parentPointersCheck: pointer or context data is null!!!");
    PREPROCESSOR_CTX_GET(ctx,
                                 int* const errorInit,
                                 int* const allfieldsIsNull);

    *allfieldsIsNull = 0;

    M_Assert_BreakSaveCheck(    field->writeContainer == NULL
                                ||  field->writeContainer->msk == 0
                                ||  field->writeContainer->writePool == NULL, M_EMPTY, ++(*errorInit), "parentPointersCheck: parent write container is null!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);

    M_Assert_BreakSaveCheck(field->boardNumber >= ENTITY_READ_SYSTEM_BOARD_COUNT, M_EMPTY, ++(*errorInit), "parentPointersCheck: board number is invalid!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);
    M_Assert_BreakSaveCheck(field->entityNumber >= MAX_NUBER_OF_ENTITIES, M_EMPTY, ++(*errorInit), "parentPointersCheck: entity number is invalid!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);
    M_Assert_BreakSaveCheck(field->fieldNumber >= MAX_NUBER_OF_FIELDS, M_EMPTY, ++(*errorInit), "parentPointersCheck: field number is invalid!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);
    M_Assert_BreakSaveCheck(field->type >= TYPE_ARRAY_LENGTH, M_EMPTY, ++(*errorInit), "parentPointersCheck: type is invalid!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);

    M_Assert_BreakSaveCheck(    field->writeContainer == NULL
                                ||  field->writeContainer->msk == 0
                                ||  field->writeContainer->writePool == NULL, M_EMPTY, ++(*errorInit), "parentPointersCheck: parent write container is null!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);

    M_Assert_BreakSaveCheck(field->data == NULL, M_EMPTY, ++(*errorInit), "parentPointersCheck: parent data is null!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);
    M_Assert_BreakSaveCheck(field->set == NULL, M_EMPTY, ++(*errorInit), "parentPointersCheck: set function is invalid!!!, board: %d, entity: %d, field: %d", field->boardNumber, field->entityNumber, field->fieldNumber);

    field->onValueNotUpdated = 1;

    return 0; // next counting parents
}

int initEntityReadSystem(void)
{
    /* BEGIN GENERATION CODE */

    TYPEOF_STRUCT(EntityReadPoolContainer_t, boards_count) boards_num = 0;
    TYPEOF_STRUCT(Entity, fields_count) fieldCountList[3] = {1, 2, 3};

    /***********************************************************************
     * Read container initialization (for fast search field)
     */
    // init all boards----------------------------------------------------------------------------------------------------------------------
    EN_RD_SYS_ASSERT( entityReadPoolContainer_initBoards(&ersys.readPool, ENTITY_READ_SYSTEM_BOARD_COUNT) );  // init all boards
    // init each board----------------------------------------------------------------------------------------------------------------------
    EN_RD_SYS_ASSERT( entityReadPoolContainer_initBoard(&ersys.readPool, &boards_num, 3, fieldCountList, ARRAY_COUNT(fieldCountList)) );
    // all parent push to container---------------------------------------------------------------------------------------------------------
    EN_RD_SYS_ASSERT( entityReadPoolContainer_push(&ersys.readPool, &ersys.name.parent) );
    EN_RD_SYS_ASSERT( entityReadPoolContainer_push(&ersys.readPool, &ersys.name1.parent) );

    /***********************************************************************
     * Write container initialization (for only write changed fields)
     */

    EN_RD_SYS_ASSERT( entityWritePoolContainer_init(&ersys.writePool[0], 2) );

    /* END GENERATION CODE */

    // check all pointers--------------------------------------------------------------------------------------------------------------
    int errorInit = 0;
    int allfieldsIsNull = 1;
    entityReadPool_foreach(&ersys.readPool, parentPointersCheck, PREPROCESSOR_CTX_CAPTURE({&errorInit, &allfieldsIsNull}));

    if(allfieldsIsNull) {
        return ENTITY_ERROR;
    }

    if(errorInit == 0) {
        return ENTITY_OK;
    }

    return ENTITY_ERROR;

    ersys.name.set(&ersys.name, 230);
        //ersys.name.parent.
}


#undef EN_RD_SYS_ASSERT


#endif /* USE_ENTITY_READ_SERVICE */

#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
/*--------------------------------------- FILE MUST BE GENERATED (DO NOT EDIT!!!) --------------------------------------------*/
