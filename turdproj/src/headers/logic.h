#pragma once
#include <array>


#include "defines.hpp"


constexpr u32 SIZE{ 15 };
constexpr i32 SIZE_I{ SIZE };
constexpr u32 WIDTH_U{ 1280 / SIZE };
constexpr u32 HEIGHT_U{ 720 / SIZE };
constexpr usize WIDTH{ WIDTH_U };
constexpr usize HEIGHT{ HEIGHT_U };


constexpr u32 ROOM_MIN_SIZE{ 6 };
constexpr u32 ROOM_MAX_SIZE{ 10 };


using std::array, std::pair;


enum class CellBox {
    Void,
    Floor,
    FloorInvisable,
    FloorExplorerd,
    Wall,
    WallExplorerd,
    WallInvisavle,
    Playe
};


struct Vector2D {
    Vector2D();

    Vector2D(u32 x, u32 y);

    u32 x;
    u32 y;
};


struct Rect2D {
    Rect2D();

    Rect2D(u32 x, u32 y, u32 w, u32 h);

    fn has_intersection(const Rect2D& other) const -> bool;

    fn center() const -> pair<u32, u32>;

    u32 x;
    u32 y;
    u32 w;
    u32 h;
};


struct BoardMap {
    BoardMap();

    static fn rand_range(u32 min_value, u32 max_value) -> u32;

    static fn compare_interval(Rect2D const& lhs, Rect2D const& rhs) -> bool;

    // template<u32 NumberOfRooms>
    fn create_rooms(Vector2D& pla_pos) -> void;

private:
    static fn cramp_zero(u32 a) -> u32;

    static fn cramp_value(u32 a, u32 value) -> u32;

    fn add_rect(Rect2D const& rect) -> void ;

    fn create_h_tunnel(u32 x1, u32 x2, u32 y) -> void;

    fn create_v_tunnel(u32 y1, u32 y2, u32 x) -> void;
public:
    array<CellBox, WIDTH * HEIGHT> data;
};

