#define CAN_MSG_RX_PERIOD_2      (6750000000)  //137 symbols * 2 = 2 * Worst_Case_CAN_frame
#define CAN_MSG_RX_OFFSET_2      (3500000000)  //nr of tile clk cycles

#define CAN_MSG_RX_PRIOS_2   4
int rxPrioFilt_2[CAN_MSG_RX_PRIOS_2] = {-1};   //message ID filters

// #define CAN_MSG_RX_PRIOS_2   4
// int rxPrioFilt_2[CAN_MSG_RX_PRIOS_2] = {0,1,2,3};   //message ID filters

// #define CAN_MSG_RX_NUM_FILTERS_2   1
// int rxPrioFilt_2[CAN_MSG_RX_NUM_FILTERS_2] = {-1};   //message ID filters
