#ifndef SERIALDEFINES_H
#define SERIALDEFINES_H

//! Data transmitted from master node to server
// Info \ Byte No.     |   0  |     1     |     2     |   3,4,5,6
// --------------------*------*-----------*-----------*---------------
// Cart position       | 0xFF | <cart id> | <node id> | time (4 bytes)
// --------------------*------*-----------*-----------*---------------
// Frame finished      | 0xAF |           |           |
// --------------------*------*-----------*-----------*---------------

#define TYPE_CART_POSITION  0xFF
#define TYPE_END_OF_FRAME   0xAF
#define TYPE_BYTE_NO        0
#define CART_ID_BYTE_NO     1
#define NODE_ID_BYTE_NO     2

#define CART_POSITION_LEN   7 // bytes

//! Data transmitted from server to master node
// Info \ Byte No.     |   0  |  1
// --------------------*------*----------
// Start tracking cart | 0xBF | <cart id>
// --------------------*------*----------
// Stop tracking cart  | 0xCF | <cart id>
// --------------------*------*----------

#define TYPE_START_CART_TRACKING    0xBF
#define TYPE_STOP_CART_TRACKING     0xCF
#define TYPE_BYTE_NO                0
#define CART_ID_BYTE_NO             1

#endif // SERIALDEFINES_H
