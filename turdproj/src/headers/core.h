#pragma once
#include <array>
#include <filesystem>
#include <iostream>
#include <optional>

#include <SDL.h>
#include <SDL_image.h>


#include "defines.hpp"
#include "sdl_errs.h"

using std::tuple, std::array;

constexpr f32 TARGET_FPS = 60;

constexpr auto WINDOW_WIDTH{ 1280 };
constexpr auto WINDOW_HEIGHT{ 720 };


namespace core {
    struct Rect {
        Rect(i32 x, i32 y, i32 w, i32 h);

        // Rect(Rect const& a) = delete;
        // Rect(Rect&& other) = delete;
        // fn operator=(const Rect& other) -> Rect& = delete;
        // fn operator=(Rect&& other) -> Rect& = delete;

        fn set_x(i32 x) -> void;
        fn set_y(i32 y) -> void;

        SDL_Rect raw_rect;
    };

    struct Texture {
        explicit Texture(SDL_Texture *texture);

        ~Texture();

        Texture( Texture const& other) = delete;
        Texture(Texture&& other) noexcept;
        fn operator=( Texture const& other) -> Texture& = delete;
        fn operator=(Texture&& other) -> Texture& = delete;

        SDL_Texture *raw_texture;
    };

    struct Renderer {
        Renderer();

        ~Renderer();

         Renderer(const Renderer& other) = delete;
         Renderer(Renderer&& other) = delete;
         fn operator=(const Renderer& other) -> Renderer& = delete;
         fn operator=(Renderer&& other) -> Renderer& = delete;


        [[nodiscard]]
        fn init(const char *title) -> std::optional<bool>;

        [[nodiscard]]
        fn load_texture(const char *filename) const -> std::optional<Texture>;

        [[nodiscard]]
        fn render_copy(Texture const& texture, Rect const& srcrect, Rect const& dstrect) const -> std::optional<bool>;
        fn render_copy_unchecked(Texture const& texture, Rect const& srcrect, Rect const& dstrect) const -> void;

        fn set_draw_color(std::tuple<uint8_t, uint8_t, uint8_t>&& color) const -> void;
        fn set_draw_color(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>&& color) const -> void;

        fn clear() const -> void;
        fn present() const -> void;

        [[nodiscard]]
        fn fill_rect(Rect const& rect) const -> std::optional<bool>;
        fn fill_rect_unchecked(Rect const& rect) const -> void;
    private:
        SDL_Renderer *ekran_;
        SDL_Window *window_;
    };


    struct FpsCapDeltaTime {
        explicit FpsCapDeltaTime(u32 fps);

        FpsCapDeltaTime() = delete;
        fn start() -> void;
        fn end() -> void;

        f32 dt;
    private:
        u32 cap_frame_start_;
        u64 fps_last_time_;
        u64 fps_now_;
        u32 cap_frame_end_;
        u32 cap_frame_delay_;
    };

}
