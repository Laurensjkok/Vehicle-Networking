#define CAN_MSG_RX_PERIOD_1      (6750000000)  //137 symbols * 2 = 2 * Worst_Case_CAN_frame
#define CAN_MSG_RX_OFFSET_1      (3500000000)  //nr of tile clk cycles

#define CAN_MSG_RX_PRIOS_1                    4
int rxPrioFilt_1[CAN_MSG_RX_PRIOS_1] = {0,1,2,3};  //message ID filters

//#define CAN_MSG_RX_NUM_FILTERS_1                    1
//int rxPrioFilt_1[CAN_MSG_RX_NUM_FILTERS_1] = {-1};  //message ID filters
