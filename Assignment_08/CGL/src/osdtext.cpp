#include <iostream>

#include "osdtext.h"

namespace CGL {

// ---------------------------------------------------------------------------
// Windows build: FreeType is not available, so text rendering is stubbed out.
// The 3D viewport (rope, masses, springs) still renders normally; only the
// on-screen text overlay is disabled.
// ---------------------------------------------------------------------------

OSDText::OSDText() {}

OSDText::~OSDText() {}

int OSDText::init(bool use_hdpi) { (void)use_hdpi; return 0; }

void OSDText::render() {}

void OSDText::resize(size_t w, size_t h) { (void)w; (void)h; }

int OSDText::add_line(float x, float y, std::string text, size_t size, Color color) {
  (void)x; (void)y; (void)text; (void)size; (void)color;
  return 0;
}

void OSDText::del_line(int line_id) { (void)line_id; }

void OSDText::set_anchor(int line_id, float x, float y) {
  (void)line_id; (void)x; (void)y;
}

void OSDText::set_text(int line_id, std::string text) {
  (void)line_id; (void)text;
}

void OSDText::set_size(int line_id, size_t size) { (void)line_id; (void)size; }

void OSDText::set_color(int line_id, Color color) { (void)line_id; (void)color; }

} // namespace CGL
