#include "headers/logic.h"

#include "boost/container/static_vector.hpp"
#include <memory_resource>
#include <random>
#include <algorithm>

Vector2D::Vector2D(): x(0), y(0) {}


Vector2D::Vector2D(const u32 x, const u32 y)
    : x(x), y(y) {}

Rect2D::Rect2D()
    : x(0), y(0), w(0), h(0) {}

Rect2D::Rect2D(const u32 x, const u32 y, const u32 w, const u32 h)
    : x(x), y(y), w(w), h(h) {}

fn Rect2D::has_intersection(const Rect2D& other) const -> bool {
    return self.x <= other.x + other.w and self.x + self.w >= other.x and self.y <= other.y + other.h and
        self.y + self.h >= other.y;
}

fn Rect2D::center() const -> pair<u32, u32> {
    return std::make_pair((self.x + self.x + self.w) / 2, (self.y + self.y + self.h) / 2);
}

BoardMap::BoardMap(): data() {
    // self.data.fill(Cell::Void);
}

fn BoardMap::rand_range(const u32 min_value, const u32 max_value) -> u32 {
    // printf("min_value %d max_value %d = ", min_value, max_value);
    std::random_device seed;
    std::mt19937 gen{ seed() };                                   // seed the generator
    std::uniform_int_distribution dist{ min_value, max_value }; // set min and max


    let result = dist(gen);
    // printf("result %d\n", result);

    return result;                                            // generate number
    // let range = max_value - min_value + 1;
    // return rand() % range + min_value;
}

fn BoardMap::compare_interval(Rect2D const& lhs, Rect2D const& rhs) -> bool {
    let [ax,ay]{ lhs.center() };
    let [bx,by]{ rhs.center() };
    return ax > by or ay > by;
}

// template<u32 NumberOfRooms>
fn BoardMap::create_rooms(Vector2D& pla_pos) -> void {
    constexpr u32 NumberOfRooms{24};

    boost::container::static_vector<Rect2D, NumberOfRooms> rooms{};
    {
        // if empty move out of loop
        let w_tmp = rand_range(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        let h_tmp = rand_range(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        let x_tmp = rand_range(1, WIDTH_U - w_tmp - 1);
        let y_tmp = rand_range(1, HEIGHT_U - h_tmp - 1);

        let new_room_tmp{ Rect2D(x_tmp, y_tmp, w_tmp, h_tmp) };
        self.add_rect(new_room_tmp);
        let [new_x, new_y] = new_room_tmp.center();
        pla_pos.x = new_x;
        pla_pos.y = new_y;
        self.data[pla_pos.y * WIDTH_U + pla_pos.x] = CellBox::Playe;
        rooms.emplace_back(new_room_tmp);
    }
    for (u32 i = 0; i <= NumberOfRooms; ++i) {
        let w = rand_range(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        let h = rand_range(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
        let x = rand_range(1, WIDTH_U - w - 1);
        let y = rand_range(1, HEIGHT_U - h - 1);

        let new_room{ Rect2D(x, y, w, h) };

        for (let& room : rooms) {
            if (room.has_intersection(new_room)) {
                goto continue_next;
            }
        }
        // if `new_room` rect intersects with any of the rects in `rooms` continues
        // if (std::ranges::none_of(rooms, [&new_room](Rect2D const& room) {return room.has_intersection(new_room);})) {
        //      continue;
        // }
        self.add_rect(new_room);

        rooms.emplace_back(new_room);
    continue_next: ;
    }

    std::ranges::sort(rooms, [](Rect2D const& a, Rect2D const& b) { return a.x > b.x; });
    for (usize i = 1; i < rooms.size(); ++i) {
        let [new_x, new_y] = rooms[i].center();
        let [prev_x, prev_y] = rooms[i - 1].center();

        // assert(new_x < WIDTH_U);
        // assert(prev_x < WIDTH_U);
        // assert(new_y < HEIGHT_U);
        // assert(prev_y < HEIGHT_U);
        self.create_h_tunnel(prev_x, new_x, prev_y);
        self.create_v_tunnel(prev_y, new_y, new_x);
    }
}

fn BoardMap::cramp_zero(const u32 a) -> u32 {
    // variable = (condition) ? expressionTrue : expressionFalse;
    return (static_cast<i32>(a) - 1 < 0) ? 0 : a - 1;

}

fn BoardMap::cramp_value(const u32 a, const u32 value) -> u32 {
    return (a + value > WIDTH_U) ? WIDTH_U - 1 : a + value;
}


fn BoardMap::add_rect(Rect2D const& rect) -> void {
    let x_left{ cramp_zero(rect.x) };
    let x_right{ cramp_value(rect.x, rect.w) };

    for (u32 y1 = rect.y; y1 < rect.h + rect.y; ++y1) {
        self.data[y1 * WIDTH_U + x_left] = CellBox::WallInvisavle;
        self.data[y1 * WIDTH_U + x_right] = CellBox::WallInvisavle;

        for (u32 x1 = rect.x; x1 < rect.w + rect.x; ++x1) { self.data[y1 * WIDTH_U + x1] = CellBox::FloorInvisable; }
    }

    let y_top{ cramp_zero(rect.y) };
    let y_bot{ cramp_value(rect.y, rect.h) };
    for (u32 rx = rect.x; rx < rect.w + rect.x; ++rx) {
        self.data[y_top * WIDTH_U + rx] = CellBox::WallInvisavle;
        self.data[y_bot * WIDTH_U + rx] = CellBox::WallInvisavle;
    }
}

fn BoardMap::create_h_tunnel(const u32 x1, const u32 x2, const u32 y) -> void {
    for (u32 x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
        // printf("h_tunnel x %d, y %d \n", x, y);
        self.data[y * WIDTH_U + x] = CellBox::FloorInvisable;

        if (self.data[(y + 1) * WIDTH_U + x] == CellBox::Void) {
            self.data[((y + 1) * WIDTH_U + x)] = CellBox::WallInvisavle;
        }
        if (self.data[((y - 1) * WIDTH_U + x)] == CellBox::Void) {
            self.data[((y - 1) * WIDTH_U + x)] = CellBox::WallInvisavle;
        }
    }
}

fn BoardMap::create_v_tunnel(const u32 y1, const u32 y2, const u32 x) -> void {
    for (u32 y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
        // printf("v_tunnel x %d, y %d \n", x, y);

        self.data[y * WIDTH_U + x] = CellBox::FloorInvisable;

        if (self.data[y * WIDTH_U + (x + 1)] == CellBox::Void) {
            self.data[y * WIDTH_U + (x + 1)] = CellBox::WallInvisavle;
        }
        if (self.data[y * WIDTH_U + (x - 1)] == CellBox::Void) {
            self.data[y * WIDTH_U + (x - 1)] = CellBox::WallInvisavle;
        }
    }
}
