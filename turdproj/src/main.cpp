#include "SDL.h"
#undef main

#include "headers/core.h"
#include "headers/fov.h"
#include "headers/logic.h"


#ifdef _DEBUG
#include <source_location>

template <class T>
inline fn inner_unwrap(expected<T, SdlErrs> value, std::source_location&& location) -> T {
    if (not value.has_value()) {
        std::cout << "-->"
            << location.function_name() << ':'
            << location.column() << ':'
            << location.line() << ' '
            << location.file_name() << ':'
            << '\n';
        exit(-69);
    }

    return *value;
}


#define unwrap(x) inner_unwrap((x), std::source_location::current())
#define check(x) do { if (not (x).has_value())  {printf("line %d, file %s\n", __LINE__, __FILE__); return static_cast<int>((x).error());} } while (0)

#else
    #define unwrap(x) *(x)
    #define check(x) *(x)
#endif

// template fn BoardMap::create_rooms<24ULL>(Vector2D& pla_pos) -> void;


// https://www.youtube.com/watch?v=q6A7cKFXjY0
int main() {
    auto core{ core::Renderer() };
    check(core.init("Rouge cpp"));

    let sprite_sheet{ core.load_texture("./assets/ss.png").value() };


    auto fps_ctrl{ core::FpsCapDeltaTime(60) };


    std::cout << "__cplusplus " << __cplusplus << '\n';


    auto map{ BoardMap() };
    array<CellBox, 4080> screen{};

    auto pla_pos{ Vector2D() };
    u32 pla_fov{ 5 };


    map.create_rooms(pla_pos);


    bool update_tunr{ false };
    bool show_map{ false };

    bool is_running{ true };
//--------- LOOP
    while (is_running) {
        fps_ctrl.start();


//--------- EVENT
        for (SDL_Event event{}; SDL_WaitEvent(&event) == 1;) {
            switch (event.type) {
                case SDL_QUIT: {
                    is_running = false;
                } break;

                case SDL_KEYDOWN: {
                    screen = map.data; // copy
                    update_tunr = true;
                    // SDL_GetMouseState();

                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_F1: {
                            show_map = not show_map;
                        } break;

                        case SDL_SCANCODE_S: {
                            let next_cell = screen[((pla_pos.y + 1) * WIDTH_U + pla_pos.x)];
                            if (pla_pos.y + 1 < HEIGHT_U and (next_cell != CellBox::Void and next_cell != CellBox::Wall and next_cell != CellBox::WallExplorerd)) {
                                pla_pos.y += 1;
                            }
                        } break;

                        case SDL_SCANCODE_W: {
                            let next_cell = screen[((pla_pos.y - 1) * WIDTH_U + pla_pos.x)];
                            if (pla_pos.y != 0 and (next_cell != CellBox::Void and next_cell != CellBox::Wall and next_cell != CellBox::WallExplorerd)) {
                                pla_pos.y -= 1;
                            }
                        } break;

                        case SDL_SCANCODE_A: {
                            let next_cell = screen[(pla_pos.y * WIDTH_U + (pla_pos.x - 1))];
                            if (pla_pos.x != 0 and (next_cell != CellBox::Void and next_cell != CellBox::Wall and next_cell != CellBox::WallExplorerd)) {
                                pla_pos.x -= 1;
                            }
                        } break;

                        case SDL_SCANCODE_D: {
                            let next_cell = screen[(pla_pos.y * WIDTH_U + (pla_pos.x + 1))];
                            if (pla_pos.x + 1 < WIDTH_U and (next_cell != CellBox::Void and next_cell != CellBox::Wall and next_cell != CellBox::WallExplorerd)) {
                                pla_pos.x += 1;
                            }
                        } break;

                        default: break;
                    }
                    screen[(pla_pos.y * WIDTH_U + pla_pos.x)] = CellBox::Playe;
                } break;
                default: break;
            } break;
        }
//--------- UPDATE
        if (update_tunr) {
            printf("updarte\n");
            // update_tunr = false;

            fov::do_fov(map.data, screen, pla_pos, pla_fov);
        }
//--------- RENDER
        if (update_tunr) {
            update_tunr = false;

            // core.set_draw_color({ 0, 0, 0 });
            // core.clear();

            auto dstrect{ core::Rect(0, 0, SIZE_I, SIZE_I) };


            for (usize y = 0; y < HEIGHT; ++y) {
                for (usize x = 0; x < WIDTH; ++x) {
                    dstrect.set_x(static_cast<i32>(x) * SIZE_I);
                    dstrect.set_y(static_cast<i32>(y) * SIZE_I);

                    switch (screen[y * WIDTH + x]) {
                        case CellBox::Void: {
                            core.set_draw_color({ 0, 0, 0 });
                            check(core.fill_rect( dstrect));
                        } break;
                        case CellBox::Floor: {
                            check(core.render_copy(sprite_sheet, core::Rect(16, 16, 16, 16), dstrect));
                        } break;
                        case CellBox::WallInvisavle:
                        case CellBox::FloorInvisable: {
                            if (show_map) {
                                core.set_draw_color({ 100, 100, 0 });
                            } else {
                                core.set_draw_color({ 0, 0, 0 });
                            }
                            check(core.fill_rect( dstrect));
                        } break;
                        case CellBox::FloorExplorerd: {
                            check(core.render_copy(sprite_sheet, core::Rect(32, 16, 16, 16), dstrect));
                        } break;
                        case CellBox::Wall: {
                            core.set_draw_color({ 80, 10, 0 });
                            check(core.render_copy(sprite_sheet, core::Rect(0, 16, 16, 16), dstrect));
                        } break;
                        case CellBox::Playe: {
                            check(core.render_copy(sprite_sheet, core::Rect(16, 16, 16, 16), dstrect));
                            check(core.render_copy(sprite_sheet, core::Rect(0, 0, 16, 16), dstrect));
                        } break;
                        case CellBox::WallExplorerd: {
                            check(core.render_copy(sprite_sheet, core::Rect(16, 0, 16, 16), dstrect));
                        } break;
                    }
                }
            }
            core.present();
        }
        fps_ctrl.end();
    }
}
