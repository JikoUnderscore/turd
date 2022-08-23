#include "headers/fov.h"

fn fov::set_light(array<CellBox, WIDTH * HEIGHT> &map_data,
                  array<CellBox, WIDTH * HEIGHT> &screen, const u32 x,
                  const u32 y)
    ->void {
  let index = (y * WIDTH_U + x);
  switch (screen[index]) {
  case CellBox::FloorInvisable:
  case CellBox::FloorExplorerd: {
    map_data[index] = CellBox::FloorExplorerd;
    screen[index] = CellBox::Floor;
  } break;

  case CellBox::WallExplorerd:
  case CellBox::WallInvisavle: {
    map_data[index] = CellBox::WallExplorerd;
    screen[index] = CellBox::Wall;
  } break;

  case CellBox::Void:
  case CellBox::Floor:
  case CellBox::Wall:
  case CellBox::Playe:
    break;
  }
}

fn fov::cast_light(array<CellBox, WIDTH * HEIGHT> &map_data,
                   array<CellBox, WIDTH * HEIGHT> &screen, const u32 x,
                   const u32 y, const u32 radius, const u32 row,
                   f32 start_slope, const f32 end_slope, const i32 xx,
                   const i32 xy, const i32 yx, const i32 yy)
    ->void {
  if (start_slope < end_slope) {
    return;
  }

  auto next_start_slope{start_slope};
  let radius_squared{radius * radius};

  for (u32 j = row; j <= radius; ++j) {
    let dy{-static_cast<i32>(j)};
    auto blocked{false};
    for (i32 dx = -static_cast<i32>(j); dx <= 1; ++dx) {
      let l_slope{(static_cast<f32>(dx) - 0.5F) /
                  (static_cast<f32>(dy) + 0.5f)};
      let r_slope{(static_cast<f32>(dx) + 0.5F) /
                  (static_cast<f32>(dy) - 0.5f)};

      if (start_slope < r_slope) {
        continue;
      }
      if (end_slope > l_slope) {
        break;
      }

      let da_x{dx * xx + dy * xy};
      let da_y{dx * yx + dy * yy};
      if (da_x < 0 and static_cast<u32>(std::abs(da_x)) > x or
          da_y < 0 and static_cast<u32>(std::abs(da_y)) > y) {
        continue;
      }

      let map_x{x + da_x};
      let map_y{y + da_y};
      if (static_cast<u32>(dx * dx + dy * dy) < radius_squared) {
        set_light(map_data, screen, map_x, map_y);
      }

      let blkd{blocked_cell(map_data, map_x, map_y)};
      if (blocked) {
        if (blkd) {
          next_start_slope = r_slope;
          continue;
        }
        blocked = false;
        start_slope = next_start_slope;
      } else if (blkd and j < radius) {
        blocked = true;
        next_start_slope = r_slope;
        cast_light(map_data, screen, x, y, radius, j + 1, start_slope, l_slope,
                   xx, xy, yx, yy);
      }
    }
    if (blocked) {
      break;
    }
  }
}

fn fov::do_fov(array<CellBox, WIDTH * HEIGHT> &map_data,
               array<CellBox, WIDTH * HEIGHT> &screen, Vector2D const &pla_pos,
               const u32 pla_radius)
    ->void {
  static constexpr i32 mul[4][8]{{1, 0, 0, -1, -1, 0, 0, 1},
                                 {0, 1, -1, 0, 0, -1, 1, 0},
                                 {0, 1, 1, 0, 0, -1, -1, 0},
                                 {1, 0, 0, 1, -1, 0, 0, -1}};

  for (u32 oct = 0; oct < 8; ++oct) {
    cast_light(map_data, screen, pla_pos.x, pla_pos.y, pla_radius, 1, 1.0, 0.0,
               mul[0][oct], mul[1][oct], mul[2][oct], mul[3][oct]);
  }
}

fn fov::blocked_cell(array<CellBox, WIDTH * HEIGHT> const &map_data,
                     const u32 x, const u32 y)
    ->bool {
  bool wall{false};

  switch (map_data[(y * WIDTH_U + x)]) {
  case CellBox::Wall:
  case CellBox::WallInvisavle:
  case CellBox::WallExplorerd:
  case CellBox::Void: {
    wall = true;
  } break;
  case CellBox::Floor:
  case CellBox::FloorInvisable:
  case CellBox::FloorExplorerd:
  case CellBox::Playe:
    break;
  }

  return x >= WIDTH_U || y >= HEIGHT_U || wall;
}
