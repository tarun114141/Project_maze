#pragma once
#include <GL/glut.h>
#include <string>
using namespace std;


class Button {
private:
    float x, y;
    float width, height;
    string label;
    bool hovered;

    // text offset from top-left of button (manual control)
    float text_offset_x;
    float text_offset_y;

    // colors
    float nr, ng, nb;
    float hr, hg, hb;
    float br, bg, bb;
    float tr, tg, tb;

    void* font;   // GLUT bitmap font

    void drawText() {
        float tx = x + text_offset_x;
        float ty = y - text_offset_y;

        glColor3f(tr, tg, tb);
        glRasterPos2f(tx, ty);
        for (char c : label)
            glutBitmapCharacter(font, c);
    }

public:
    Button(float x, float y, float width, float height, string label)
        : x(x), y(y), width(width), height(height), label(label), hovered(false)
    {
        // default: auto center approximation
        text_offset_x = width * 0.1f;
        text_offset_y = height * 0.62f;

        font = GLUT_BITMAP_HELVETICA_18;

        nr = ng = nb = 0.25f;
        hr = hg = hb = 0.35f;
        br = 0.0f; bg = 1.0f; bb = 0.0f;
        tr = tg = tb = 1.0f;
    }

    // ── text control ─────────────────────────────────────────────────────────

    // manually nudge text inside button
    // offset_x: 0 = left edge,  width = right edge
    // offset_y: 0 = top edge,   height = bottom edge
    void setTextOffset(float offset_x, float offset_y) {
        text_offset_x = offset_x;
        text_offset_y = offset_y;
    }

    // font options:
    // GLUT_BITMAP_HELVETICA_10
    // GLUT_BITMAP_HELVETICA_12
    // GLUT_BITMAP_HELVETICA_18  (default)
    // GLUT_BITMAP_TIMES_ROMAN_10
    // GLUT_BITMAP_TIMES_ROMAN_24
    // GLUT_BITMAP_8_BY_13
    void setFont(void* f) {
        font = f;
    }

    void setLabel(string s) { label = s; }

    // ── color control ─────────────────────────────────────────────────────────
    void setNormalColor(float r, float g, float b) { nr = r; ng = g; nb = b; }
    void setHoverColor (float r, float g, float b) { hr = r; hg = g; hb = b; }
    void setBorderColor(float r, float g, float b) { br = r; bg = g; bb = b; }
    void setTextColor  (float r, float g, float b) { tr = r; tg = g; tb = b; }

    // ── draw (call every frame in display()) ──────────────────────────────────
    void draw() {
        if (hovered) glColor3f(hr, hg, hb);
        else         glColor3f(nr, ng, nb);

        glBegin(GL_QUADS);
        glVertex2f(x,         y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x,         y - height);
        glEnd();

        glColor3f(br, bg, bb);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x,         y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x,         y - height);
        glEnd();

        drawText();
    }

    // ── mouse ─────────────────────────────────────────────────────────────────
    bool isClicked(float gl_x, float gl_y) {
        return gl_x > x && gl_x < x + width &&
               gl_y < y && gl_y > y - height;
    }

    void updateHover(float gl_x, float gl_y) {
        hovered = isClicked(gl_x, gl_y);
    }

    bool isHovered() { return hovered; }
};