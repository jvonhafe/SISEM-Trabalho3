#include "can_app.h"
#include "aht10.h"

static volatile uint8_t can_tx_request = 0;

static const uint16_t can_valid_ids[] =
{
    1200, 1201, 1202, 1203, 1204,
    1205, 1206, 1207, 1208, 1209,
    1210, 1211, 1212, 1213, 1214,
    1215, 1216, 1217, 1219, 1220
};

static uint8_t can_id_index = 10;   // começa em 1210

void APP_CAN_RequestTx(void)
{
    can_tx_request = 1;
}

uint16_t APP_CAN_GetTxID(void)
{
    return can_valid_ids[can_id_index];
}

void APP_CAN_NextTxID(void)
{
    can_id_index++;

    if(can_id_index >= (sizeof(can_valid_ids) / sizeof(can_valid_ids[0])))
    {
        can_id_index = 0;
    }
}

void APP_CAN_PrevTxID(void)
{
    if(can_id_index == 0)
    {
        can_id_index = (sizeof(can_valid_ids) / sizeof(can_valid_ids[0])) - 1;
    }
    else
    {
        can_id_index--;
    }
}

void APP_CAN_Task(void)
{
    if(can_tx_request)
    {
        can_tx_request = 0;

        APP_AHT10_Read();

        int32_t temp =
            (int32_t)
            ((aht10.temperature + 55.0f)
             * 10.0f);

        uint32_t hum =
            (uint32_t)
            (aht10.humidity * 10.0f);

        uint8_t data[8];

        data[0] = temp;
        data[1] = temp >> 8;
        data[2] = temp >> 16;
        data[3] = temp >> 24;

        data[4] = hum;
        data[5] = hum >> 8;
        data[6] = hum >> 16;
        data[7] = hum >> 24;

        APP_CAN_Transmit(
            APP_CAN_GetTxID(),
            data,
            8);
    }
}

void APP_CAN_Transmit(
        uint16_t id,
        uint8_t *data,
        uint8_t length)
{
    XMC_CAN_MO_t *MO_Ptr;
    const CAN_NODE_t *HandlePtr =
        &CAN_NODE_0;

    MO_Ptr =
      HandlePtr->lmobj_ptr[1]->mo_ptr;

    MO_Ptr->can_data_length =
        length;

    MO_Ptr->can_identifier =
        id;

    MO_Ptr->can_data[0] =
        data[0] |
        (data[1] << 8) |
        (data[2] << 16) |
        (data[3] << 24);

    MO_Ptr->can_data[1] =
        data[4] |
        (data[5] << 8) |
        (data[6] << 16) |
        (data[7] << 24);

    CAN_NODE_MO_Init(
        HandlePtr->lmobj_ptr[1]);

    CAN_NODE_MO_Transmit(
        HandlePtr->lmobj_ptr[1]);
}

volatile uint16_t can_rx_id = 0;
volatile uint8_t can_rx_dlc = 0;
volatile uint8_t can_rx_data[8] = {0};
volatile uint8_t can_rx_flag = 0;

void CAN_A_RX(void)
{
    CAN_NODE_STATUS_t status;
    CAN_NODE_STATUS_t receive_status;
    XMC_CAN_MO_t *MO_Ptr;
    const CAN_NODE_t *HandlePtr = &CAN_NODE_0;

    MO_Ptr = HandlePtr->lmobj_ptr[0]->mo_ptr;   // LMO_01 = RX

    status = CAN_NODE_MO_GetStatus(HandlePtr->lmobj_ptr[0]);

    if(status & XMC_CAN_MO_STATUS_RX_PENDING)
    {
        XMC_CAN_MO_ResetStatus(MO_Ptr,
                               XMC_CAN_MO_RESET_STATUS_RX_PENDING);

        receive_status = CAN_NODE_MO_Receive(HandlePtr->lmobj_ptr[0]);

        if(receive_status == CAN_NODE_STATUS_SUCCESS)
        {
            can_rx_id = MO_Ptr->can_identifier;
            can_rx_dlc = MO_Ptr->can_data_length;

            can_rx_data[0] = (uint8_t)(MO_Ptr->can_data[0] & 0xFF);
            can_rx_data[1] = (uint8_t)((MO_Ptr->can_data[0] >> 8) & 0xFF);
            can_rx_data[2] = (uint8_t)((MO_Ptr->can_data[0] >> 16) & 0xFF);
            can_rx_data[3] = (uint8_t)((MO_Ptr->can_data[0] >> 24) & 0xFF);

            can_rx_data[4] = (uint8_t)(MO_Ptr->can_data[1] & 0xFF);
            can_rx_data[5] = (uint8_t)((MO_Ptr->can_data[1] >> 8) & 0xFF);
            can_rx_data[6] = (uint8_t)((MO_Ptr->can_data[1] >> 16) & 0xFF);
            can_rx_data[7] = (uint8_t)((MO_Ptr->can_data[1] >> 24) & 0xFF);

            can_rx_flag = 1;
            can_rx_flag = 0;
        }
    }
}
