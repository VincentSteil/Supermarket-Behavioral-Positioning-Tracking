#ifndef SQLDEFINES_H
#define SQLDEFINES_H

#define GET_CUSTOMERS_LIST      "SELECT Customer_ID, Cart_ID FROM Active_Customers;"
#define CUSTOMERS_LIST_CSTMR_ID 0
#define CUSTOMERS_LIST_CART_ID  1



#define INSERT_CART_POS         "INSERT INTO Cart_Positioning(Customer_ID, Time, X, Y) VALUES (?, ?, ?, ?);"
#define CART_POS_BIND_CSTMR_ID  0
#define CART_POS_BIND_TIME      1
#define CART_POS_BIND_X         2
#define CART_POS_BIND_Y         3

#define XY_FROM_3_TILES    "SELECT tbla.Tile_X, tbla.Tile_Y "\
                           "FROM (select b.Tile_X, b.Tile_Y from Stationary_Nodes a, Shopping_Tiles b where a.Adjacent_Tile_ID = b.Tile_ID and a.Node_ID = ?) AS tbla, "\
                           "(select b.Tile_X, b.Tile_Y from Stationary_Nodes a, Shopping_Tiles b where a.Adjacent_Tile_ID = b.Tile_ID and a.Node_ID = ?) AS tblb, "\
                           "(select b.Tile_X, b.Tile_Y from Stationary_Nodes a, Shopping_Tiles b where a.Adjacent_Tile_ID = b.Tile_ID and a.Node_ID = ?) AS tblc "\
                           "WHERE (tbla.Tile_X=tblb.Tile_X and tbla.Tile_X=tblc.Tile_X and tbla.Tile_Y=tblb.Tile_Y and tbla.Tile_Y=tblc.Tile_Y);"
#define XY_FROM_TILES_POS_1TILE_ID  0
#define XY_FROM_TILES_POS_2TILE_ID  1
#define XY_FROM_TILES_POS_3TILE_ID  2
#define XY_FROM_TILES_X_COL 0
#define XY_FROM_TILES_Y_COL 1


#endif // SQLDEFINES_H
