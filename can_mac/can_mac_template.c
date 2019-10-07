#include <can_driver_mac_types.h>
#include <can_phy.h>

static void hw_can_mac_driver(
		       volatile CAN_PORT *can_port_id,
		       CAN_FRAME * volatile *TxFrameFromSensor,
		       CAN_FRAME * volatile *RxFrameForActuator,
		       int *rxPrioFilters, uint rxPrioFiltersLen)
{
  CAN_FRAME TxFrame, RxFrame;
  bool newFrameFromSensor;
  CAN_SYMBOL TxSymbol, RxSymbol;
  
  while (1) {
    /* if (*rxPrioFilters) < 0 then we're master else slave
       as a master, to get the next frame to send from the sensor use:
       newFrameFromSensor = can_mac_rx_next_frame(TxFrameFromSensor, &TxFrame);
       as a slave, to send a received frame with priority rxPrioFilter to the actuator use:
       can_mac_tx_next_frame(RxFrameForActuator, &RxFrame);
    */
    
    /* to send a CAN symbol on the CAN bus use:
       can_phy_tx_symbol(can_port_id, TxSymbol)
       to receive a CAN symbol from the CAN bus use:
       can_phy_rx_symbol_blocking(can_port_id,&RxSymbol)
       this function blocks until a new symbol is available on the bus
    */
    newFrameFromSensor = can_mac_rx_next_frame(TxFrameFromSensor, &TxFrame);
    if (newFrameFromSensor == true) {
      /* if you wish to send different test sequences from different sensors,
       * then this is how you can do this. Note that this should not be used in the final driver,
       * since there all sensors must be treated the same.
       */
      if (TxFrame.ID == 0) {
        /* sensor 1 with priority/ID 0 */
        can_phy_tx_symbol(can_port_id, DOMINANT);
        can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
        can_phy_tx_symbol(can_port_id, DOMINANT);
        can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
        can_phy_tx_symbol(can_port_id, RECESSIVE);
        can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
      }
      else { 
        /* the other active sensors */
        can_phy_tx_symbol(can_port_id, RECESSIVE);
        can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
      }
    }
  }
}
