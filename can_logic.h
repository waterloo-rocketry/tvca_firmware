/* 
 * File:   can_logic.h
 * Author: kavin
 *
 * Created on October 17, 2024, 10:11 PM
 */

#ifndef CAN_LOGIC_H
#define	CAN_LOGIC_H

#include <canlib.h>

void can_receive_callback(const can_msg_t *msg);

void initialize_can(uint8_t *tx_pool, size_t tx_pool_size);



#endif	/* CAN_LOGIC_H */

