#include "entity_packet.h"

#ifdef C_ENTITY_FRAMEWORK_LIB_ENA

#include "entity_manager.h"
////------------------- Packet formaters function ----------------------------------
////basic
//void read_entities_descriptions(u16 startEntityNum, u8 len, u8 *outputData, u16 *size)
//{   //send {entities[0].descr, ... entities[entities_count].descr}

//    if() {

//    }

//    u16 pos = 0;
//    u8 cmd = DATA_CMD;

//    if(entities && (startEntityNum < entities_count)) {
//        to_pos_copy(1, &cmd, outputData, &pos, ENTITY_DIR_COPY);

//        cmd = ENTITY_DECRIPTION_LEN;
//        to_pos_copy(1, &cmd, outputData, &pos, ENTITY_DIR_COPY);

//        cmd = MIN((entities_count - startEntityNum), len);
//        to_pos_copy(1, (u8 *)&cmd, outputData, &pos, ENTITY_DIR_COPY);

//        cmd = cmd + startEntityNum;

//        for(u32 i = startEntityNum; i < cmd; ++i) {
//            to_pos_copy(ENTITY_DECRIPTION_LEN, (u8 *)entities[i]->descr, outputData, &pos, ENTITY_DIR_COPY);
//        }
//    } else {
//        cmd = NULL_CMD;
//        to_pos_copy(1, &cmd, outputData, &pos, ENTITY_DIR_COPY); //send 0x00
//    }

//    *size = pos;
//}


#endif /* C_ENTITY_FRAMEWORK_LIB_ENA */
