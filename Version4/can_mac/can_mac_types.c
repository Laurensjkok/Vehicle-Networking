#include <mk_types.h>
#include <can_driver_mac_types.h>


static bool can_mac_rx_next_frame(CAN_FRAME * volatile *ppTxFrame, CAN_FRAME * pTxFrame)
{
	bool dataUpdated = false;

	if(*ppTxFrame != NULL)
	{
		*pTxFrame = **ppTxFrame;
		*ppTxFrame = NULL;
		dataUpdated = true;
	}

	return dataUpdated;
}

static void can_mac_tx_next_frame(CAN_FRAME * volatile *ppRxFrame, CAN_FRAME * pRxFrame)
{

	if(*ppRxFrame == NULL)
	{
		*ppRxFrame = pRxFrame;
	}
}
