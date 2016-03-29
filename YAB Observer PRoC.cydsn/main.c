/*******************************************************************************
* File Name: main.c
*
* Version: 1.1
*
* Description:
*  This project demonstrates simultaneous usage of the BLE GAP Peripheral and 
*  Broadcaster roles. The device would connect to a peer device, while also 
*  broadcasting (non-connectable advertising) at the same time.
*
* Hardware Dependency:
*  YABP-00102-A : Cypress EZ-BLE PRoC Bluetooth Evaluation Board
*  YP-01: USB to TTL
*
********************************************************************************
* Copyright (2016), YABTRONIX TECHNOLOGIES INC.
******************************************************************************
* By : Yvan Bourassa
* Date: March 21, 2016
*******************************************************************************/

/*******************************************************************************
* Included headers
*******************************************************************************/
#include <project.h>
#include <stdio.h>
#include <common.h>

/***************************************
*        Function Prototypes
***************************************/
void StackEventHandler(uint32 event, void* eventParam);

/*******************************************************************************
* Function Name: StackEventHandler
********************************************************************************
*
* Summary:
*  This is an event callback function to receive events from the CYBLE Component.
*
* Parameters:
*  uint8 event:       Event from the CYBLE component.
*  void* eventParams: A structure instance for corresponding event type. The
*                     list of event structure is described in the component
*                     datasheet.
*
* Return:
*  None
*
*******************************************************************************/
void StackEventHandler(uint32 event, void* eventParam)
{
    /*local variables*/
   CYBLE_GAPC_ADV_REPORT_T advReport;
   uint8 i;
     
    switch(event)
    {
        
        case CYBLE_EVT_STACK_ON:    /*BLE stack ON*/
        
                //printf("Bluetooth ON:\r\n");
                /*Start Scanning*/
                if(CYBLE_ERROR_OK==CyBle_GapcStartScan(CYBLE_SCANNING_SLOW))
                {
                    //printf("Sarted to Scan\r\n"); 
                }   
                
            break;

        case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:  
                
                /* scan progress result event occurs 
                 * when it receives any advertisiment packet
                 * or scan response packet from peer device*/
                
                advReport=*(CYBLE_GAPC_ADV_REPORT_T *)eventParam;

                //printf("CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:\r\n");
                /* Print the Advertising Event details of peer device:*/
                //printf("eventType:");
                
                //************************************************
                /* REPLACE THIS BY JSON FORMAT */
                //************************************************
                
                switch(advReport.eventType)
                {
                    case CYBLE_GAPC_CONN_UNDIRECTED_ADV: 
                        
                         //printf("Connectable undirected advertising\r\n");
                        break;
                        
                    case  CYBLE_GAPC_CONN_DIRECTED_ADV:
                         
                         //printf("Connectable directed advertising\r\n");
                        break;
                    
                    case  CYBLE_GAPC_SCAN_UNDIRECTED_ADV:
                        
                         //printf("Scannable undirected advertising\r\n");
                        break;
                     
                    case CYBLE_GAPC_NON_CONN_UNDIRECTED_ADV:
                        
                          //printf("Non connectable undirected advertising\r\n");
                        break;
                     
                    case CYBLE_GAPC_SCAN_RSP:
                        
                          //printf("SCAN_RSP\r\n");
                        break;
                }
                
                /*  PEER addr type  */
                //printf("    peerAddrType: ");
                
                if(advReport.peerAddrType==CYBLE_GAP_ADDR_TYPE_PUBLIC)
                {
                    //printf("PUBLIC\r\n");
                }
                else if(advReport.peerAddrType==CYBLE_GAP_ADDR_TYPE_RANDOM)
                {
                    //printf("RANDOM \r\n");
                }
                
                 /*  PEER Device address type  */
                printf("%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x,",
                    advReport.peerBdAddr[5u], advReport.peerBdAddr[4u],
                    advReport.peerBdAddr[3u], advReport.peerBdAddr[2u],
                    advReport.peerBdAddr[1u], advReport.peerBdAddr[0u]);
  
                /*  Advertising or scan response data  */
                //printf("    Peer device adveritsing/scan response data Length: %x \r\n", advReport.dataLen);
                //printf("    advertising/scan response data of peer device: ");

                if(advReport.dataLen!=0)
                {
                    for(i = 0u; i < (advReport.dataLen); i++)
                    {
                        printf("%02x,", advReport.data[i]);
                    }
                
                }
           
                /*  RSSI of the received packet from peer Device  */
                printf("%i", (advReport.rssi));
                printf("\n");
                
                /*   Stop GAP for next scan */
                CyBle_GapcStopScan();
                
            break;  
                
         case  CYBLE_EVT_GAPC_SCAN_START_STOP:
                
                if(CyBle_GetState()==CYBLE_STATE_DISCONNECTED)
                {   /*Restart scanning if time out happens*/
                    CyBle_GapcStartScan(CYBLE_SCANNING_SLOW);
                }
            break;
                
        default:
            break;
    }
}

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/

int main()
{
    /* Variable declarations */
    //CYBLE_LP_MODE_T lpMode;
    //CYBLE_BLESS_STATE_T blessState;
    //uint8 InterruptsStatus;    
   
    /* Start communication component */
    UART_Start();
    
    /* Enable global interrupts */
    CyGlobalIntEnable;
       
    /* Internal low power oscillator is stopped as it is not used in this project */
    CySysClkIloStop();
    
    /* Set the divider for ECO, ECO will be used as source when IMO is switched off to save power,
    **  to drive the HFCLK */
    CySysClkWriteEcoDiv(CY_SYS_CLK_ECO_DIV8);
   
    CyBle_Start(StackEventHandler);
    /*Infinite Loop*/
    for(;;)
    {
        CyBle_ProcessEvents();
    }
 }
/* [] END OF FILE */

