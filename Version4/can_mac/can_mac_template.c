#include <can_driver_mac_types.h>
#include <can_phy.h>
#define IdLength 11
#define MaxDataLength 64


static void hw_can_mac_driver(
		       volatile CAN_PORT *can_port_id,
		       CAN_FRAME * volatile *TxFrameFromSensor,
		       CAN_FRAME * volatile *RxFrameForActuator,
		       int *rxPrioFilters, uint rxPrioFiltersLen)
{
  CAN_FRAME TxFrame, RxFrame;
  bool newFrameFromSensor;
  CAN_SYMBOL TxSymbol, RxSymbol;
  
  bool frame[135];
bool framestart[15];
bool bindata[MaxDataLength];
bool idbin[IdLength];
bool checksum[15];
bool DLCbin[4];
bool polynomial[16] = {1,1,0,0,0,1,0,1,1,0,0,1,1,0,0,1};
int iddec;
int EndOfData;
int numbytes;
int stuffedlength;
unsigned long long data;


void stuffing()
{
	int insertedbits = 0;
	int c, ins;
	int n = 122;
	int val;
	int i = 0;
	while(i<122){
		val = frame[i];
		if ((val == frame[i+1]) && (val == frame[i+2]) && (val == frame[i+3]) && (val == frame[i+4])){
		ins = 1 - frame[i];
		insertedbits++;
		for (c = n - 1; c >= (i+5) - 1; c--) {  
			frame[c+1] = frame[c];
		}
		frame[i+5] = ins;
		}
		if (i >= (insertedbits + EndOfData + 13)){
			break;
		}
		i++;
	}
	stuffedlength = EndOfData + 15 + insertedbits; //Indicates index of CRC delimiter bit
	
}

void CRC(int Data_end)
{	//Very inefficient CRC implementation! Can no doubt be improved
	bool checkdata[98];
	int k;
	for (int j = 0; j < Data_end; j++){
		checkdata[j] = frame[j];
	}
	for (int i = (Data_end + 1); i < (Data_end + 1 + 15); i++){
		checkdata[i] = 0;
		
	}
	while (k<(Data_end + 1)){
		if (checkdata[k]==0){
			k++;
		}
		else{
			for (int l=0; l<16; l++)
			{
				checkdata[k+l] = checkdata[k+l] ^ polynomial[l];
			}
	
		}
	}
	for (int m = 0; m<15; m++){
		checksum[m] = checkdata[(Data_end + 1)+m];
	}
}
	

/*
int RoundUp(double input)
{
	int whole;
	double rem;
	int output;
	whole = (int)input;
	rem = input - (double)whole;
	if (rem == 0){
		output = whole;
	}
	else{
		output = whole + 1;
	}
	return output;
}*/

void iddec2bin()
{
	int n = iddec;
	int i = 0;
	while (n > 0)
	{
		idbin[IdLength-i -1 ] = n % 2;
		n = n/2;
		i++;
	}
}

void DLCdec2bin(int n)
{
	int i = 0;
	while (n > 0)
	{
		DLCbin[3-i] = n % 2;
		n = n/2;
		i++;
	}
}


void datadec2bin()
{
data = TxFrame.Data;
	unsigned long long n = data;
 
 mk_mon_debug_info(n);
	int i = 0;
	int k = 0;
	int LengthOfDataField = 0;
	double dnumbytes = 0;
	while (n > 0)
	{
		bindata[MaxDataLength-i -1 ] = n % 2;
		n = n/2;
		i++;
     
	}
//	i = 0;
/*	while (k == 0)
	{
		if (i == 63 && bindata[i] == 0){
			LengthOfDataField = 0;
			break;
		}
		else{
		k = bindata[i];
		LengthOfDataField = MaxDataLength - i;
		i++;
		}
	}
	
	dnumbytes = (double)LengthOfDataField / 8;
	LengthOfDataField = RoundUp(dnumbytes); */
//	int BitsOfDataField = LengthOfDataField * 8;
//	bool DataOut[BitsOfDataField];
//	for (i = 0; i < LengthOfDataField; ++i)
//	{
//		DataOut[BitsOfDataField -i -1] = bindata[MaxDataLength -i -1];
//	}
//	dnumbytes2 = (dnumbytes + 0.5);
//	printf("%f \n", dnumbytes2);
//	inumbytes = dnumbytes2;
//	printf("%d \n", inumbytes);
//	printf("%d \n", LengthOfDataField);
//	return LengthOfDataField;
	
}

void make_frame()
{
	iddec = TxFrame.ID;
	iddec2bin();
	datadec2bin();
  DLCdec2bin(TxFrame.DLC);

  numbytes = TxFrame.DLC;

	
	EndOfData = (19+(8*numbytes)); // Not really end of data. Indicates first bit of CRC field
	frame[0] = 0;
	for (int i = 1; i < (IdLength + 1); ++i)
	{
		frame[i] = idbin[i-1];
	}
	for (int i = 12; i < 15; ++i)
	{
		frame[i] = 0;
	}
	for (int i = 15; i < 19; ++i)
	{
		frame[i] = DLCbin[i-15];
   mk_mon_debug_info(frame[i]);
	}
	for (int i = 19; i < EndOfData; ++i)
	{
		frame[i] = bindata[MaxDataLength - (8*numbytes) + i - 19];
 //     mk_mon_debug_info(frame[i]);
	}
 CRC((EndOfData -1));
	for (int i = EndOfData; i < (EndOfData+15);++i){
		frame[i] = checksum[i-EndOfData];
	}
	stuffing();
	for (int i = stuffedlength; i<(stuffedlength+13); ++i){
		frame[i] = 1;
	}
   frame[stuffedlength+2] = 0;    //REMOVE THIS LINE WHEN ACK IS PROPERLY IMPLEMENTED
}

bool send_frame(){
  for (int j = 0; j<stuffedlength+13; j++){
    if (frame[j] == 0){
       can_phy_tx_symbol(can_port_id, DOMINANT);
       can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
    }
    else
    {
       can_phy_tx_symbol(can_port_id, RECESSIVE);
       can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
    }
    
     if (RxSymbol != frame[j]){
       return 0;
       break;
     }
  }
  
  return 1;
}
 
 
 void queue_sending(int timeout){ // Timeout in number of symbols
   int symbolcount;
   int current_recessive;
   bool framestatus;
   while (symbolcount < timeout){
       can_phy_rx_symbol_blocking(can_port_id,&RxSymbol);
       symbolcount++;
       if (RxSymbol == 1){
         current_recessive++;
       }
       else{
         current_recessive = 0;
       }
       if (current_recessive >= 11){
         framestatus = send_frame();
         if (framestatus == 0){
           current_recessive = 0;
         }
         else{
           
           return 0;
         }
       }
   }
 
 }
 /*
 void resetFrame(){
	 memset(frame, 0, sizeof(frame));//set frame to zeros	
}

int DLCbin2dec(){
	DLCdec = 0;
	int N = 1;
	for(int i=18; i>14; i--){
		if(frame[i]==0){
		DLCdec = DLCdec + N;
		N = 2*N;
		}
		mk_mon_debug_info(DLCdec);
	}
	int lenghtToAck = 19+(DLCdec*8)+16;
	return lenghtToAck;
}
 */ 


if ((*rxPrioFilters) < 0){ //then we're master else slave
	while(1){
		newFrameFromSensor = can_mac_rx_next_frame(TxFrameFromSensor, &TxFrame);
        if (newFrameFromSensor == 1){
			make_frame();
			queue_sending(1000);
		}
	}
}


else{// you are actuator
  

}



/*
 while (1) {
      if ((*rxPrioFilters) < 0){ //then we're master else slave
      // as a master, to get the next frame to send from the sensor use:
         newFrameFromSensor = can_mac_rx_next_frame(TxFrameFromSensor, &TxFrame);
         if (newFrameFromSensor == 1){
  
         make_frame();
         queue_sending(1000);

       
         }
      // as a slave, to send a received frame with priority rxPrioFilter to the actuator use:
      }
      else{
 //      can_mac_tx_next_frame(RxFrameForActuator, &RxFrame);
    
     to send a CAN symbol on the CAN bus use:
       can_phy_tx_symbol(can_port_id, TxSymbol)
       to receive a CAN symbol from the CAN bus use:
       can_phy_rx_symbol_blocking(can_port_id,&RxSymbol)
       this function blocks until a new symbol is available on the bus
    */

   
   
 }
 
 
     
 