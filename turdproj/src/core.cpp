#include "headers/core.h"


[[nodiscard]]
fn core::Renderer::init(const char *const title) -> std::optional<bool> {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("`fn Renderer::init` error %s", SDL_GetError());
        return std::nullopt;
    }

    auto *prozorec{
        SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE)
    };
    if (prozorec == nullptr) {
        SDL_Log("`fn Renderer::init` error %s", SDL_GetError());
        return std::nullopt;
    }

    auto *ekran{ SDL_CreateRenderer(prozorec, -1, SDL_RENDERER_ACCELERATED) };
    if (ekran == nullptr) {
        SDL_Log("`fn Renderer::init` error %s", SDL_GetError());
        return std::nullopt;
    }

    if (SDL_RenderSetLogicalSize(ekran, 1280, 720) < 0) {
        SDL_Log("`fn Renderer::init` error %s", SDL_GetError());
        return std::nullopt;
    }

    let flags{ IMG_INIT_PNG };
    if ((IMG_Init(flags) & flags) != flags) {
        SDL_Log("`fn Renderer::init` error %s", SDL_GetError());
        return std::nullopt;
    }

    self.ekran_ = ekran;
    self.window_ = prozorec;
    return std::optional<bool>{true};
}

fn core::Renderer::load_texture(const char *const filename) const -> std::optional<Texture> {
    let tex{ IMG_LoadTexture(self.ekran_, filename) };
    if (tex == nullptr) {
        SDL_Log("`fn Renderer::load_texture` error %s", SDL_GetError());
        return {};
    }

    return Texture(tex);
}

fn core::Renderer::render_copy(Texture const& texture, Rect const& srcrect, Rect const& dstrect) const -> std::optional<bool> {
    if (SDL_RenderCopy(self.ekran_, texture.raw_texture, &srcrect.raw_rect, &dstrect.raw_rect) < 0) {
        SDL_Log("`fn Renderer::render_copy`  error %s", SDL_GetError());
        return {};
    }
    return std::optional{true};
}


fn core::Renderer::render_copy_unchecked(Texture const& texture, Rect const& srcrect, Rect const& dstrect) const -> void {
    SDL_RenderCopy(self.ekran_, texture.raw_texture, &srcrect.raw_rect, &dstrect.raw_rect);
}

fn core::Renderer::set_draw_color(std::tuple<uint8_t, uint8_t, uint8_t>&& color) const -> void {
    if (SDL_SetRenderDrawColor(this->ekran_, std::get<0>(color), std::get<1>(color), std::get<2>(color), 255) < 0) {
        SDL_Log("`fn Renderer::set_draw_color` error %s", SDL_GetError());
    }
}

fn core::Renderer::set_draw_color(std::tuple<uint8_t, uint8_t, uint8_t, uint8_t>&& color) const -> void {
    if (SDL_SetRenderDrawColor(this->ekran_, std::get<0>(color), std::get<1>(color), std::get<2>(color), std::get<3>(color)) < 0) {
        SDL_Log("`fn Renderer::set_draw_color` error %s", SDL_GetError());
    }
}

fn core::Renderer::clear() const -> void {
    if (SDL_RenderClear(self.ekran_) < 0) {
        SDL_Log("`fn Renderer::clear` error %s", SDL_GetError());
    }
}

fn core::Renderer::present() const -> void {
    SDL_RenderPresent(self.ekran_);
}

[[nodiscard]]
fn core::Renderer::fill_rect(Rect const& rect) const -> std::optional<bool> {
    if (SDL_RenderFillRect(self.ekran_, &rect.raw_rect) < 0) {
        SDL_Log("`fn Renderer::fill_rect` error %s", SDL_GetError());
        return {};
    }
    return std::optional{true};
}

fn core::Renderer::fill_rect_unchecked(Rect const& rect) const -> void {
    SDL_RenderFillRect(self.ekran_, &rect.raw_rect);
}

core::Renderer::Renderer(): ekran_(nullptr), window_(nullptr) {}

core::Renderer::~Renderer() {
    SDL_DestroyRenderer(self.ekran_);
    SDL_DestroyWindow(self.window_);
    IMG_Quit();
    SDL_Quit();
}


fn core::FpsCapDeltaTime::end() -> void {
    self.cap_frame_end_ = SDL_GetTicks() - self.cap_frame_start_;

    if (self.cap_frame_end_ < self.cap_frame_delay_) {
        SDL_Delay(self.cap_frame_delay_ - self.cap_frame_end_);
    }
}

core::FpsCapDeltaTime::FpsCapDeltaTime(const u32 fps)
    : dt(0.0),
      cap_frame_start_(0),
      fps_last_time_(0),
      fps_now_(SDL_GetPerformanceCounter()),
      cap_frame_end_(0),
      cap_frame_delay_(1000 / fps) {}

fn core::FpsCapDeltaTime::start() -> void {
    self.cap_frame_start_ = SDL_GetTicks();

    self.fps_last_time_ = self.fps_now_;
    self.fps_now_ = SDL_GetPerformanceCounter();

    self.dt = static_cast<f32>(self.fps_now_ - self.fps_last_time_) / static_cast<f32>(SDL_GetPerformanceFrequency());
    // std::cout << "fps: " << 1.0f / dt << " dt: " << dt << std::endl;
}

core::Rect::Rect(const i32 x, const i32 y, const i32 w, const i32 h)
    : raw_rect({ .x = x, .y = y, .w = w, .h = h }) { }

fn core::Rect::set_y(const i32 y) -> void {
    self.raw_rect.y = y;
}

core::Texture::Texture(SDL_Texture *texture):
    raw_texture(texture) {}

core::Texture::~Texture() {
    SDL_DestroyTexture(self.raw_texture);
}


core::Texture::Texture(Texture&& other) noexcept {
    self.raw_texture = other.raw_texture;

    other.raw_texture = nullptr;
}

fn core::Rect::set_x(const i32 x) -> void {
    self.raw_rect.x = x;
}
