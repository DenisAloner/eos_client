#ifndef GUI_STYLE_H
#define GUI_STYLE_H

#include "Definiton.h"

struct gui_style_t {

    atlas_tile_t* background_tile;
    atlas_tile_t* border_x_tile;
    atlas_tile_t* border_y_tile;
    atlas_tile_t* corner_tile;
	
    gui_style_t() = default;
	
};

#endif //GUI_STYLE_H
