#pragma once

#include "logic.h"
#include <cstdlib>
using std::array;

constexpr i32 FOV_REDIUS_I = 5;
constexpr u32 FOV_REDIUS = FOV_REDIUS_I;


namespace fov {

    fn set_light(array<CellBox, WIDTH * HEIGHT>& map_data, array<CellBox, WIDTH * HEIGHT>& screen,
                 u32 x, u32 y) -> void ;

    fn blocked_cell(array<CellBox, WIDTH * HEIGHT> const& map_data, u32 x, u32 y) -> bool;


    fn cast_light(array<CellBox, WIDTH * HEIGHT>& map_data, array<CellBox, WIDTH * HEIGHT>& screen,
                  u32 x, u32 y, u32 radius, u32 row, f32 start_slope, f32 end_slope,
                  i32 xx, i32 xy, i32 yx, i32 yy) -> void ;


    fn do_fov(array<CellBox, WIDTH * HEIGHT>& map_data, array<CellBox, WIDTH * HEIGHT>& screen,
              Vector2D const& pla_pos, u32 pla_radius) -> void ;


}
