#define CAN_MSG_RX_PERIOD_3      (6750000000)  //137 symbols * 2 = 2 * Worst_Case_CAN_frame
#define CAN_MSG_RX_OFFSET_3      (3500000000)  //nr of tile clk cycles

//#define CAN_MSG_RX_NUM_FILTERS_3                    4
//int rxPrioFilt_3[CAN_MSG_RX_NUM_FILTERS_3] = {0,1,2,3};   //message ID filters

#define CAN_MSG_RX_PRIOS_3                    1
int rxPrioFilt_3[CAN_MSG_RX_PRIOS_3] = {-1};   //message ID filters

