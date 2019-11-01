#include <M5StickC.h>

struct Vector2 {
public:
    float x, y;
    Vector2 () { this->x = 0; this->y = 0; }
    Vector2 (float t) { this->x = t; this->y = t; }
    Vector2 (float x, float y) { this->x = x; this->y = y; }
    //Vector2 (Vector3 v) { this->x = v.x; this->y = v.y; }
    Vector2 operator + (const Vector2& t) const { return Vector2(this->x + t.x, this->y + t.y); }
    Vector2 operator - (const Vector2& t) const { return Vector2(this->x - t.x, this->y - t.y); }
    Vector2 operator * (const Vector2& t) const { return Vector2(this->x * t.x, this->y * t.y); }
    Vector2 operator / (const Vector2& t) const { return Vector2(this->x / t.x, this->y / t.y); }
    Vector2 operator + (const float& t) const { return Vector2(this->x + t, this->y + t); }
    Vector2 operator - (const float& t) const { return Vector2(this->x - t, this->y - t); }
    Vector2 operator * (const float& t) const { return Vector2(this->x * t, this->y * t); }
    Vector2 operator / (const float& t) const { return Vector2(this->x / t, this->y / t); }
    bool operator == (const Vector2& t) const { return this->x == t.x && this->y == t.y; }
};
struct Vector3 {
public:
    float x, y, z;
    Vector3 () { this->x = 0; this->y = 0; this->z = 0; }
    Vector3 (float t) { this->x = t; this->y = t; this->z = t; }
    Vector3 (Vector2 t, float z) { this->x = t.x; this->y = t.y; this->z = z; }
    Vector3 (float x, Vector2 t) { this->x = x; this->y = t.x; this->z = t.y; }
    Vector3 (float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
    static Vector3 normalize (Vector3 v) {
        float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (d == 0) { return Vector3(0.0f, 0.0f, 0.0f); }
        return v / d;
    }
    static Vector3 cross (const Vector3 a, const Vector3 b) { return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
    static float dot (Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    Vector3 operator + (const Vector3& t) const { return Vector3(this->x + t.x, this->y + t.y, this->z + t.z); }
    Vector3 operator - (const Vector3& t) const { return Vector3(this->x - t.x, this->y - t.y, this->z - t.z); }
    Vector3 operator * (const Vector3& t) const { return Vector3(this->x * t.x, this->y * t.y, this->z * t.z); }
    Vector3 operator / (const Vector3& t) const { return Vector3(this->x / t.x, this->y / t.y, this->z / t.z); }
    Vector3 operator + (const float& t) const { return Vector3(this->x + t, this->y + t, this->z + t); }
    Vector3 operator - (const float& t) const { return Vector3(this->x - t, this->y - t, this->z - t); }
    Vector3 operator * (const float& t) const { return Vector3(this->x * t, this->y * t, this->z * t); }
    Vector3 operator / (const float& t) const { return Vector3(this->x / t, this->y / t, this->z / t); }
    bool operator == (const Vector3& t) const { return this->x == t.x && this->y == t.y && this->z == t.z; }
};
struct Matrix4x4 {
public:
    float m[4][4] = {
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 0.0f}
    };
    Matrix4x4 () {}
    Matrix4x4 (float a[]) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                this->m[i][j] = a[i*4+j];
            }
        }
    }
    static float radian (float d) { return d * 0.01745329251f; }
    static Matrix4x4 identity () {
        float a[] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        return Matrix4x4(a);
    }
    
    static Matrix4x4 mul (Matrix4x4 a, Matrix4x4 b) {
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 4; ++k) {
                    r.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return r;
    }
    
    static Vector3 mul (Vector3 v, Matrix4x4 m) {
        float d = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + 1.0f * m.m[3][3];
        Vector3 r = {
            v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + 1.0f * m.m[3][0],
            v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + 1.0f * m.m[3][1],
            v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + 1.0f * m.m[3][2],
        };
        
        if (d > 0) {
            r.x /= d;
            r.y /= d;
            r.z /= d;
        }
        return r;
    }
    
    static Matrix4x4 projectionMatrix (float w, float h) {
        Matrix4x4 r;
        float aspect =  w / h;
        float fov = Matrix4x4::radian(45.0f);
        float near = 4.0f;
        float far = 40.0f;
        float y = 1.0f / tan(fov * 0.5f);
        float x = y / aspect;
        float z = far / (near - far);
        r.m[0][0] = x;
        r.m[1][1] = y;
        r.m[2][2] = z; r.m[2][3] = -1.0f;
        r.m[3][2] = z * near;
        return r;
    }
    
    static Matrix4x4 screenMatrix (float sx, float sy, float w, float h) {
        Matrix4x4 r;
        float hcx = w / 2;
        float hcy = h / 2;
        r.m[0][0] = hcx;
        r.m[1][1] = -hcy;
        r.m[2][2] = 1.0f;
        r.m[3][0] = hcx; r.m[3][1] = hcy; r.m[3][3] = 1.0f;
        return r;
    }
    
    static Matrix4x4 projscreenMatrix (float sx, float sy, float w, float h) {
        Matrix4x4 r;
        r.m[0][0] = 1.0f; r.m[1][1] = 1.0f; r.m[2][2] = 1.0f; r.m[3][3] = 1.0f;
        r = Matrix4x4::mul(r, Matrix4x4::projectionMatrix(w, h));
        r = Matrix4x4::mul(r, Matrix4x4::screenMatrix(sx, sy, w, h));
        return r;
    }
    
    static Matrix4x4 movMatrix (Vector3 v) {
        Matrix4x4 r = Matrix4x4::identity();
        r.m[3][0] = v.x;
        r.m[3][1] = v.y;
        r.m[3][2] = v.z;
        return r;
    }
    static Matrix4x4 rotMatrix (Vector3 v) {
        Matrix4x4 r = Matrix4x4::identity();
        Matrix4x4 t;
        float x = Matrix4x4::radian(v.x);
        float y = Matrix4x4::radian(v.y);
        float z = Matrix4x4::radian(v.z);
        
        t = Matrix4x4::identity();
        t.m[1][1] =  cos(x);
        t.m[1][2] =  sin(x);
        t.m[2][1] = -sin(x);
        t.m[2][2] =  cos(x);
        r = Matrix4x4::mul(r, t);
        
        t = Matrix4x4::identity();
        t.m[0][0] =  cos(y);
        t.m[0][2] = -sin(y);
        t.m[2][0] =  sin(y);
        t.m[2][2] =  cos(y);
        r = Matrix4x4::mul(r, t);
        
        t = Matrix4x4::identity();
        t.m[0][0] =  cos(z);
        t.m[0][1] =  sin(z);
        t.m[1][0] = -sin(z);
        t.m[1][1] =  cos(z);
        r = Matrix4x4::mul(r, t);
        
        return r;
    }
};

struct Face {
    uint16_t x, y, z;
    Face () {};
    Face (uint16_t x, uint16_t y, uint16_t z) { this->x = x; this->y = y; this->z = z; }
};

struct Mesh{
public:
    uint16_t v_size = 0;
    uint16_t f_size = 0;
    Vector3 vertexs[2048];
    Face faces[2048];
    Vector3 tvertexs[2048];
    uint16_t colors[2048];
    Mesh () {};
};

struct Object3D {
public:
    Vector3 position;
    Vector3 rotation;
    Mesh* mesh;
    Object3D () {};
};

struct Camera3D {
public:
    Vector3 position;
    Vector3 rotation;
    Camera3D () {};
};

class WireFrame {
public:
    uint16_t* buff;
    uint16_t* zbuff;
    int8_t sx = 0;
    int8_t sy = 0;
    int8_t ex = 0;
    int8_t ey = 0;
    uint16_t width, height;
    uint16_t buffsize = 0;
    Matrix4x4 projscreen;
    Matrix4x4 view;
    Object3D obj;
    Camera3D camera;
    uint16_t bgcolor = 0;
    uint16_t linecolor = 0;
    
    int rtype = 0;
    
    
    WireFrame () {}
    void init (int8_t sx, int8_t sy, uint8_t width, uint8_t height, uint16_t bgcolor, uint16_t linecolor, Mesh& mesh, int rtype) {
        this->sx = sx;
        this->sy = sy;
        this->width = width;
        this->height = height;
        this->buffsize = width * height;
        this->ey = this->height + this->sy;
        this->ex = this->width + this->sx;
        this->bgcolor = bgcolor;
        this->linecolor = linecolor;
        this->obj.mesh = &mesh;
        this->projscreen = Matrix4x4::projscreenMatrix(sx, sy, this->width, this->height);
        
        this->buff = new uint16_t[this->buffsize];
        this->zbuff = new uint16_t[this->buffsize];
        
        this->rtype = rtype;
    }
    
    // ~WireFrame () {
    //     delete[] this->buff;
    //     delete[] this->zbuff;
    // }
    
    void draw () {
        this->obj.position.z = -10;
        this->obj.position.y = -1.5;
        this->obj.rotation.y = this->obj.rotation.y + 6;
        
        this->view = Matrix4x4::identity();
        
        this->view = Matrix4x4::mul(this->view, Matrix4x4::rotMatrix(this->obj.rotation));
        this->view = Matrix4x4::mul(this->view, Matrix4x4::movMatrix(this->obj.position));
        
        // this->camera.position.y = 2;
        // this->camera.rotation.x = this->camera.rotation.x - 0.1;
        this->view = Matrix4x4::mul(this->view, Matrix4x4::rotMatrix(Vector3() - this->camera.rotation));
        this->view = Matrix4x4::mul(this->view, Matrix4x4::movMatrix(Vector3() - this->camera.position));
        
        for (int i = 0; i < this->obj.mesh->v_size; ++i) {
            Vector3 t = this->obj.mesh->vertexs[i];
            t = Matrix4x4::mul(t, this->view);
            this->obj.mesh->tvertexs[i] = t;
        }
        
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            Vector3 n = Vector3::normalize(Vector3::cross(v1 - v0, v2 - v0));
            const float d = min(max(Vector3::dot(Vector3(1.0f, 0.5f, 0.5f), n), 0.1f), 1.0f) * 255;
            n = (n * 0.5f + 0.5f) * 255.0f;
            
            if (this->rtype == 0) {
                this->obj.mesh->colors[i] = M5.Lcd.color565(d, d, d);
            } else {
                this->obj.mesh->colors[i] = M5.Lcd.color565(n.x, n.y, n.z);
            }
            
            
            
        }
        
        for (int i = 0; i < this->obj.mesh->v_size; ++i) {
            Vector3 t = this->obj.mesh->tvertexs[i];
            t = Matrix4x4::mul(t, this->projscreen);
            this->obj.mesh->tvertexs[i] = t;
        }
        
        this->clear();
        this->drawWire();
        
        // M5.Lcd.setAddrWindow(this->sx, this->sy, this->width, this->height);
        // M5.Lcd.pushColors(this->buff, this->buffsize);
    }
    
    void clear () {
        //M5.Lcd.fillRect(0, 0, this->width, this->height, 0);
        
        // M5.Lcd.setAddrWindow(this->sx, this->sy, this->width, this->height);
        // M5.Lcd.pushColor(this->bgcolor, this->width*this->height);
        
        for (int y = 0; y < this->height; ++y) {
            for (int x = 0; x < this->width; ++x) {
                this->buff[y*this->width+x] = this->bgcolor;
                this->zbuff[y*this->width+x] = 0;
            }
        }
        
    }
    
    void drawWire () {
        // for (int i = 0; i < this->obj.mesh->f_size; ++i) {
        //     const Face& f = this->obj.mesh->faces[i];
        //     const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
        //     const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
        //     const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            
        //     if (!(Vector3::cross(v1 - v0, v2 - v1).z > 0)) { continue; }
            
        //     M5.Lcd.drawLine(v0.x, v0.y, v1.x, v1.y, M5.Lcd.color565(128, 128, 128));
        //     M5.Lcd.drawLine(v1.x, v1.y, v2.x, v2.y, M5.Lcd.color565(128, 128, 128));
        //     M5.Lcd.drawLine(v2.x, v2.y, v0.x, v0.y, M5.Lcd.color565(128, 128, 128));
        // }
        
        
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            
            if (Vector3::cross(v1 - v0, v2 - v1).z > 0) { continue; }
            
            // M5.Lcd.drawLine(v0.x, v0.y, v1.x, v1.y, this->linecolor);
            // M5.Lcd.drawLine(v1.x, v1.y, v2.x, v2.y, this->linecolor);
            // M5.Lcd.drawLine(v2.x, v2.y, v0.x, v0.y, this->linecolor);
            
            if (!((v0.z > 0 && v0.z < 1) || (v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1))) { continue; }
            if (!((v0.x >= 0 && v0.x < this->width) || (v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width))) { continue; }
            if (!((v0.y >= 0 && v0.y < this->height) || (v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height))) { continue; }
            
            // this->drawBuffLine(v0.x, v0.y, v1.x, v1.y, this->linecolor);
            // this->drawBuffLine(v1.x, v1.y, v2.x, v2.y, this->linecolor);
            // this->drawBuffLine(v2.x, v2.y, v0.x, v0.y, this->linecolor);
            
            uint16_t z = (1.0f-(v0.z+v1.z+v2.z)*0.333f) * 32767;
            this->fillTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, this->obj.mesh->colors[i], z);
        }
        
        // for (int i = 0; i < this->obj.mesh->v_size; ++i) {
        //     const Vector3& p = this->obj.mesh->tvertexs[i];
        //     M5.Lcd.drawLine(p.x-2, this->sy+p.y, p.x+2, this->sy+p.y, M5.Lcd.color565(255, 127, 0));
        //     M5.Lcd.drawLine(p.x, this->sy+p.y-2, p.x, this->sy+p.y+2, M5.Lcd.color565(255, 127, 0));
            
        // }
        
    }
    
    inline bool inSide (const int& x, const int& y) { return !(x < 0 || x >= this->width || y < 0 || y >= this->height); }
    inline void swap (int16_t& a, int16_t& b) { int16_t c = a; a = b; b = c; }
    
    
/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).
Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!
Copyright (c) 2013 Adafruit Industries.  All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */
    // Adafruit_GFX::drawLine
    inline void drawBuffLine (int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
        if (abs(y1 - y0) > abs(x1 - x0)) {
            this->swap(x0, y0); this->swap(x1, y1);
            if (x0 > x1) { this->swap(x0, x1); this->swap(y0, y1); }
            int16_t dx = x1 - x0;
            int16_t dy = abs(y1 - y0);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y0 < y1 ? 1 : -1;
            for (; x0<=x1; ++x0) {
                if (this->inSide(y0, x0)) { this->buff[y0+x0*this->width] = color; }
                err -= dy;
                if (err < 0) {
                    y0 += ystep;
                    err += dx;
                }
            }
        } else {
            if (x0 > x1) { this->swap(x0, x1); this->swap(y0, y1); }
            int16_t dx = x1 - x0;
            int16_t dy = abs(y1 - y0);
            // int16_t err = dx / 2;
            int16_t err = dx >> 1;
            int16_t ystep = y0 < y1 ? 1 : -1;
            for (; x0<=x1; ++x0) {
                if (this->inSide(x0, y0)) { this->buff[x0+y0*this->width] = color; }
                err -= dy;
                if (err < 0) {
                    y0 += ystep;
                    err += dx;
                }
            }
        }
    }
    
    // GFXcanvas8::writeFastHLine
    inline void drawBuffLineH (int16_t x, int16_t y, int16_t w, uint16_t color, uint16_t z) {
        if (x >= this->width || y < 0 || y >= this->height) { return; }
        int16_t x2 = x + w - 1;
        if (x2 < 0) { return; }
        if (x < 0) { x = 0; }
        if(x2 >= this->width) { x2 = this->width - 1; }
        y = y * this->width;
        for (int i = x; i < x2; ++i) {
            if (z > this->zbuff[i+y]) {
                this->zbuff[i+y] = z;
                this->buff[i+y] = color;
            }
        }
    }
    
    // Adafruit_GFX::fillTriangle
    inline void fillTriangle (int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, uint16_t z) {
        int16_t a, b, y;
        if (y0 > y1) { this->swap(y0, y1); this->swap(x0, x1); }
        if (y1 > y2) { this->swap(y2, y1); this->swap(x2, x1); }
        if (y0 > y1) { this->swap(y0, y1); this->swap(x0, x1); }
        if (y0 == y2) {
            a = b = x0;
            if (x1 < a) {
                a = x1;
            } else if(x1 > b) {
                b = x1;
            }
            if (x2 < a) {
                a = x2;
            } else if (x2 > b) {
                b = x2;
            }
            this->drawBuffLineH(a, y0, b-a+1, color, z);
            //this->drawBuffLine(a, y0, b+1, y0, color);
            return;
        }
        
        int16_t dx01 = x1 - x0;
        int16_t dy01 = y1 - y0;
        int16_t dx02 = x2 - x0;
        int16_t dy02 = y2 - y0;
        int16_t dx12 = x2 - x1;
        int16_t dy12 = y2 - y1;
        int32_t sa = 0;
        int32_t sb = 0;
        int16_t last = y1 == y2 ? y1 : y1-1;
        
        for(y=y0; y<=last; ++y) {
            a = x0 + sa / dy01;
            b = x0 + sb / dy02;
            sa += dx01;
            sb += dx02;
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineH(a, y, b-a+1, color, z);
            //this->drawBuffLine(a, y, b+1, y, color);
        }
        
        sa = (int32_t)dx12 * (y - y1);
        sb = (int32_t)dx02 * (y - y0);
        for(; y<=y2; ++y) {
            a = x1 + sa / dy12;
            b = x0 + sb / dy02;
            sa += dx12;
            sb += dx02;
            
            if(a > b) { this->swap(a, b); }
            this->drawBuffLineH(a, y, b-a+1, color, z);
            //this->drawBuffLine(a, y, b+1, y, color);
        }
    }
};

class WindowManager {
public:
    uint16_t buff[12800];
    WireFrame* wireframes[256];
    uint8_t wsize = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t buffsize = 0;
    WindowManager (uint16_t width, uint16_t height) {
        this->width = width;
        this->height = height;
        this->buffsize = height * width;
    }
    
    void add (WireFrame& wf) {
        this->wireframes[this->wsize] = &wf;
        this->wsize += 1;
    }
    
    void draw () {
        for (int i = 0; i < this->buffsize; ++i) { this->buff[i] = 0; }
        
        for (int i = 0; i < this->wsize; ++i) {
            WireFrame& w = *this->wireframes[i];
            w.draw();
            for (int y = 0; y < w.height; ++y) {
                const int16_t wy = w.sy+y;
                if (wy < 0 || wy >= this->height) { continue; }
                for (int x = 0; x < w.width; ++x) {
                    const int16_t wx = w.sx+x;
                    if (wx < 0 || wx >= this->width) { continue; }
                    this->buff[wx + wy * this->width] = w.buff[x + y*w.width];
                }
            }
        }
        
        
        
        M5.Lcd.setAddrWindow(0, 0, this->width, this->height);
        M5.Lcd.pushColors(this->buff, this->buffsize);
    }
    

};

WindowManager windowmanager(160, 80);

Mesh mesh;
WireFrame wf0;
WireFrame wf1;
WireFrame wf2;
WireFrame wf3;
WireFrame wf4;
WireFrame wf21;
WireFrame wf22;
WireFrame wf23;
WireFrame wf24;


void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    
    
    // mesh.vertexs[0] = Vector3( 1.0f, -1.0f, -1.0f);
    // mesh.vertexs[1] = Vector3( 1.0f, -1.0f,  1.0f);
    // mesh.vertexs[2] = Vector3(-1.0f, -1.0f,  1.0f);
    // mesh.vertexs[3] = Vector3(-1.0f, -1.0f, -1.0f);
    // mesh.vertexs[4] = Vector3( 1.0f,  1.0f, -1.0f);
    // mesh.vertexs[5] = Vector3( 1.0f,  1.0f,  1.0f);
    // mesh.vertexs[6] = Vector3(-1.0f,  1.0f,  1.0f);
    // mesh.vertexs[7] = Vector3(-1.0f,  1.0f, -1.0f);
    // mesh.v_size = 8;
    
    // mesh.faces[0]  = Face(1, 3, 0);
    // mesh.faces[1]  = Face(7, 5, 4);
    // mesh.faces[2]  = Face(4, 1, 0);
    // mesh.faces[3]  = Face(5, 2, 1);
    // mesh.faces[4]  = Face(2, 7, 3);
    // mesh.faces[5]  = Face(0, 7, 4);
    // mesh.faces[6]  = Face(1, 2, 3);
    // mesh.faces[7]  = Face(7, 6, 5);
    // mesh.faces[8]  = Face(4, 5, 1);
    // mesh.faces[9]  = Face(5, 6, 2);
    // mesh.faces[10] = Face(2, 6, 7);
    // mesh.faces[11] = Face(0, 3, 7);
    // mesh.f_size = 12;
    
    mesh.vertexs[0] = Vector3(0.712124, 0.343239, -1.955276);
    mesh.vertexs[1] = Vector3(-0.712124, 0.343239, -1.955276);
    mesh.vertexs[2] = Vector3(0.995829, 0.263676, -1.207646);
    mesh.vertexs[3] = Vector3(-0.995829, 0.263676, -1.207646);
    mesh.vertexs[4] = Vector3(1.212771, 1.505506, -2.251083);
    mesh.vertexs[5] = Vector3(-1.212771, 1.505506, -2.251083);
    mesh.vertexs[6] = Vector3(1.334074, 2.829149, -1.216997);
    mesh.vertexs[7] = Vector3(-1.334073, 2.829149, -1.216997);
    mesh.vertexs[8] = Vector3(0.0, 0.186174, -1.225716);
    mesh.vertexs[9] = Vector3(0.0, 3.727206, -1.271224);
    mesh.vertexs[10] = Vector3(1.418549, 1.827501, -1.214256);
    mesh.vertexs[11] = Vector3(-1.418549, 1.827501, -1.214256);
    mesh.vertexs[12] = Vector3(0.190204, 0.01997, -2.893315);
    mesh.vertexs[13] = Vector3(-0.190204, 0.01997, -2.893315);
    mesh.vertexs[14] = Vector3(0.0, 0.489414, -2.801201);
    mesh.vertexs[15] = Vector3(0.164448, 0.405893, -3.320767);
    mesh.vertexs[16] = Vector3(-0.164448, 0.405893, -3.320767);
    mesh.vertexs[17] = Vector3(0.0, 0.625957, -3.118722);
    mesh.vertexs[18] = Vector3(1.068152, 0.238261, -2.143327);
    mesh.vertexs[19] = Vector3(-1.068152, 0.238261, -2.143327);
    mesh.vertexs[20] = Vector3(1.097245, 0.045193, -2.84028);
    mesh.vertexs[21] = Vector3(-1.097245, 0.045193, -2.84028);
    mesh.vertexs[22] = Vector3(0.251435, -0.28231, -2.854148);
    mesh.vertexs[23] = Vector3(-0.251435, -0.28231, -2.854148);
    mesh.vertexs[24] = Vector3(1.000889, -0.156508, -2.79419);
    mesh.vertexs[25] = Vector3(-1.000888, -0.156508, -2.79419);
    mesh.vertexs[26] = Vector3(0.916633, -0.102694, -2.301797);
    mesh.vertexs[27] = Vector3(-0.916632, -0.102694, -2.301797);
    mesh.vertexs[28] = Vector3(0.660387, -0.13562, -2.176559);
    mesh.vertexs[29] = Vector3(-0.660387, -0.13562, -2.176559);
    mesh.vertexs[30] = Vector3(0.204997, -2.00994, -2.812823);
    mesh.vertexs[31] = Vector3(-0.204997, -2.00994, -2.812823);
    mesh.vertexs[32] = Vector3(0.910759, -2.00994, -2.923685);
    mesh.vertexs[33] = Vector3(-0.910759, -2.00994, -2.923685);
    mesh.vertexs[34] = Vector3(0.902642, -2.00994, -2.32242);
    mesh.vertexs[35] = Vector3(-0.902642, -2.00994, -2.32242);
    mesh.vertexs[36] = Vector3(0.545798, -2.00994, -2.121508);
    mesh.vertexs[37] = Vector3(-0.545798, -2.00994, -2.121508);
    mesh.vertexs[38] = Vector3(0.0, 1.174393, 1.575172);
    mesh.vertexs[39] = Vector3(0.0, 0.799726, 0.77398);
    mesh.vertexs[40] = Vector3(1.069965, 0.90622, 0.780193);
    mesh.vertexs[41] = Vector3(-1.069964, 0.90622, 0.780193);
    mesh.vertexs[42] = Vector3(0.270909, 0.008363, 0.259453);
    mesh.vertexs[43] = Vector3(-0.270909, 0.008363, 0.259453);
    mesh.vertexs[44] = Vector3(0.844588, 0.191564, 0.712435);
    mesh.vertexs[45] = Vector3(-0.844587, 0.191564, 0.712435);
    mesh.vertexs[46] = Vector3(0.184621, 0.122916, 0.567233);
    mesh.vertexs[47] = Vector3(-0.18462, 0.122916, 0.567233);
    mesh.vertexs[48] = Vector3(0.161068, -2.00994, 0.330695);
    mesh.vertexs[49] = Vector3(-0.161068, -2.00994, 0.330695);
    mesh.vertexs[50] = Vector3(0.892018, -2.00994, 0.869805);
    mesh.vertexs[51] = Vector3(-0.892018, -2.00994, 0.869805);
    mesh.vertexs[52] = Vector3(0.145175, -2.00994, 0.89084);
    mesh.vertexs[53] = Vector3(-0.145175, -2.00994, 0.89084);
    mesh.vertexs[54] = Vector3(0.0, 2.932565, 2.840091);
    mesh.vertexs[55] = Vector3(0.0, 3.427747, 1.835111);
    mesh.vertexs[56] = Vector3(0.0, 1.849925, 3.10356);
    mesh.vertexs[57] = Vector3(0.877904, 0.297041, -1.652074);
    mesh.vertexs[58] = Vector3(-0.877904, 0.297041, -1.652074);
    mesh.vertexs[59] = Vector3(0.989254, 0.329146, -0.681729);
    mesh.vertexs[60] = Vector3(-0.989254, 0.329146, -0.681729);
    mesh.vertexs[61] = Vector3(0.995563, 0.734413, -1.929289);
    mesh.vertexs[62] = Vector3(-0.995562, 0.734413, -1.929289);
    mesh.vertexs[63] = Vector3(1.095616, 2.941073, 0.356661);
    mesh.vertexs[64] = Vector3(-1.095615, 2.941073, 0.356661);
    mesh.vertexs[65] = Vector3(1.247486, 2.600116, -1.999972);
    mesh.vertexs[66] = Vector3(-1.247486, 2.600116, -1.999972);
    mesh.vertexs[67] = Vector3(1.248877, 2.889464, -0.49111);
    mesh.vertexs[68] = Vector3(-1.248877, 2.889464, -0.49111);
    mesh.vertexs[69] = Vector3(0.672605, 0.219104, -1.22074);
    mesh.vertexs[70] = Vector3(-0.672605, 0.219104, -1.22074);
    mesh.vertexs[71] = Vector3(0.0, 0.244707, -0.730095);
    mesh.vertexs[72] = Vector3(0.0, 0.217067, -1.710199);
    mesh.vertexs[73] = Vector3(0.0, 3.487749, -2.206993);
    mesh.vertexs[74] = Vector3(0.0, 3.772162, -0.327733);
    mesh.vertexs[75] = Vector3(0.843413, 3.412975, -1.224012);
    mesh.vertexs[76] = Vector3(-0.843413, 3.412975, -1.224012);
    mesh.vertexs[77] = Vector3(1.29095, 1.64247, -1.788828);
    mesh.vertexs[78] = Vector3(-1.290949, 1.64247, -1.788828);
    mesh.vertexs[79] = Vector3(1.35652, 1.921824, -0.65598);
    mesh.vertexs[80] = Vector3(-1.356519, 1.921824, -0.65598);
    mesh.vertexs[81] = Vector3(1.251067, 0.791004, -1.194947);
    mesh.vertexs[82] = Vector3(-1.251067, 0.791004, -1.194947);
    mesh.vertexs[83] = Vector3(0.0, 2.933173, -3.355259);
    mesh.vertexs[84] = Vector3(1.112218, 2.383843, -2.729163);
    mesh.vertexs[85] = Vector3(-1.112218, 2.383843, -2.729163);
    mesh.vertexs[86] = Vector3(0.231035, 0.361439, -2.316163);
    mesh.vertexs[87] = Vector3(-0.231034, 0.361439, -2.316163);
    mesh.vertexs[88] = Vector3(0.0, 0.359988, -2.323414);
    mesh.vertexs[89] = Vector3(0.14517, 0.987087, -3.534714);
    mesh.vertexs[90] = Vector3(-0.14517, 0.987087, -3.534714);
    mesh.vertexs[91] = Vector3(0.0, 1.836306, -3.760285);
    mesh.vertexs[92] = Vector3(0.0, 0.917459, -3.518806);
    mesh.vertexs[93] = Vector3(1.144795, 0.66829, -2.190932);
    mesh.vertexs[94] = Vector3(-1.144795, 0.66829, -2.190932);
    mesh.vertexs[95] = Vector3(0.660897, -0.050216, -3.067415);
    mesh.vertexs[96] = Vector3(-0.660897, -0.050216, -3.067415);
    mesh.vertexs[97] = Vector3(1.182624, 0.859658, -3.032264);
    mesh.vertexs[98] = Vector3(-1.182624, 0.859658, -3.032264);
    mesh.vertexs[99] = Vector3(1.185273, 0.339247, -2.918175);
    mesh.vertexs[100] = Vector3(-1.185272, 0.339247, -2.918175);
    mesh.vertexs[101] = Vector3(0.631473, -0.284753, -2.982881);
    mesh.vertexs[102] = Vector3(-0.631473, -0.284753, -2.982881);
    mesh.vertexs[103] = Vector3(0.37229, -0.107534, -2.272141);
    mesh.vertexs[104] = Vector3(-0.37229, -0.107534, -2.272141);
    mesh.vertexs[105] = Vector3(0.851708, -1.607755, -2.850754);
    mesh.vertexs[106] = Vector3(-0.851708, -1.607755, -2.850754);
    mesh.vertexs[107] = Vector3(0.546894, -2.00994, -3.019514);
    mesh.vertexs[108] = Vector3(-0.546894, -2.00994, -3.019514);
    mesh.vertexs[109] = Vector3(0.296683, -1.61319, -2.77205);
    mesh.vertexs[110] = Vector3(-0.296683, -1.61319, -2.77205);
    mesh.vertexs[111] = Vector3(0.850109, -1.61437, -2.420256);
    mesh.vertexs[112] = Vector3(-0.850109, -1.61437, -2.420256);
    mesh.vertexs[113] = Vector3(0.534877, -1.609278, -2.26268);
    mesh.vertexs[114] = Vector3(-0.534877, -1.609278, -2.26268);
    mesh.vertexs[115] = Vector3(0.215571, -2.00994, -2.273956);
    mesh.vertexs[116] = Vector3(-0.21557, -2.00994, -2.273956);
    mesh.vertexs[117] = Vector3(0.347413, 1.232219, 1.533897);
    mesh.vertexs[118] = Vector3(-0.347413, 1.232219, 1.533897);
    mesh.vertexs[119] = Vector3(0.631074, 1.219622, 1.317006);
    mesh.vertexs[120] = Vector3(-0.631074, 1.219622, 1.317006);
    mesh.vertexs[121] = Vector3(0.0, 1.127572, 1.366131);
    mesh.vertexs[122] = Vector3(1.345901, 2.166615, 0.50938);
    mesh.vertexs[123] = Vector3(-1.3459, 2.166615, 0.50938);
    mesh.vertexs[124] = Vector3(0.855045, 0.579804, -0.082523);
    mesh.vertexs[125] = Vector3(-0.855045, 0.579804, -0.082523);
    mesh.vertexs[126] = Vector3(1.144174, 0.653919, 0.162359);
    mesh.vertexs[127] = Vector3(-1.144173, 0.653919, 0.162359);
    mesh.vertexs[128] = Vector3(0.691814, 0.05254, 0.070559);
    mesh.vertexs[129] = Vector3(-0.691814, 0.05254, 0.070559);
    mesh.vertexs[130] = Vector3(0.264487, 0.229818, 0.128576);
    mesh.vertexs[131] = Vector3(-0.264487, 0.229818, 0.128576);
    mesh.vertexs[132] = Vector3(0.925502, 0.560337, 0.697674);
    mesh.vertexs[133] = Vector3(-0.925502, 0.560337, 0.697674);
    mesh.vertexs[134] = Vector3(0.909784, 0.166099, 0.227564);
    mesh.vertexs[135] = Vector3(-0.909784, 0.166099, 0.227564);
    mesh.vertexs[136] = Vector3(0.176062, 0.439541, 0.68627);
    mesh.vertexs[137] = Vector3(-0.176062, 0.439541, 0.68627);
    mesh.vertexs[138] = Vector3(0.555513, 0.193176, 0.927516);
    mesh.vertexs[139] = Vector3(-0.555513, 0.193176, 0.927516);
    mesh.vertexs[140] = Vector3(0.551165, -2.00994, 0.212188);
    mesh.vertexs[141] = Vector3(-0.551165, -2.00994, 0.212188);
    mesh.vertexs[142] = Vector3(0.273969, -1.129256, 0.415094);
    mesh.vertexs[143] = Vector3(-0.273969, -1.129256, 0.415094);
    mesh.vertexs[144] = Vector3(0.815743, -1.12111, 0.75436);
    mesh.vertexs[145] = Vector3(-0.815742, -1.12111, 0.75436);
    mesh.vertexs[146] = Vector3(0.872811, -2.00994, 0.324614);
    mesh.vertexs[147] = Vector3(-0.872811, -2.00994, 0.324614);
    mesh.vertexs[148] = Vector3(0.209956, -1.12111, 0.751308);
    mesh.vertexs[149] = Vector3(-0.209956, -1.12111, 0.751308);
    mesh.vertexs[150] = Vector3(0.5, -2.00994, 1.043779);
    mesh.vertexs[151] = Vector3(-0.5, -2.00994, 1.043779);
    mesh.vertexs[152] = Vector3(0.494629, 2.941544, 2.908058);
    mesh.vertexs[153] = Vector3(-0.494628, 2.941544, 2.908058);
    mesh.vertexs[154] = Vector3(0.0, 3.435638, 1.278223);
    mesh.vertexs[155] = Vector3(0.578098, 3.427358, 1.841468);
    mesh.vertexs[156] = Vector3(-0.578097, 3.427358, 1.841468);
    mesh.vertexs[157] = Vector3(0.0, 3.387841, 2.529556);
    mesh.vertexs[158] = Vector3(0.0, 2.476262, 3.059709);
    mesh.vertexs[159] = Vector3(0.454832, 1.706591, 2.936781);
    mesh.vertexs[160] = Vector3(-0.454832, 1.706591, 2.936781);
    mesh.vertexs[161] = Vector3(0.0, 1.099739, 1.805865);
    mesh.vertexs[162] = Vector3(0.0, 1.050898, 2.273259);
    mesh.vertexs[163] = Vector3(0.518014, 0.249286, -1.680986);
    mesh.vertexs[164] = Vector3(-0.518013, 0.249286, -1.680986);
    mesh.vertexs[165] = Vector3(0.655495, 0.290537, -0.706458);
    mesh.vertexs[166] = Vector3(-0.655495, 0.290537, -0.706458);
    mesh.vertexs[167] = Vector3(0.74723, 3.136852, -2.112849);
    mesh.vertexs[168] = Vector3(-0.74723, 3.136852, -2.112849);
    mesh.vertexs[169] = Vector3(0.831393, 3.476235, -0.38437);
    mesh.vertexs[170] = Vector3(-0.831393, 3.476235, -0.38437);
    mesh.vertexs[171] = Vector3(1.121891, 0.722922, -1.63841);
    mesh.vertexs[172] = Vector3(-1.121891, 0.722922, -1.63841);
    mesh.vertexs[173] = Vector3(1.253155, 0.928584, -0.709996);
    mesh.vertexs[174] = Vector3(-1.253154, 0.928584, -0.709996);
    mesh.vertexs[175] = Vector3(0.614869, 2.771739, -3.14813);
    mesh.vertexs[176] = Vector3(-0.614869, 2.771739, -3.14813);
    mesh.vertexs[177] = Vector3(0.716122, 0.778791, -3.461078);
    mesh.vertexs[178] = Vector3(-0.716122, 0.778791, -3.461078);
    mesh.vertexs[179] = Vector3(0.96828, 1.363974, -3.255801);
    mesh.vertexs[180] = Vector3(-0.96828, 1.363974, -3.255801);
    mesh.vertexs[181] = Vector3(0.20705, 1.792587, -3.506803);
    mesh.vertexs[182] = Vector3(-0.20705, 1.792587, -3.506803);
    mesh.vertexs[183] = Vector3(0.647175, 0.276252, -3.323141);
    mesh.vertexs[184] = Vector3(-0.647175, 0.276252, -3.323141);
    mesh.vertexs[185] = Vector3(1.240355, 1.345373, -2.373479);
    mesh.vertexs[186] = Vector3(-1.240355, 1.345373, -2.373479);
    mesh.vertexs[187] = Vector3(0.551869, -1.615068, -2.965128);
    mesh.vertexs[188] = Vector3(-0.551868, -1.615068, -2.965128);
    mesh.vertexs[189] = Vector3(0.300653, -1.609263, -2.404913);
    mesh.vertexs[190] = Vector3(-0.300653, -1.609263, -2.404913);
    mesh.vertexs[191] = Vector3(0.964932, 2.932086, 0.956221);
    mesh.vertexs[192] = Vector3(-0.964932, 2.932086, 0.956221);
    mesh.vertexs[193] = Vector3(0.229687, 1.135904, 1.363954);
    mesh.vertexs[194] = Vector3(-0.229686, 1.135904, 1.363954);
    mesh.vertexs[195] = Vector3(1.135738, 1.155481, -0.026897);
    mesh.vertexs[196] = Vector3(-1.135738, 1.155481, -0.026897);
    mesh.vertexs[197] = Vector3(1.088992, 2.353637, 1.043545);
    mesh.vertexs[198] = Vector3(-1.088992, 2.353637, 1.043545);
    mesh.vertexs[199] = Vector3(0.728775, 0.239847, 0.010336);
    mesh.vertexs[200] = Vector3(-0.728775, 0.239847, 0.010336);
    mesh.vertexs[201] = Vector3(0.999041, 0.390784, 0.205899);
    mesh.vertexs[202] = Vector3(-0.999041, 0.390784, 0.205899);
    mesh.vertexs[203] = Vector3(0.5869, 0.521603, 1.012711);
    mesh.vertexs[204] = Vector3(-0.586899, 0.521603, 1.012711);
    mesh.vertexs[205] = Vector3(0.571853, -1.13102, 0.28497);
    mesh.vertexs[206] = Vector3(-0.571853, -1.13102, 0.28497);
    mesh.vertexs[207] = Vector3(0.794631, -1.13102, 0.38404);
    mesh.vertexs[208] = Vector3(-0.794631, -1.13102, 0.38404);
    mesh.vertexs[209] = Vector3(0.48999, -1.13102, 0.900447);
    mesh.vertexs[210] = Vector3(-0.48999, -1.13102, 0.900447);
    mesh.vertexs[211] = Vector3(0.486515, 3.333708, 1.315016);
    mesh.vertexs[212] = Vector3(-0.486515, 3.333708, 1.315016);
    mesh.vertexs[213] = Vector3(0.542219, 3.474965, 2.507201);
    mesh.vertexs[214] = Vector3(-0.542219, 3.474965, 2.507201);
    mesh.vertexs[215] = Vector3(0.466965, 2.589664, 3.041276);
    mesh.vertexs[216] = Vector3(-0.466965, 2.589664, 3.041276);
    mesh.vertexs[217] = Vector3(0.440591, 1.028584, 1.851222);
    mesh.vertexs[218] = Vector3(-0.440591, 1.028584, 1.851222);
    mesh.vertexs[219] = Vector3(0.387348, 1.031212, 2.234724);
    mesh.vertexs[220] = Vector3(-0.387348, 1.031212, 2.234724);
    mesh.vertexs[221] = Vector3(1.239835, 1.527501, 0.919053);
    mesh.vertexs[222] = Vector3(-1.239834, 1.527501, 0.919053);
    mesh.vertexs[223] = Vector3(1.304781, 1.207592, 0.239758);
    mesh.vertexs[224] = Vector3(-1.304781, 1.207592, 0.239758);
    mesh.vertexs[225] = Vector3(0.664619, 0.800481, 1.209078);
    mesh.vertexs[226] = Vector3(-0.664618, 0.800481, 1.209078);
    mesh.vertexs[227] = Vector3(0.157761, 0.766436, 0.811604);
    mesh.vertexs[228] = Vector3(-0.15776, 0.766436, 0.811604);
    mesh.vertexs[229] = Vector3(0.0, 3.66741, 0.309168);
    mesh.vertexs[230] = Vector3(0.7144, 3.446259, 0.357958);
    mesh.vertexs[231] = Vector3(-0.7144, 3.446259, 0.357958);
    mesh.vertexs[232] = Vector3(0.0, 3.58107, 0.840363);
    mesh.vertexs[233] = Vector3(0.615492, 3.403351, 0.851765);
    mesh.vertexs[234] = Vector3(-0.615492, 3.403351, 0.851765);
    mesh.vertexs[235] = Vector3(0.259645, 0.422033, -0.001566);
    mesh.vertexs[236] = Vector3(-0.259645, 0.422033, -0.001566);
    mesh.vertexs[237] = Vector3(0.0, 0.312022, -0.180404);
    mesh.vertexs[238] = Vector3(1.293545, 1.92276, 0.098737);
    mesh.vertexs[239] = Vector3(-1.293544, 1.92276, 0.098737);
    mesh.vertexs[240] = Vector3(0.0, 3.507984, 1.065972);
    mesh.vertexs[241] = Vector3(0.518685, 3.318544, 1.078709);
    mesh.vertexs[242] = Vector3(-0.518685, 3.318544, 1.078709);
    mesh.vertexs[243] = Vector3(0.627094, 1.417171, 1.442486);
    mesh.vertexs[244] = Vector3(-0.627094, 1.417171, 1.442486);
    mesh.vertexs[245] = Vector3(0.948655, 2.395952, 1.238207);
    mesh.vertexs[246] = Vector3(-0.948655, 2.395952, 1.238207);
    mesh.vertexs[247] = Vector3(0.922582, 2.901012, 1.195493);
    mesh.vertexs[248] = Vector3(-0.922582, 2.901012, 1.195493);
    mesh.vertexs[249] = Vector3(0.0, 0.923234, -3.84126);
    mesh.vertexs[250] = Vector3(0.096467, 0.038973, -3.584176);
    mesh.vertexs[251] = Vector3(-0.096467, 0.038973, -3.584176);
    mesh.vertexs[252] = Vector3(0.0, 0.043228, -3.522604);
    mesh.vertexs[253] = Vector3(0.0, 0.043228, -3.745495);
    mesh.vertexs[254] = Vector3(0.074719, -0.608265, -3.55001);
    mesh.vertexs[255] = Vector3(-0.074719, -0.608265, -3.55001);
    mesh.vertexs[256] = Vector3(0.0, -0.60401, -3.62653);
    mesh.vertexs[257] = Vector3(0.0, -0.60401, -3.486953);
    mesh.vertexs[258] = Vector3(0.057031, -1.294491, -3.371183);
    mesh.vertexs[259] = Vector3(-0.057031, -1.294491, -3.371183);
    mesh.vertexs[260] = Vector3(0.0, -1.290236, -3.425111);
    mesh.vertexs[261] = Vector3(0.0, -1.290236, -3.314561);
    mesh.vertexs[262] = Vector3(0.793838, 3.300557, 1.971249);
    mesh.vertexs[263] = Vector3(-0.793838, 3.300557, 1.971249);
    mesh.vertexs[264] = Vector3(0.923073, 2.994228, 2.183931);
    mesh.vertexs[265] = Vector3(-0.923072, 2.994228, 2.183931);
    mesh.vertexs[266] = Vector3(0.841346, 2.816506, 2.671191);
    mesh.vertexs[267] = Vector3(-0.841346, 2.816506, 2.671191);
    mesh.vertexs[268] = Vector3(0.919967, 2.906421, 1.36882);
    mesh.vertexs[269] = Vector3(-0.919967, 2.906421, 1.36882);
    mesh.vertexs[270] = Vector3(0.772379, 2.506588, 2.585693);
    mesh.vertexs[271] = Vector3(-0.772379, 2.506588, 2.585693);
    mesh.vertexs[272] = Vector3(0.783211, 2.02597, 2.193675);
    mesh.vertexs[273] = Vector3(-0.783211, 2.02597, 2.193675);
    mesh.vertexs[274] = Vector3(0.0, 2.187331, 3.129442);
    mesh.vertexs[275] = Vector3(0.822428, 2.40799, 1.88782);
    mesh.vertexs[276] = Vector3(-0.822428, 2.40799, 1.88782);
    mesh.vertexs[277] = Vector3(0.980347, 2.347098, 1.431852);
    mesh.vertexs[278] = Vector3(-0.980346, 2.347098, 1.431852);
    mesh.vertexs[279] = Vector3(0.684757, 1.254725, 1.728601);
    mesh.vertexs[280] = Vector3(-0.684757, 1.254725, 1.728601);
    mesh.vertexs[281] = Vector3(0.594825, 1.656858, 2.32419);
    mesh.vertexs[282] = Vector3(-0.594824, 1.656858, 2.32419);
    mesh.vertexs[283] = Vector3(0.0, 1.280301, 3.236645);
    mesh.vertexs[284] = Vector3(0.409625, 1.251048, 2.955123);
    mesh.vertexs[285] = Vector3(-0.409624, 1.251048, 2.955123);
    mesh.vertexs[286] = Vector3(0.0, 0.232732, 3.198842);
    mesh.vertexs[287] = Vector3(0.254545, 0.169726, 3.004738);
    mesh.vertexs[288] = Vector3(-0.254545, 0.169726, 3.004738);
    mesh.vertexs[289] = Vector3(0.483905, 1.43686, 2.640788);
    mesh.vertexs[290] = Vector3(-0.483905, 1.43686, 2.640788);
    mesh.vertexs[291] = Vector3(0.427262, 1.088948, 2.66592);
    mesh.vertexs[292] = Vector3(-0.427262, 1.088948, 2.66592);
    mesh.vertexs[293] = Vector3(0.260698, 0.091254, 2.764201);
    mesh.vertexs[294] = Vector3(-0.260697, 0.091254, 2.764201);
    mesh.vertexs[295] = Vector3(0.0, 1.184703, 2.464109);
    mesh.vertexs[296] = Vector3(0.0, 0.092903, 2.652447);
    mesh.vertexs[297] = Vector3(0.159055, 0.748817, 2.604924);
    mesh.vertexs[298] = Vector3(-0.159054, 0.748817, 2.604924);
    mesh.vertexs[299] = Vector3(0.0, 0.573897, 2.568213);
    mesh.vertexs[300] = Vector3(0.243038, 1.098148, 2.58079);
    mesh.vertexs[301] = Vector3(-0.243038, 1.098148, 2.58079);
    mesh.vertexs[302] = Vector3(0.0, 0.969701, 2.365579);
    mesh.vertexs[303] = Vector3(0.0, 1.285505, 2.25781);
    mesh.vertexs[304] = Vector3(0.0, 1.339451, 2.444381);
    mesh.vertexs[305] = Vector3(0.724738, 1.584358, 2.076583);
    mesh.vertexs[306] = Vector3(-0.724737, 1.584358, 2.076583);
    mesh.vertexs[307] = Vector3(0.653917, 1.275397, 2.106506);
    mesh.vertexs[308] = Vector3(-0.653917, 1.275397, 2.106506);
    mesh.vertexs[309] = Vector3(0.953592, 1.702016, 1.197974);
    mesh.vertexs[310] = Vector3(-0.953592, 1.702016, 1.197974);
    mesh.vertexs[311] = Vector3(0.748768, 1.725008, 1.363499);
    mesh.vertexs[312] = Vector3(-0.748768, 1.725008, 1.363499);
    mesh.vertexs[313] = Vector3(0.777751, 1.541251, 1.549228);
    mesh.vertexs[314] = Vector3(-0.77775, 1.541251, 1.549228);
    mesh.vertexs[315] = Vector3(0.0, -0.603584, 3.42382);
    mesh.vertexs[316] = Vector3(0.181946, -0.690372, 3.343028);
    mesh.vertexs[317] = Vector3(-0.181946, -0.690372, 3.343028);
    mesh.vertexs[318] = Vector3(0.188098, -0.845165, 3.138151);
    mesh.vertexs[319] = Vector3(-0.188098, -0.845165, 3.138151);
    mesh.vertexs[320] = Vector3(0.0, -0.804155, 3.096275);
    mesh.vertexs[321] = Vector3(0.144291, -1.355688, 3.823553);
    mesh.vertexs[322] = Vector3(-0.14429, -1.355688, 3.823553);
    mesh.vertexs[323] = Vector3(0.0, -1.283736, 3.868135);
    mesh.vertexs[324] = Vector3(0.150443, -1.530599, 3.747084);
    mesh.vertexs[325] = Vector3(-0.150443, -1.530599, 3.747084);
    mesh.vertexs[326] = Vector3(0.0, -1.500103, 3.764431);
    mesh.vertexs[327] = Vector3(0.159379, -1.634308, 3.919721);
    mesh.vertexs[328] = Vector3(-0.159379, -1.634308, 3.919721);
    mesh.vertexs[329] = Vector3(0.0, -1.553479, 3.992316);
    mesh.vertexs[330] = Vector3(0.179773, -1.678559, 3.774787);
    mesh.vertexs[331] = Vector3(-0.179772, -1.678559, 3.774787);
    mesh.vertexs[332] = Vector3(0.0, -1.671046, 3.782751);
    mesh.vertexs[333] = Vector3(1.398977, 3.075111, 1.630607);
    mesh.vertexs[334] = Vector3(-1.398977, 3.075111, 1.630607);
    mesh.vertexs[335] = Vector3(1.024983, 3.230716, 1.91905);
    mesh.vertexs[336] = Vector3(-1.024983, 3.230716, 1.91905);
    mesh.vertexs[337] = Vector3(1.149545, 3.060814, 2.002972);
    mesh.vertexs[338] = Vector3(-1.149545, 3.060814, 2.002972);
    mesh.vertexs[339] = Vector3(0.497734, 1.886112, 2.937304);
    mesh.vertexs[340] = Vector3(-0.497733, 1.886112, 2.937304);
    mesh.vertexs[341] = Vector3(0.643978, 2.198878, 3.044978);
    mesh.vertexs[342] = Vector3(-0.643977, 2.198878, 3.044978);
    mesh.vertexs[343] = Vector3(0.809641, 2.163587, 2.573313);
    mesh.vertexs[344] = Vector3(-0.809641, 2.163587, 2.573313);
    mesh.vertexs[345] = Vector3(0.581339, 1.849421, 2.604416);
    mesh.vertexs[346] = Vector3(-0.581338, 1.849421, 2.604416);
    mesh.vertexs[347] = Vector3(0.645346, 2.038764, 2.648478);
    mesh.vertexs[348] = Vector3(-0.645346, 2.038764, 2.648478);
    mesh.vertexs[349] = Vector3(0.632916, 2.013321, 2.869752);
    mesh.vertexs[350] = Vector3(-0.632916, 2.013321, 2.869752);
    mesh.vertexs[351] = Vector3(0.541719, 1.921597, 2.860386);
    mesh.vertexs[352] = Vector3(-0.541719, 1.921597, 2.860386);
    mesh.vertexs[353] = Vector3(0.570961, 1.958197, 2.687148);
    mesh.vertexs[354] = Vector3(-0.57096, 1.958197, 2.687148);
    mesh.vertexs[355] = Vector3(0.565215, 2.012091, 2.746957);
    mesh.vertexs[356] = Vector3(-0.565214, 2.012091, 2.746957);
    mesh.vertexs[357] = Vector3(0.351174, 1.320581, 2.581601);
    mesh.vertexs[358] = Vector3(-0.351174, 1.320581, 2.581601);
    mesh.vertexs[359] = Vector3(0.0, -1.703652, 3.974355);
    mesh.vertexs[360] = Vector3(0.0, -1.653813, 3.925617);
    mesh.vertexs[361] = Vector3(0.292585, -0.902294, 0.412668);
    mesh.vertexs[362] = Vector3(-0.292585, -0.902294, 0.412668);
    mesh.vertexs[363] = Vector3(0.738537, -0.884757, 0.724892);
    mesh.vertexs[364] = Vector3(-0.738537, -0.884757, 0.724892);
    mesh.vertexs[365] = Vector3(0.285478, -0.893702, 0.733306);
    mesh.vertexs[366] = Vector3(-0.285477, -0.893702, 0.733306);
    mesh.vertexs[367] = Vector3(0.58085, -0.908148, 0.267129);
    mesh.vertexs[368] = Vector3(-0.58085, -0.908148, 0.267129);
    mesh.vertexs[369] = Vector3(0.780689, -0.899631, 0.382191);
    mesh.vertexs[370] = Vector3(-0.780689, -0.899631, 0.382191);
    mesh.vertexs[371] = Vector3(0.494905, -0.8976, 0.840375);
    mesh.vertexs[372] = Vector3(-0.494904, -0.8976, 0.840375);
    mesh.vertexs[373] = Vector3(0.831403, -1.336095, -2.815172);
    mesh.vertexs[374] = Vector3(-0.831403, -1.336095, -2.815172);
    mesh.vertexs[375] = Vector3(0.313411, -1.354315, -2.731287);
    mesh.vertexs[376] = Vector3(-0.313411, -1.354315, -2.731287);
    mesh.vertexs[377] = Vector3(0.824901, -1.318808, -2.470135);
    mesh.vertexs[378] = Vector3(-0.824901, -1.318808, -2.470135);
    mesh.vertexs[379] = Vector3(0.580175, -1.332931, -2.363902);
    mesh.vertexs[380] = Vector3(-0.580175, -1.332931, -2.363902);
    mesh.vertexs[381] = Vector3(0.569524, -1.337606, -2.917296);
    mesh.vertexs[382] = Vector3(-0.569524, -1.337606, -2.917296);
    mesh.vertexs[383] = Vector3(0.325062, -1.329548, -2.469038);
    mesh.vertexs[384] = Vector3(-0.325062, -1.329548, -2.469038);
    mesh.vertexs[385] = Vector3(1.298641, 3.132874, 1.738933);
    mesh.vertexs[386] = Vector3(-1.29864, 3.132874, 1.738933);
    mesh.vertexs[387] = Vector3(1.3374, 3.09547, 1.770139);
    mesh.vertexs[388] = Vector3(-1.3374, 3.09547, 1.770139);
    mesh.vertexs[389] = Vector3(1.128002, 3.174523, 1.859516);
    mesh.vertexs[390] = Vector3(-1.128002, 3.174523, 1.859516);
    mesh.vertexs[391] = Vector3(1.231236, 3.06408, 1.89145);
    mesh.vertexs[392] = Vector3(-1.231236, 3.06408, 1.89145);
    mesh.vertexs[393] = Vector3(1.051525, 2.826521, 1.833987);
    mesh.vertexs[394] = Vector3(-1.051525, 2.826521, 1.833987);
    mesh.vertexs[395] = Vector3(1.140819, 2.957812, 1.791227);
    mesh.vertexs[396] = Vector3(-1.140819, 2.957812, 1.791227);
    mesh.vertexs[397] = Vector3(1.293728, 3.065808, 1.698867);
    mesh.vertexs[398] = Vector3(-1.293728, 3.065808, 1.698867);
    mesh.vertexs[399] = Vector3(1.163861, 1.875032, 1.444103);
    mesh.vertexs[400] = Vector3(-1.163861, 1.875032, 1.444103);
    mesh.vertexs[401] = Vector3(0.857274, 2.610662, 1.98695);
    mesh.vertexs[402] = Vector3(-0.857273, 2.610662, 1.98695);
    mesh.vertexs[403] = Vector3(1.735361, 2.376928, 0.986597);
    mesh.vertexs[404] = Vector3(-1.735361, 2.376928, 0.986597);
    mesh.vertexs[405] = Vector3(1.530894, 2.236285, 1.177014);
    mesh.vertexs[406] = Vector3(-1.530893, 2.236285, 1.177014);
    mesh.vertexs[407] = Vector3(1.369421, 2.960101, 1.637927);
    mesh.vertexs[408] = Vector3(-1.369421, 2.960101, 1.637927);
    mesh.vertexs[409] = Vector3(1.47883, 3.015835, 1.536478);
    mesh.vertexs[410] = Vector3(-1.47883, 3.015835, 1.536478);
    mesh.vertexs[411] = Vector3(1.404114, 2.640053, 1.430707);
    mesh.vertexs[412] = Vector3(-1.404114, 2.640053, 1.430707);
    mesh.vertexs[413] = Vector3(1.59969, 2.726244, 1.34324);
    mesh.vertexs[414] = Vector3(-1.59969, 2.726244, 1.34324);
    mesh.vertexs[415] = Vector3(1.075485, 2.406625, 1.733168);
    mesh.vertexs[416] = Vector3(-1.075485, 2.406625, 1.733168);
    mesh.vertexs[417] = Vector3(1.0181, 1.675889, 1.613462);
    mesh.vertexs[418] = Vector3(-1.018099, 1.675889, 1.613462);
    mesh.vertexs[419] = Vector3(1.317696, 2.038561, 1.307079);
    mesh.vertexs[420] = Vector3(-1.317696, 2.038561, 1.307079);
    mesh.vertexs[421] = Vector3(1.234846, 2.853651, 1.707559);
    mesh.vertexs[422] = Vector3(-1.234846, 2.853651, 1.707559);
    mesh.vertexs[423] = Vector3(1.232531, 2.53436, 1.554136);
    mesh.vertexs[424] = Vector3(-1.232531, 2.53436, 1.554136);
    mesh.vertexs[425] = Vector3(0.287109, -1.729859, 0.421716);
    mesh.vertexs[426] = Vector3(-0.287108, -1.729859, 0.421716);
    mesh.vertexs[427] = Vector3(0.714472, -1.729859, 0.677072);
    mesh.vertexs[428] = Vector3(-0.714472, -1.729859, 0.677072);
    mesh.vertexs[429] = Vector3(0.298364, -1.729859, 0.695593);
    mesh.vertexs[430] = Vector3(-0.298364, -1.729859, 0.695593);
    mesh.vertexs[431] = Vector3(0.527955, -1.732981, 0.30153);
    mesh.vertexs[432] = Vector3(-0.527955, -1.732981, 0.30153);
    mesh.vertexs[433] = Vector3(0.700879, -1.732981, 0.36447);
    mesh.vertexs[434] = Vector3(-0.700879, -1.732981, 0.36447);
    mesh.vertexs[435] = Vector3(0.500433, -1.732981, 0.80619);
    mesh.vertexs[436] = Vector3(-0.500433, -1.732981, 0.80619);
    mesh.vertexs[437] = Vector3(0.881222, -1.814743, -2.872721);
    mesh.vertexs[438] = Vector3(-0.881222, -1.814743, -2.872721);
    mesh.vertexs[439] = Vector3(0.239461, -1.817653, -2.789424);
    mesh.vertexs[440] = Vector3(-0.23946, -1.817653, -2.789424);
    mesh.vertexs[441] = Vector3(0.878865, -1.81805, -2.3871);
    mesh.vertexs[442] = Vector3(-0.878864, -1.81805, -2.3871);
    mesh.vertexs[443] = Vector3(0.554665, -1.815504, -2.213596);
    mesh.vertexs[444] = Vector3(-0.554664, -1.815504, -2.213596);
    mesh.vertexs[445] = Vector3(0.554913, -1.818399, -3.00054);
    mesh.vertexs[446] = Vector3(-0.554913, -1.818399, -3.00054);
    mesh.vertexs[447] = Vector3(0.246326, -1.815497, -2.358441);
    mesh.vertexs[448] = Vector3(-0.246326, -1.815497, -2.358441);
    mesh.vertexs[449] = Vector3(0.159482, -1.833082, 0.329568);
    mesh.vertexs[450] = Vector3(-0.159482, -1.833082, 0.329568);
    mesh.vertexs[451] = Vector3(0.883991, -1.833082, 0.858866);
    mesh.vertexs[452] = Vector3(-0.883991, -1.833082, 0.858866);
    mesh.vertexs[453] = Vector3(0.14274, -1.833082, 0.889953);
    mesh.vertexs[454] = Vector3(-0.142739, -1.833082, 0.889953);
    mesh.vertexs[455] = Vector3(0.550381, -1.834643, 0.201434);
    mesh.vertexs[456] = Vector3(-0.55038, -1.834643, 0.201434);
    mesh.vertexs[457] = Vector3(0.866262, -1.834643, 0.325799);
    mesh.vertexs[458] = Vector3(-0.866262, -1.834643, 0.325799);
    mesh.vertexs[459] = Vector3(0.483831, -1.834643, 1.031705);
    mesh.vertexs[460] = Vector3(-0.48383, -1.834643, 1.031705);
    mesh.vertexs[461] = Vector3(0.892384, -1.881585, -2.88981);
    mesh.vertexs[462] = Vector3(-0.892384, -1.881585, -2.88981);
    mesh.vertexs[463] = Vector3(0.220468, -1.88304, -2.792981);
    mesh.vertexs[464] = Vector3(-0.220467, -1.88304, -2.792981);
    mesh.vertexs[465] = Vector3(0.898962, -1.883238, -2.355534);
    mesh.vertexs[466] = Vector3(-0.898962, -1.883238, -2.355534);
    mesh.vertexs[467] = Vector3(0.550931, -1.881965, -2.123231);
    mesh.vertexs[468] = Vector3(-0.550931, -1.881965, -2.123231);
    mesh.vertexs[469] = Vector3(0.553544, -1.883413, -3.011523);
    mesh.vertexs[470] = Vector3(-0.553543, -1.883413, -3.011523);
    mesh.vertexs[471] = Vector3(0.235744, -1.881962, -2.308993);
    mesh.vertexs[472] = Vector3(-0.235743, -1.881962, -2.308993);
    mesh.v_size = 473;
    mesh.faces[0] = Face(258, 261, 260);
    mesh.faces[1] = Face(260, 261, 259);
    mesh.faces[2] = Face(97, 177, 179);
    mesh.faces[3] = Face(180, 178, 98);
    mesh.faces[4] = Face(4, 185, 84);
    mesh.faces[5] = Face(85, 186, 5);
    mesh.faces[6] = Face(63, 122, 238);
    mesh.faces[7] = Face(239, 123, 64);
    mesh.faces[8] = Face(297, 302, 299);
    mesh.faces[9] = Face(299, 302, 298);
    mesh.faces[10] = Face(162, 297, 299);
    mesh.faces[11] = Face(299, 298, 162);
    mesh.faces[12] = Face(155, 213, 262);
    mesh.faces[13] = Face(263, 214, 156);
    mesh.faces[14] = Face(152, 266, 213);
    mesh.faces[15] = Face(214, 267, 153);
    mesh.faces[16] = Face(159, 339, 56);
    mesh.faces[17] = Face(56, 340, 160);
    mesh.faces[18] = Face(349, 355, 347);
    mesh.faces[19] = Face(348, 356, 350);
    mesh.faces[20] = Face(353, 355, 351);
    mesh.faces[21] = Face(352, 356, 354);
    mesh.faces[22] = Face(289, 304, 357);
    mesh.faces[23] = Face(358, 304, 290);
    mesh.faces[24] = Face(349, 351, 355);
    mesh.faces[25] = Face(356, 352, 350);
    mesh.faces[26] = Face(347, 355, 353);
    mesh.faces[27] = Face(354, 356, 348);
    mesh.faces[28] = Face(327, 359, 360);
    mesh.faces[29] = Face(360, 359, 328);
    mesh.faces[30] = Face(327, 329, 359);
    mesh.faces[31] = Face(359, 329, 328);
    mesh.faces[32] = Face(387, 333, 385);
    mesh.faces[33] = Face(386, 334, 388);
    mesh.faces[34] = Face(333, 387, 397);
    mesh.faces[35] = Face(398, 388, 334);
    mesh.faces[36] = Face(401, 264, 266);
    mesh.faces[37] = Face(267, 265, 402);
    mesh.faces[38] = Face(393, 421, 395);
    mesh.faces[39] = Face(396, 422, 394);
    mesh.faces[40] = Face(48, 140, 146);
    mesh.faces[41] = Face(147, 141, 49);
    mesh.faces[42] = Face(50, 150, 52);
    mesh.faces[43] = Face(53, 151, 51);
    mesh.faces[44] = Face(34, 36, 115);
    mesh.faces[45] = Face(116, 37, 35);
    mesh.faces[46] = Face(30, 107, 32);
    mesh.faces[47] = Face(33, 108, 31);
    mesh.faces[48] = Face(333, 397, 385);
    mesh.faces[49] = Face(386, 398, 334);
    mesh.faces[50] = Face(262, 401, 268);
    mesh.faces[51] = Face(269, 402, 263);
    mesh.faces[52] = Face(276, 402, 278);
    mesh.faces[53] = Face(402, 269, 278);
    mesh.faces[54] = Face(268, 401, 277);
    mesh.faces[55] = Face(401, 275, 277);
    mesh.faces[56] = Face(394, 336, 263);
    mesh.faces[57] = Face(394, 263, 402);
    mesh.faces[58] = Face(262, 335, 393);
    mesh.faces[59] = Face(262, 393, 401);
    mesh.faces[60] = Face(396, 390, 336);
    mesh.faces[61] = Face(396, 336, 394);
    mesh.faces[62] = Face(335, 389, 395);
    mesh.faces[63] = Face(335, 395, 393);
    mesh.faces[64] = Face(396, 398, 386);
    mesh.faces[65] = Face(396, 386, 390);
    mesh.faces[66] = Face(385, 397, 395);
    mesh.faces[67] = Face(385, 395, 389);
    mesh.faces[68] = Face(35, 33, 31);
    mesh.faces[69] = Face(35, 31, 116);
    mesh.faces[70] = Face(30, 32, 34);
    mesh.faces[71] = Face(30, 34, 115);
    mesh.faces[72] = Face(468, 466, 35);
    mesh.faces[73] = Face(468, 35, 37);
    mesh.faces[74] = Face(34, 465, 467);
    mesh.faces[75] = Face(34, 467, 36);
    mesh.faces[76] = Face(468, 444, 466);
    mesh.faces[77] = Face(444, 442, 466);
    mesh.faces[78] = Face(441, 443, 465);
    mesh.faces[79] = Face(443, 467, 465);
    mesh.faces[80] = Face(444, 114, 112);
    mesh.faces[81] = Face(444, 112, 442);
    mesh.faces[82] = Face(111, 113, 443);
    mesh.faces[83] = Face(111, 443, 441);
    mesh.faces[84] = Face(380, 378, 112);
    mesh.faces[85] = Face(380, 112, 114);
    mesh.faces[86] = Face(111, 377, 379);
    mesh.faces[87] = Face(111, 379, 113);
    mesh.faces[88] = Face(380, 29, 378);
    mesh.faces[89] = Face(29, 27, 378);
    mesh.faces[90] = Face(26, 28, 377);
    mesh.faces[91] = Face(28, 379, 377);
    mesh.faces[92] = Face(27, 29, 19);
    mesh.faces[93] = Face(29, 1, 19);
    mesh.faces[94] = Face(0, 28, 18);
    mesh.faces[95] = Face(28, 26, 18);
    mesh.faces[96] = Face(94, 19, 62);
    mesh.faces[97] = Face(19, 1, 62);
    mesh.faces[98] = Face(0, 18, 61);
    mesh.faces[99] = Face(18, 93, 61);
    mesh.faces[100] = Face(94, 62, 186);
    mesh.faces[101] = Face(62, 5, 186);
    mesh.faces[102] = Face(4, 61, 185);
    mesh.faces[103] = Face(61, 93, 185);
    mesh.faces[104] = Face(51, 147, 49);
    mesh.faces[105] = Face(51, 49, 53);
    mesh.faces[106] = Face(48, 146, 50);
    mesh.faces[107] = Face(48, 50, 52);
    mesh.faces[108] = Face(458, 456, 141);
    mesh.faces[109] = Face(458, 141, 147);
    mesh.faces[110] = Face(140, 455, 457);
    mesh.faces[111] = Face(140, 457, 146);
    mesh.faces[112] = Face(458, 434, 456);
    mesh.faces[113] = Face(434, 432, 456);
    mesh.faces[114] = Face(431, 433, 455);
    mesh.faces[115] = Face(433, 457, 455);
    mesh.faces[116] = Face(434, 208, 206);
    mesh.faces[117] = Face(434, 206, 432);
    mesh.faces[118] = Face(205, 207, 433);
    mesh.faces[119] = Face(205, 433, 431);
    mesh.faces[120] = Face(370, 368, 206);
    mesh.faces[121] = Face(370, 206, 208);
    mesh.faces[122] = Face(205, 367, 369);
    mesh.faces[123] = Face(205, 369, 207);
    mesh.faces[124] = Face(370, 135, 129);
    mesh.faces[125] = Face(370, 129, 368);
    mesh.faces[126] = Face(128, 134, 369);
    mesh.faces[127] = Face(128, 369, 367);
    mesh.faces[128] = Face(202, 200, 135);
    mesh.faces[129] = Face(200, 129, 135);
    mesh.faces[130] = Face(128, 199, 134);
    mesh.faces[131] = Face(199, 201, 134);
    mesh.faces[132] = Face(202, 127, 125);
    mesh.faces[133] = Face(202, 125, 200);
    mesh.faces[134] = Face(124, 126, 201);
    mesh.faces[135] = Face(124, 201, 199);
    mesh.faces[136] = Face(224, 196, 127);
    mesh.faces[137] = Face(196, 125, 127);
    mesh.faces[138] = Face(124, 195, 126);
    mesh.faces[139] = Face(195, 223, 126);
    mesh.faces[140] = Face(196, 224, 239);
    mesh.faces[141] = Face(224, 123, 239);
    mesh.faces[142] = Face(122, 223, 238);
    mesh.faces[143] = Face(223, 195, 238);
    mesh.faces[144] = Face(442, 438, 462);
    mesh.faces[145] = Face(442, 462, 466);
    mesh.faces[146] = Face(461, 437, 441);
    mesh.faces[147] = Face(461, 441, 465);
    mesh.faces[148] = Face(466, 462, 35);
    mesh.faces[149] = Face(462, 33, 35);
    mesh.faces[150] = Face(32, 461, 34);
    mesh.faces[151] = Face(461, 465, 34);
    mesh.faces[152] = Face(448, 444, 472);
    mesh.faces[153] = Face(444, 468, 472);
    mesh.faces[154] = Face(467, 443, 471);
    mesh.faces[155] = Face(443, 447, 471);
    mesh.faces[156] = Face(472, 468, 37);
    mesh.faces[157] = Face(472, 37, 116);
    mesh.faces[158] = Face(36, 467, 471);
    mesh.faces[159] = Face(36, 471, 115);
    mesh.faces[160] = Face(116, 31, 472);
    mesh.faces[161] = Face(31, 464, 472);
    mesh.faces[162] = Face(463, 30, 471);
    mesh.faces[163] = Face(30, 115, 471);
    mesh.faces[164] = Face(472, 464, 448);
    mesh.faces[165] = Face(464, 440, 448);
    mesh.faces[166] = Face(439, 463, 447);
    mesh.faces[167] = Face(463, 471, 447);
    mesh.faces[168] = Face(446, 440, 470);
    mesh.faces[169] = Face(440, 464, 470);
    mesh.faces[170] = Face(463, 439, 469);
    mesh.faces[171] = Face(439, 445, 469);
    mesh.faces[172] = Face(470, 464, 108);
    mesh.faces[173] = Face(464, 31, 108);
    mesh.faces[174] = Face(30, 463, 107);
    mesh.faces[175] = Face(463, 469, 107);
    mesh.faces[176] = Face(108, 33, 470);
    mesh.faces[177] = Face(33, 462, 470);
    mesh.faces[178] = Face(461, 32, 469);
    mesh.faces[179] = Face(32, 107, 469);
    mesh.faces[180] = Face(470, 462, 446);
    mesh.faces[181] = Face(462, 438, 446);
    mesh.faces[182] = Face(437, 461, 445);
    mesh.faces[183] = Face(461, 469, 445);
    mesh.faces[184] = Face(426, 430, 450);
    mesh.faces[185] = Face(430, 454, 450);
    mesh.faces[186] = Face(453, 429, 449);
    mesh.faces[187] = Face(429, 425, 449);
    mesh.faces[188] = Face(450, 454, 49);
    mesh.faces[189] = Face(454, 53, 49);
    mesh.faces[190] = Face(52, 453, 48);
    mesh.faces[191] = Face(453, 449, 48);
    mesh.faces[192] = Face(436, 428, 452);
    mesh.faces[193] = Face(436, 452, 460);
    mesh.faces[194] = Face(451, 427, 435);
    mesh.faces[195] = Face(451, 435, 459);
    mesh.faces[196] = Face(460, 452, 151);
    mesh.faces[197] = Face(452, 51, 151);
    mesh.faces[198] = Face(50, 451, 150);
    mesh.faces[199] = Face(451, 459, 150);
    mesh.faces[200] = Face(151, 53, 460);
    mesh.faces[201] = Face(53, 454, 460);
    mesh.faces[202] = Face(453, 52, 459);
    mesh.faces[203] = Face(52, 150, 459);
    mesh.faces[204] = Face(460, 454, 436);
    mesh.faces[205] = Face(454, 430, 436);
    mesh.faces[206] = Face(429, 453, 435);
    mesh.faces[207] = Face(453, 459, 435);
    mesh.faces[208] = Face(147, 51, 452);
    mesh.faces[209] = Face(147, 452, 458);
    mesh.faces[210] = Face(451, 50, 146);
    mesh.faces[211] = Face(451, 146, 457);
    mesh.faces[212] = Face(458, 452, 428);
    mesh.faces[213] = Face(458, 428, 434);
    mesh.faces[214] = Face(427, 451, 457);
    mesh.faces[215] = Face(427, 457, 433);
    mesh.faces[216] = Face(432, 426, 456);
    mesh.faces[217] = Face(426, 450, 456);
    mesh.faces[218] = Face(449, 425, 455);
    mesh.faces[219] = Face(425, 431, 455);
    mesh.faces[220] = Face(456, 450, 141);
    mesh.faces[221] = Face(450, 49, 141);
    mesh.faces[222] = Face(48, 449, 140);
    mesh.faces[223] = Face(449, 455, 140);
    mesh.faces[224] = Face(112, 106, 438);
    mesh.faces[225] = Face(112, 438, 442);
    mesh.faces[226] = Face(437, 105, 111);
    mesh.faces[227] = Face(437, 111, 441);
    mesh.faces[228] = Face(190, 114, 448);
    mesh.faces[229] = Face(114, 444, 448);
    mesh.faces[230] = Face(443, 113, 447);
    mesh.faces[231] = Face(113, 189, 447);
    mesh.faces[232] = Face(448, 440, 190);
    mesh.faces[233] = Face(440, 110, 190);
    mesh.faces[234] = Face(109, 439, 189);
    mesh.faces[235] = Face(439, 447, 189);
    mesh.faces[236] = Face(188, 110, 446);
    mesh.faces[237] = Face(110, 440, 446);
    mesh.faces[238] = Face(439, 109, 445);
    mesh.faces[239] = Face(109, 187, 445);
    mesh.faces[240] = Face(446, 438, 106);
    mesh.faces[241] = Face(446, 106, 188);
    mesh.faces[242] = Face(105, 437, 445);
    mesh.faces[243] = Face(105, 445, 187);
    mesh.faces[244] = Face(143, 149, 430);
    mesh.faces[245] = Face(143, 430, 426);
    mesh.faces[246] = Face(429, 148, 142);
    mesh.faces[247] = Face(429, 142, 425);
    mesh.faces[248] = Face(210, 145, 428);
    mesh.faces[249] = Face(210, 428, 436);
    mesh.faces[250] = Face(427, 144, 209);
    mesh.faces[251] = Face(427, 209, 435);
    mesh.faces[252] = Face(436, 430, 210);
    mesh.faces[253] = Face(430, 149, 210);
    mesh.faces[254] = Face(148, 429, 209);
    mesh.faces[255] = Face(429, 435, 209);
    mesh.faces[256] = Face(434, 428, 208);
    mesh.faces[257] = Face(428, 145, 208);
    mesh.faces[258] = Face(144, 427, 207);
    mesh.faces[259] = Face(427, 433, 207);
    mesh.faces[260] = Face(206, 143, 432);
    mesh.faces[261] = Face(143, 426, 432);
    mesh.faces[262] = Face(425, 142, 431);
    mesh.faces[263] = Face(142, 205, 431);
    mesh.faces[264] = Face(408, 422, 398);
    mesh.faces[265] = Face(422, 396, 398);
    mesh.faces[266] = Face(395, 421, 397);
    mesh.faces[267] = Face(421, 407, 397);
    mesh.faces[268] = Face(416, 394, 424);
    mesh.faces[269] = Face(394, 422, 424);
    mesh.faces[270] = Face(421, 393, 423);
    mesh.faces[271] = Face(393, 415, 423);
    mesh.faces[272] = Face(424, 422, 412);
    mesh.faces[273] = Face(422, 408, 412);
    mesh.faces[274] = Face(407, 421, 411);
    mesh.faces[275] = Face(421, 423, 411);
    mesh.faces[276] = Face(400, 416, 420);
    mesh.faces[277] = Face(416, 424, 420);
    mesh.faces[278] = Face(423, 415, 419);
    mesh.faces[279] = Face(415, 399, 419);
    mesh.faces[280] = Face(420, 424, 406);
    mesh.faces[281] = Face(424, 412, 406);
    mesh.faces[282] = Face(411, 423, 405);
    mesh.faces[283] = Face(423, 419, 405);
    mesh.faces[284] = Face(400, 418, 416);
    mesh.faces[285] = Face(418, 276, 416);
    mesh.faces[286] = Face(275, 417, 415);
    mesh.faces[287] = Face(417, 399, 415);
    mesh.faces[288] = Face(394, 416, 402);
    mesh.faces[289] = Face(416, 276, 402);
    mesh.faces[290] = Face(275, 415, 401);
    mesh.faces[291] = Face(415, 393, 401);
    mesh.faces[292] = Face(404, 406, 414);
    mesh.faces[293] = Face(406, 412, 414);
    mesh.faces[294] = Face(411, 405, 413);
    mesh.faces[295] = Face(405, 403, 413);
    mesh.faces[296] = Face(414, 412, 410);
    mesh.faces[297] = Face(412, 408, 410);
    mesh.faces[298] = Face(407, 411, 409);
    mesh.faces[299] = Face(411, 413, 409);
    mesh.faces[300] = Face(410, 408, 334);
    mesh.faces[301] = Face(408, 398, 334);
    mesh.faces[302] = Face(397, 407, 333);
    mesh.faces[303] = Face(407, 409, 333);
    mesh.faces[304] = Face(402, 265, 394);
    mesh.faces[305] = Face(265, 338, 394);
    mesh.faces[306] = Face(337, 264, 393);
    mesh.faces[307] = Face(264, 401, 393);
    mesh.faces[308] = Face(276, 271, 402);
    mesh.faces[309] = Face(271, 267, 402);
    mesh.faces[310] = Face(266, 270, 401);
    mesh.faces[311] = Face(270, 275, 401);
    mesh.faces[312] = Face(396, 392, 398);
    mesh.faces[313] = Face(392, 388, 398);
    mesh.faces[314] = Face(387, 391, 397);
    mesh.faces[315] = Face(391, 395, 397);
    mesh.faces[316] = Face(396, 394, 338);
    mesh.faces[317] = Face(396, 338, 392);
    mesh.faces[318] = Face(337, 393, 395);
    mesh.faces[319] = Face(337, 395, 391);
    mesh.faces[320] = Face(386, 388, 392);
    mesh.faces[321] = Face(386, 392, 390);
    mesh.faces[322] = Face(391, 387, 385);
    mesh.faces[323] = Face(391, 385, 389);
    mesh.faces[324] = Face(390, 392, 338);
    mesh.faces[325] = Face(390, 338, 336);
    mesh.faces[326] = Face(337, 391, 389);
    mesh.faces[327] = Face(337, 389, 335);
    mesh.faces[328] = Face(106, 112, 378);
    mesh.faces[329] = Face(106, 378, 374);
    mesh.faces[330] = Face(377, 111, 105);
    mesh.faces[331] = Face(377, 105, 373);
    mesh.faces[332] = Face(374, 378, 25);
    mesh.faces[333] = Face(378, 27, 25);
    mesh.faces[334] = Face(26, 377, 24);
    mesh.faces[335] = Face(377, 373, 24);
    mesh.faces[336] = Face(104, 29, 380);
    mesh.faces[337] = Face(104, 380, 384);
    mesh.faces[338] = Face(379, 28, 103);
    mesh.faces[339] = Face(379, 103, 383);
    mesh.faces[340] = Face(384, 380, 190);
    mesh.faces[341] = Face(380, 114, 190);
    mesh.faces[342] = Face(113, 379, 189);
    mesh.faces[343] = Face(379, 383, 189);
    mesh.faces[344] = Face(190, 110, 376);
    mesh.faces[345] = Face(190, 376, 384);
    mesh.faces[346] = Face(375, 109, 189);
    mesh.faces[347] = Face(375, 189, 383);
    mesh.faces[348] = Face(384, 376, 23);
    mesh.faces[349] = Face(384, 23, 104);
    mesh.faces[350] = Face(22, 375, 383);
    mesh.faces[351] = Face(22, 383, 103);
    mesh.faces[352] = Face(102, 23, 382);
    mesh.faces[353] = Face(23, 376, 382);
    mesh.faces[354] = Face(375, 22, 381);
    mesh.faces[355] = Face(22, 101, 381);
    mesh.faces[356] = Face(382, 376, 110);
    mesh.faces[357] = Face(382, 110, 188);
    mesh.faces[358] = Face(109, 375, 381);
    mesh.faces[359] = Face(109, 381, 187);
    mesh.faces[360] = Face(188, 106, 382);
    mesh.faces[361] = Face(106, 374, 382);
    mesh.faces[362] = Face(373, 105, 381);
    mesh.faces[363] = Face(105, 187, 381);
    mesh.faces[364] = Face(382, 374, 102);
    mesh.faces[365] = Face(374, 25, 102);
    mesh.faces[366] = Face(24, 373, 101);
    mesh.faces[367] = Face(373, 381, 101);
    mesh.faces[368] = Face(149, 143, 366);
    mesh.faces[369] = Face(143, 362, 366);
    mesh.faces[370] = Face(361, 142, 365);
    mesh.faces[371] = Face(142, 148, 365);
    mesh.faces[372] = Face(366, 362, 43);
    mesh.faces[373] = Face(366, 43, 47);
    mesh.faces[374] = Face(42, 361, 365);
    mesh.faces[375] = Face(42, 365, 46);
    mesh.faces[376] = Face(139, 45, 364);
    mesh.faces[377] = Face(139, 364, 372);
    mesh.faces[378] = Face(363, 44, 138);
    mesh.faces[379] = Face(363, 138, 371);
    mesh.faces[380] = Face(372, 364, 210);
    mesh.faces[381] = Face(364, 145, 210);
    mesh.faces[382] = Face(144, 363, 209);
    mesh.faces[383] = Face(363, 371, 209);
    mesh.faces[384] = Face(210, 149, 366);
    mesh.faces[385] = Face(210, 366, 372);
    mesh.faces[386] = Face(365, 148, 209);
    mesh.faces[387] = Face(365, 209, 371);
    mesh.faces[388] = Face(372, 366, 47);
    mesh.faces[389] = Face(372, 47, 139);
    mesh.faces[390] = Face(46, 365, 371);
    mesh.faces[391] = Face(46, 371, 138);
    mesh.faces[392] = Face(208, 145, 364);
    mesh.faces[393] = Face(208, 364, 370);
    mesh.faces[394] = Face(363, 144, 207);
    mesh.faces[395] = Face(363, 207, 369);
    mesh.faces[396] = Face(370, 364, 45);
    mesh.faces[397] = Face(370, 45, 135);
    mesh.faces[398] = Face(44, 363, 369);
    mesh.faces[399] = Face(44, 369, 134);
    mesh.faces[400] = Face(129, 43, 368);
    mesh.faces[401] = Face(43, 362, 368);
    mesh.faces[402] = Face(361, 42, 367);
    mesh.faces[403] = Face(42, 128, 367);
    mesh.faces[404] = Face(368, 362, 206);
    mesh.faces[405] = Face(362, 143, 206);
    mesh.faces[406] = Face(142, 361, 205);
    mesh.faces[407] = Face(361, 367, 205);
    mesh.faces[408] = Face(332, 360, 328);
    mesh.faces[409] = Face(332, 328, 331);
    mesh.faces[410] = Face(327, 360, 332);
    mesh.faces[411] = Face(327, 332, 330);
    mesh.faces[412] = Face(236, 237, 39);
    mesh.faces[413] = Face(236, 39, 228);
    mesh.faces[414] = Face(39, 237, 235);
    mesh.faces[415] = Face(39, 235, 227);
    mesh.faces[416] = Face(228, 137, 236);
    mesh.faces[417] = Face(137, 131, 236);
    mesh.faces[418] = Face(130, 136, 235);
    mesh.faces[419] = Face(136, 227, 235);
    mesh.faces[420] = Face(137, 47, 131);
    mesh.faces[421] = Face(47, 43, 131);
    mesh.faces[422] = Face(42, 46, 130);
    mesh.faces[423] = Face(46, 136, 130);
    mesh.faces[424] = Face(346, 282, 160);
    mesh.faces[425] = Face(346, 160, 340);
    mesh.faces[426] = Face(159, 281, 345);
    mesh.faces[427] = Face(159, 345, 339);
    mesh.faces[428] = Face(354, 346, 352);
    mesh.faces[429] = Face(346, 340, 352);
    mesh.faces[430] = Face(339, 345, 351);
    mesh.faces[431] = Face(345, 353, 351);
    mesh.faces[432] = Face(354, 348, 346);
    mesh.faces[433] = Face(348, 344, 346);
    mesh.faces[434] = Face(343, 347, 345);
    mesh.faces[435] = Face(347, 353, 345);
    mesh.faces[436] = Face(346, 344, 273);
    mesh.faces[437] = Face(346, 273, 282);
    mesh.faces[438] = Face(272, 343, 345);
    mesh.faces[439] = Face(272, 345, 281);
    mesh.faces[440] = Face(273, 344, 271);
    mesh.faces[441] = Face(273, 271, 276);
    mesh.faces[442] = Face(270, 343, 272);
    mesh.faces[443] = Face(270, 272, 275);
    mesh.faces[444] = Face(350, 352, 340);
    mesh.faces[445] = Face(350, 340, 342);
    mesh.faces[446] = Face(339, 351, 349);
    mesh.faces[447] = Face(339, 349, 341);
    mesh.faces[448] = Face(348, 350, 344);
    mesh.faces[449] = Face(350, 342, 344);
    mesh.faces[450] = Face(341, 349, 343);
    mesh.faces[451] = Face(349, 347, 343);
    mesh.faces[452] = Face(344, 342, 271);
    mesh.faces[453] = Face(342, 216, 271);
    mesh.faces[454] = Face(215, 341, 270);
    mesh.faces[455] = Face(341, 343, 270);
    mesh.faces[456] = Face(102, 25, 96);
    mesh.faces[457] = Face(25, 21, 96);
    mesh.faces[458] = Face(20, 24, 95);
    mesh.faces[459] = Face(24, 101, 95);
    mesh.faces[460] = Face(102, 96, 23);
    mesh.faces[461] = Face(96, 13, 23);
    mesh.faces[462] = Face(12, 95, 22);
    mesh.faces[463] = Face(95, 101, 22);
    mesh.faces[464] = Face(104, 23, 13);
    mesh.faces[465] = Face(104, 13, 87);
    mesh.faces[466] = Face(12, 22, 103);
    mesh.faces[467] = Face(12, 103, 86);
    mesh.faces[468] = Face(104, 87, 1);
    mesh.faces[469] = Face(104, 1, 29);
    mesh.faces[470] = Face(0, 86, 103);
    mesh.faces[471] = Face(0, 103, 28);
    mesh.faces[472] = Face(25, 27, 21);
    mesh.faces[473] = Face(27, 19, 21);
    mesh.faces[474] = Face(18, 26, 20);
    mesh.faces[475] = Face(26, 24, 20);
    mesh.faces[476] = Face(342, 274, 216);
    mesh.faces[477] = Face(274, 158, 216);
    mesh.faces[478] = Face(158, 274, 215);
    mesh.faces[479] = Face(274, 341, 215);
    mesh.faces[480] = Face(304, 358, 301);
    mesh.faces[481] = Face(304, 301, 303);
    mesh.faces[482] = Face(300, 357, 304);
    mesh.faces[483] = Face(300, 304, 303);
    mesh.faces[484] = Face(358, 290, 308);
    mesh.faces[485] = Face(290, 306, 308);
    mesh.faces[486] = Face(305, 289, 307);
    mesh.faces[487] = Face(289, 357, 307);
    mesh.faces[488] = Face(358, 308, 220);
    mesh.faces[489] = Face(358, 220, 301);
    mesh.faces[490] = Face(219, 307, 357);
    mesh.faces[491] = Face(219, 357, 300);
    mesh.faces[492] = Face(306, 290, 282);
    mesh.faces[493] = Face(290, 160, 282);
    mesh.faces[494] = Face(159, 289, 281);
    mesh.faces[495] = Face(289, 305, 281);
    mesh.faces[496] = Face(274, 342, 340);
    mesh.faces[497] = Face(274, 340, 56);
    mesh.faces[498] = Face(339, 341, 274);
    mesh.faces[499] = Face(339, 274, 56);
    mesh.faces[500] = Face(336, 338, 265);
    mesh.faces[501] = Face(336, 265, 263);
    mesh.faces[502] = Face(264, 337, 335);
    mesh.faces[503] = Face(264, 335, 262);
    mesh.faces[504] = Face(265, 267, 214);
    mesh.faces[505] = Face(265, 214, 263);
    mesh.faces[506] = Face(213, 266, 264);
    mesh.faces[507] = Face(213, 264, 262);
    mesh.faces[508] = Face(301, 220, 162);
    mesh.faces[509] = Face(301, 162, 298);
    mesh.faces[510] = Face(162, 219, 300);
    mesh.faces[511] = Face(162, 300, 297);
    mesh.faces[512] = Face(331, 325, 332);
    mesh.faces[513] = Face(325, 326, 332);
    mesh.faces[514] = Face(326, 324, 332);
    mesh.faces[515] = Face(324, 330, 332);
    mesh.faces[516] = Face(328, 322, 325);
    mesh.faces[517] = Face(328, 325, 331);
    mesh.faces[518] = Face(324, 321, 327);
    mesh.faces[519] = Face(324, 327, 330);
    mesh.faces[520] = Face(329, 323, 322);
    mesh.faces[521] = Face(329, 322, 328);
    mesh.faces[522] = Face(321, 323, 329);
    mesh.faces[523] = Face(321, 329, 327);
    mesh.faces[524] = Face(325, 319, 326);
    mesh.faces[525] = Face(319, 320, 326);
    mesh.faces[526] = Face(320, 318, 326);
    mesh.faces[527] = Face(318, 324, 326);
    mesh.faces[528] = Face(322, 317, 319);
    mesh.faces[529] = Face(322, 319, 325);
    mesh.faces[530] = Face(318, 316, 321);
    mesh.faces[531] = Face(318, 321, 324);
    mesh.faces[532] = Face(323, 315, 317);
    mesh.faces[533] = Face(323, 317, 322);
    mesh.faces[534] = Face(316, 315, 323);
    mesh.faces[535] = Face(316, 323, 321);
    mesh.faces[536] = Face(319, 294, 320);
    mesh.faces[537] = Face(294, 296, 320);
    mesh.faces[538] = Face(296, 293, 320);
    mesh.faces[539] = Face(293, 318, 320);
    mesh.faces[540] = Face(317, 288, 294);
    mesh.faces[541] = Face(317, 294, 319);
    mesh.faces[542] = Face(293, 287, 316);
    mesh.faces[543] = Face(293, 316, 318);
    mesh.faces[544] = Face(315, 286, 288);
    mesh.faces[545] = Face(315, 288, 317);
    mesh.faces[546] = Face(287, 286, 315);
    mesh.faces[547] = Face(287, 315, 316);
    mesh.faces[548] = Face(220, 218, 161);
    mesh.faces[549] = Face(220, 161, 162);
    mesh.faces[550] = Face(161, 217, 219);
    mesh.faces[551] = Face(161, 219, 162);
    mesh.faces[552] = Face(308, 280, 218);
    mesh.faces[553] = Face(308, 218, 220);
    mesh.faces[554] = Face(217, 279, 307);
    mesh.faces[555] = Face(217, 307, 219);
    mesh.faces[556] = Face(306, 314, 280);
    mesh.faces[557] = Face(306, 280, 308);
    mesh.faces[558] = Face(279, 313, 305);
    mesh.faces[559] = Face(279, 305, 307);
    mesh.faces[560] = Face(314, 306, 276);
    mesh.faces[561] = Face(314, 276, 278);
    mesh.faces[562] = Face(275, 305, 313);
    mesh.faces[563] = Face(275, 313, 277);
    mesh.faces[564] = Face(120, 310, 222);
    mesh.faces[565] = Face(310, 198, 222);
    mesh.faces[566] = Face(197, 309, 221);
    mesh.faces[567] = Face(309, 119, 221);
    mesh.faces[568] = Face(278, 246, 312);
    mesh.faces[569] = Face(278, 312, 314);
    mesh.faces[570] = Face(311, 245, 277);
    mesh.faces[571] = Face(311, 277, 313);
    mesh.faces[572] = Face(314, 312, 244);
    mesh.faces[573] = Face(314, 244, 280);
    mesh.faces[574] = Face(243, 311, 313);
    mesh.faces[575] = Face(243, 313, 279);
    mesh.faces[576] = Face(246, 198, 310);
    mesh.faces[577] = Face(246, 310, 312);
    mesh.faces[578] = Face(309, 197, 245);
    mesh.faces[579] = Face(309, 245, 311);
    mesh.faces[580] = Face(312, 310, 244);
    mesh.faces[581] = Face(310, 120, 244);
    mesh.faces[582] = Face(119, 309, 243);
    mesh.faces[583] = Face(309, 311, 243);
    mesh.faces[584] = Face(292, 285, 290);
    mesh.faces[585] = Face(285, 160, 290);
    mesh.faces[586] = Face(159, 284, 289);
    mesh.faces[587] = Face(284, 291, 289);
    mesh.faces[588] = Face(295, 292, 304);
    mesh.faces[589] = Face(292, 290, 304);
    mesh.faces[590] = Face(289, 291, 304);
    mesh.faces[591] = Face(291, 295, 304);
    mesh.faces[592] = Face(303, 301, 302);
    mesh.faces[593] = Face(301, 298, 302);
    mesh.faces[594] = Face(297, 300, 302);
    mesh.faces[595] = Face(300, 303, 302);
    mesh.faces[596] = Face(296, 294, 292);
    mesh.faces[597] = Face(296, 292, 295);
    mesh.faces[598] = Face(291, 293, 296);
    mesh.faces[599] = Face(291, 296, 295);
    mesh.faces[600] = Face(294, 288, 292);
    mesh.faces[601] = Face(288, 285, 292);
    mesh.faces[602] = Face(284, 287, 291);
    mesh.faces[603] = Face(287, 293, 291);
    mesh.faces[604] = Face(286, 283, 285);
    mesh.faces[605] = Face(286, 285, 288);
    mesh.faces[606] = Face(284, 283, 286);
    mesh.faces[607] = Face(284, 286, 287);
    mesh.faces[608] = Face(283, 56, 160);
    mesh.faces[609] = Face(283, 160, 285);
    mesh.faces[610] = Face(159, 56, 283);
    mesh.faces[611] = Face(159, 283, 284);
    mesh.faces[612] = Face(306, 282, 273);
    mesh.faces[613] = Face(306, 273, 276);
    mesh.faces[614] = Face(272, 281, 305);
    mesh.faces[615] = Face(272, 305, 275);
    mesh.faces[616] = Face(280, 244, 118);
    mesh.faces[617] = Face(280, 118, 218);
    mesh.faces[618] = Face(117, 243, 279);
    mesh.faces[619] = Face(117, 279, 217);
    mesh.faces[620] = Face(269, 248, 246);
    mesh.faces[621] = Face(269, 246, 278);
    mesh.faces[622] = Face(245, 247, 268);
    mesh.faces[623] = Face(245, 268, 277);
    mesh.faces[624] = Face(271, 216, 267);
    mesh.faces[625] = Face(216, 153, 267);
    mesh.faces[626] = Face(152, 215, 266);
    mesh.faces[627] = Face(215, 270, 266);
    mesh.faces[628] = Face(269, 263, 212);
    mesh.faces[629] = Face(263, 156, 212);
    mesh.faces[630] = Face(155, 262, 211);
    mesh.faces[631] = Face(262, 268, 211);
    mesh.faces[632] = Face(248, 269, 212);
    mesh.faces[633] = Face(248, 212, 242);
    mesh.faces[634] = Face(211, 268, 247);
    mesh.faces[635] = Face(211, 247, 241);
    mesh.faces[636] = Face(242, 240, 232);
    mesh.faces[637] = Face(242, 232, 234);
    mesh.faces[638] = Face(232, 240, 241);
    mesh.faces[639] = Face(232, 241, 233);
    mesh.faces[640] = Face(242, 234, 192);
    mesh.faces[641] = Face(242, 192, 248);
    mesh.faces[642] = Face(191, 233, 241);
    mesh.faces[643] = Face(191, 241, 247);
    mesh.faces[644] = Face(246, 248, 198);
    mesh.faces[645] = Face(248, 192, 198);
    mesh.faces[646] = Face(191, 247, 197);
    mesh.faces[647] = Face(247, 245, 197);
    mesh.faces[648] = Face(194, 228, 121);
    mesh.faces[649] = Face(228, 39, 121);
    mesh.faces[650] = Face(39, 227, 121);
    mesh.faces[651] = Face(227, 193, 121);
    mesh.faces[652] = Face(228, 194, 226);
    mesh.faces[653] = Face(194, 120, 226);
    mesh.faces[654] = Face(119, 193, 225);
    mesh.faces[655] = Face(193, 227, 225);
    mesh.faces[656] = Face(120, 222, 41);
    mesh.faces[657] = Face(120, 41, 226);
    mesh.faces[658] = Face(40, 221, 119);
    mesh.faces[659] = Face(40, 119, 225);
    mesh.faces[660] = Face(87, 88, 72);
    mesh.faces[661] = Face(87, 72, 164);
    mesh.faces[662] = Face(72, 88, 86);
    mesh.faces[663] = Face(72, 86, 163);
    mesh.faces[664] = Face(164, 58, 1);
    mesh.faces[665] = Face(164, 1, 87);
    mesh.faces[666] = Face(0, 57, 163);
    mesh.faces[667] = Face(0, 163, 86);
    mesh.faces[668] = Face(198, 192, 64);
    mesh.faces[669] = Face(198, 64, 123);
    mesh.faces[670] = Face(63, 191, 197);
    mesh.faces[671] = Face(63, 197, 122);
    mesh.faces[672] = Face(80, 239, 68);
    mesh.faces[673] = Face(239, 64, 68);
    mesh.faces[674] = Face(63, 238, 67);
    mesh.faces[675] = Face(238, 79, 67);
    mesh.faces[676] = Face(80, 68, 7);
    mesh.faces[677] = Face(80, 7, 11);
    mesh.faces[678] = Face(6, 67, 79);
    mesh.faces[679] = Face(6, 79, 10);
    mesh.faces[680] = Face(78, 11, 66);
    mesh.faces[681] = Face(11, 7, 66);
    mesh.faces[682] = Face(6, 10, 65);
    mesh.faces[683] = Face(10, 77, 65);
    mesh.faces[684] = Face(66, 85, 5);
    mesh.faces[685] = Face(66, 5, 78);
    mesh.faces[686] = Face(4, 84, 65);
    mesh.faces[687] = Face(4, 65, 77);
    mesh.faces[688] = Face(98, 186, 180);
    mesh.faces[689] = Face(186, 85, 180);
    mesh.faces[690] = Face(84, 185, 179);
    mesh.faces[691] = Face(185, 97, 179);
    mesh.faces[692] = Face(182, 176, 83);
    mesh.faces[693] = Face(182, 83, 91);
    mesh.faces[694] = Face(83, 175, 181);
    mesh.faces[695] = Face(83, 181, 91);
    mesh.faces[696] = Face(182, 180, 85);
    mesh.faces[697] = Face(182, 85, 176);
    mesh.faces[698] = Face(84, 179, 181);
    mesh.faces[699] = Face(84, 181, 175);
    mesh.faces[700] = Face(94, 186, 98);
    mesh.faces[701] = Face(94, 98, 100);
    mesh.faces[702] = Face(97, 185, 93);
    mesh.faces[703] = Face(97, 93, 99);
    mesh.faces[704] = Face(100, 21, 19);
    mesh.faces[705] = Face(100, 19, 94);
    mesh.faces[706] = Face(18, 20, 99);
    mesh.faces[707] = Face(18, 99, 93);
    mesh.faces[708] = Face(261, 257, 259);
    mesh.faces[709] = Face(257, 255, 259);
    mesh.faces[710] = Face(254, 257, 258);
    mesh.faces[711] = Face(257, 261, 258);
    mesh.faces[712] = Face(259, 255, 260);
    mesh.faces[713] = Face(255, 256, 260);
    mesh.faces[714] = Face(256, 254, 260);
    mesh.faces[715] = Face(254, 258, 260);
    mesh.faces[716] = Face(257, 252, 255);
    mesh.faces[717] = Face(252, 251, 255);
    mesh.faces[718] = Face(250, 252, 254);
    mesh.faces[719] = Face(252, 257, 254);
    mesh.faces[720] = Face(255, 251, 256);
    mesh.faces[721] = Face(251, 253, 256);
    mesh.faces[722] = Face(253, 250, 256);
    mesh.faces[723] = Face(250, 254, 256);
    mesh.faces[724] = Face(252, 92, 251);
    mesh.faces[725] = Face(92, 90, 251);
    mesh.faces[726] = Face(89, 92, 250);
    mesh.faces[727] = Face(92, 252, 250);
    mesh.faces[728] = Face(253, 251, 249);
    mesh.faces[729] = Face(251, 90, 249);
    mesh.faces[730] = Face(89, 250, 249);
    mesh.faces[731] = Face(250, 253, 249);
    mesh.faces[732] = Face(91, 249, 90);
    mesh.faces[733] = Face(91, 90, 182);
    mesh.faces[734] = Face(89, 249, 91);
    mesh.faces[735] = Face(89, 91, 181);
    mesh.faces[736] = Face(180, 182, 90);
    mesh.faces[737] = Face(180, 90, 178);
    mesh.faces[738] = Face(89, 181, 179);
    mesh.faces[739] = Face(89, 179, 177);
    mesh.faces[740] = Face(92, 17, 90);
    mesh.faces[741] = Face(17, 16, 90);
    mesh.faces[742] = Face(15, 17, 89);
    mesh.faces[743] = Face(17, 92, 89);
    mesh.faces[744] = Face(17, 14, 16);
    mesh.faces[745] = Face(14, 13, 16);
    mesh.faces[746] = Face(12, 14, 15);
    mesh.faces[747] = Face(14, 17, 15);
    mesh.faces[748] = Face(88, 87, 14);
    mesh.faces[749] = Face(87, 13, 14);
    mesh.faces[750] = Face(12, 86, 14);
    mesh.faces[751] = Face(86, 88, 14);
    mesh.faces[752] = Face(176, 85, 168);
    mesh.faces[753] = Face(85, 66, 168);
    mesh.faces[754] = Face(65, 84, 167);
    mesh.faces[755] = Face(84, 175, 167);
    mesh.faces[756] = Face(176, 168, 73);
    mesh.faces[757] = Face(176, 73, 83);
    mesh.faces[758] = Face(73, 167, 175);
    mesh.faces[759] = Face(73, 175, 83);
    mesh.faces[760] = Face(184, 100, 178);
    mesh.faces[761] = Face(100, 98, 178);
    mesh.faces[762] = Face(97, 99, 177);
    mesh.faces[763] = Face(99, 183, 177);
    mesh.faces[764] = Face(178, 90, 16);
    mesh.faces[765] = Face(178, 16, 184);
    mesh.faces[766] = Face(15, 89, 177);
    mesh.faces[767] = Face(15, 177, 183);
    mesh.faces[768] = Face(236, 166, 71);
    mesh.faces[769] = Face(236, 71, 237);
    mesh.faces[770] = Face(71, 165, 235);
    mesh.faces[771] = Face(71, 235, 237);
    mesh.faces[772] = Face(236, 125, 166);
    mesh.faces[773] = Face(125, 60, 166);
    mesh.faces[774] = Face(59, 124, 165);
    mesh.faces[775] = Face(124, 235, 165);
    mesh.faces[776] = Face(231, 229, 74);
    mesh.faces[777] = Face(231, 74, 170);
    mesh.faces[778] = Face(74, 229, 230);
    mesh.faces[779] = Face(74, 230, 169);
    mesh.faces[780] = Face(170, 68, 64);
    mesh.faces[781] = Face(170, 64, 231);
    mesh.faces[782] = Face(63, 67, 169);
    mesh.faces[783] = Face(63, 169, 230);
    mesh.faces[784] = Face(196, 239, 80);
    mesh.faces[785] = Face(196, 80, 174);
    mesh.faces[786] = Face(79, 238, 195);
    mesh.faces[787] = Face(79, 195, 173);
    mesh.faces[788] = Face(196, 174, 125);
    mesh.faces[789] = Face(174, 60, 125);
    mesh.faces[790] = Face(59, 173, 124);
    mesh.faces[791] = Face(173, 195, 124);
    mesh.faces[792] = Face(120, 194, 118);
    mesh.faces[793] = Face(120, 118, 244);
    mesh.faces[794] = Face(117, 193, 119);
    mesh.faces[795] = Face(117, 119, 243);
    mesh.faces[796] = Face(212, 154, 242);
    mesh.faces[797] = Face(154, 240, 242);
    mesh.faces[798] = Face(240, 154, 241);
    mesh.faces[799] = Face(154, 211, 241);
    mesh.faces[800] = Face(131, 200, 236);
    mesh.faces[801] = Face(200, 125, 236);
    mesh.faces[802] = Face(124, 199, 235);
    mesh.faces[803] = Face(199, 130, 235);
    mesh.faces[804] = Face(234, 231, 192);
    mesh.faces[805] = Face(231, 64, 192);
    mesh.faces[806] = Face(63, 230, 191);
    mesh.faces[807] = Face(230, 233, 191);
    mesh.faces[808] = Face(229, 231, 234);
    mesh.faces[809] = Face(229, 234, 232);
    mesh.faces[810] = Face(233, 230, 229);
    mesh.faces[811] = Face(233, 229, 232);
    mesh.faces[812] = Face(16, 13, 184);
    mesh.faces[813] = Face(13, 96, 184);
    mesh.faces[814] = Face(95, 12, 183);
    mesh.faces[815] = Face(12, 15, 183);
    mesh.faces[816] = Face(222, 198, 123);
    mesh.faces[817] = Face(222, 123, 224);
    mesh.faces[818] = Face(122, 197, 221);
    mesh.faces[819] = Face(122, 221, 223);
    mesh.faces[820] = Face(226, 204, 228);
    mesh.faces[821] = Face(204, 137, 228);
    mesh.faces[822] = Face(136, 203, 227);
    mesh.faces[823] = Face(203, 225, 227);
    mesh.faces[824] = Face(41, 133, 226);
    mesh.faces[825] = Face(133, 204, 226);
    mesh.faces[826] = Face(203, 132, 225);
    mesh.faces[827] = Face(132, 40, 225);
    mesh.faces[828] = Face(224, 127, 41);
    mesh.faces[829] = Face(224, 41, 222);
    mesh.faces[830] = Face(40, 126, 223);
    mesh.faces[831] = Face(40, 223, 221);
    mesh.faces[832] = Face(118, 38, 161);
    mesh.faces[833] = Face(118, 161, 218);
    mesh.faces[834] = Face(161, 38, 117);
    mesh.faces[835] = Face(161, 117, 217);
    mesh.faces[836] = Face(158, 54, 216);
    mesh.faces[837] = Face(54, 153, 216);
    mesh.faces[838] = Face(152, 54, 215);
    mesh.faces[839] = Face(54, 158, 215);
    mesh.faces[840] = Face(153, 54, 157);
    mesh.faces[841] = Face(153, 157, 214);
    mesh.faces[842] = Face(157, 54, 152);
    mesh.faces[843] = Face(157, 152, 213);
    mesh.faces[844] = Face(157, 55, 214);
    mesh.faces[845] = Face(55, 156, 214);
    mesh.faces[846] = Face(155, 55, 213);
    mesh.faces[847] = Face(55, 157, 213);
    mesh.faces[848] = Face(156, 55, 212);
    mesh.faces[849] = Face(55, 154, 212);
    mesh.faces[850] = Face(154, 55, 211);
    mesh.faces[851] = Face(55, 155, 211);
    mesh.faces[852] = Face(133, 45, 204);
    mesh.faces[853] = Face(45, 139, 204);
    mesh.faces[854] = Face(138, 44, 203);
    mesh.faces[855] = Face(44, 132, 203);
    mesh.faces[856] = Face(139, 47, 137);
    mesh.faces[857] = Face(139, 137, 204);
    mesh.faces[858] = Face(136, 46, 138);
    mesh.faces[859] = Face(136, 138, 203);
    mesh.faces[860] = Face(135, 45, 202);
    mesh.faces[861] = Face(45, 133, 202);
    mesh.faces[862] = Face(132, 44, 201);
    mesh.faces[863] = Face(44, 134, 201);
    mesh.faces[864] = Face(133, 41, 127);
    mesh.faces[865] = Face(133, 127, 202);
    mesh.faces[866] = Face(126, 40, 132);
    mesh.faces[867] = Face(126, 132, 201);
    mesh.faces[868] = Face(131, 43, 129);
    mesh.faces[869] = Face(131, 129, 200);
    mesh.faces[870] = Face(128, 42, 130);
    mesh.faces[871] = Face(128, 130, 199);
    mesh.faces[872] = Face(121, 38, 194);
    mesh.faces[873] = Face(38, 118, 194);
    mesh.faces[874] = Face(117, 38, 193);
    mesh.faces[875] = Face(38, 121, 193);
    mesh.faces[876] = Face(96, 21, 100);
    mesh.faces[877] = Face(96, 100, 184);
    mesh.faces[878] = Face(99, 20, 95);
    mesh.faces[879] = Face(99, 95, 183);
    mesh.faces[880] = Face(80, 11, 174);
    mesh.faces[881] = Face(11, 82, 174);
    mesh.faces[882] = Face(81, 10, 173);
    mesh.faces[883] = Face(10, 79, 173);
    mesh.faces[884] = Face(82, 3, 60);
    mesh.faces[885] = Face(82, 60, 174);
    mesh.faces[886] = Face(59, 2, 81);
    mesh.faces[887] = Face(59, 81, 173);
    mesh.faces[888] = Face(82, 11, 78);
    mesh.faces[889] = Face(82, 78, 172);
    mesh.faces[890] = Face(77, 10, 81);
    mesh.faces[891] = Face(77, 81, 171);
    mesh.faces[892] = Face(78, 5, 62);
    mesh.faces[893] = Face(78, 62, 172);
    mesh.faces[894] = Face(61, 4, 77);
    mesh.faces[895] = Face(61, 77, 171);
    mesh.faces[896] = Face(62, 1, 58);
    mesh.faces[897] = Face(62, 58, 172);
    mesh.faces[898] = Face(57, 0, 61);
    mesh.faces[899] = Face(57, 61, 171);
    mesh.faces[900] = Face(58, 3, 172);
    mesh.faces[901] = Face(3, 82, 172);
    mesh.faces[902] = Face(81, 2, 171);
    mesh.faces[903] = Face(2, 57, 171);
    mesh.faces[904] = Face(74, 9, 170);
    mesh.faces[905] = Face(9, 76, 170);
    mesh.faces[906] = Face(75, 9, 169);
    mesh.faces[907] = Face(9, 74, 169);
    mesh.faces[908] = Face(76, 7, 68);
    mesh.faces[909] = Face(76, 68, 170);
    mesh.faces[910] = Face(67, 6, 75);
    mesh.faces[911] = Face(67, 75, 169);
    mesh.faces[912] = Face(76, 9, 168);
    mesh.faces[913] = Face(9, 73, 168);
    mesh.faces[914] = Face(73, 9, 167);
    mesh.faces[915] = Face(9, 75, 167);
    mesh.faces[916] = Face(66, 7, 168);
    mesh.faces[917] = Face(7, 76, 168);
    mesh.faces[918] = Face(75, 6, 167);
    mesh.faces[919] = Face(6, 65, 167);
    mesh.faces[920] = Face(70, 8, 71);
    mesh.faces[921] = Face(70, 71, 166);
    mesh.faces[922] = Face(71, 8, 69);
    mesh.faces[923] = Face(71, 69, 165);
    mesh.faces[924] = Face(60, 3, 166);
    mesh.faces[925] = Face(3, 70, 166);
    mesh.faces[926] = Face(69, 2, 165);
    mesh.faces[927] = Face(2, 59, 165);
    mesh.faces[928] = Face(72, 8, 164);
    mesh.faces[929] = Face(8, 70, 164);
    mesh.faces[930] = Face(69, 8, 163);
    mesh.faces[931] = Face(8, 72, 163);
    mesh.faces[932] = Face(70, 3, 58);
    mesh.faces[933] = Face(70, 58, 164);
    mesh.faces[934] = Face(57, 2, 69);
    mesh.faces[935] = Face(57, 69, 163);
    mesh.f_size = 936;
    
    
    wf0.init( 0, 0, 80, 80, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);
    wf1.init(80, 0, 80, 80, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 1);
    
    // wf0.init( 0,  0, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);
    
    // wf1.init( 0,  0, 40, 40, M5.Lcd.color565(140, 32, 55), M5.Lcd.color565(200, 200, 200), mesh, 0);
    // wf2.init(16, 16, 40, 40, M5.Lcd.color565(160, 36, 65), M5.Lcd.color565(200, 200, 200), mesh, 0);
    // wf3.init(32, 32, 40, 40, M5.Lcd.color565(180, 40, 75), M5.Lcd.color565(200, 200, 200), mesh, 0);
    // wf4.init(48, 48, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);
    
    
    // wf21.init(32+ 0,  0, 40, 40, M5.Lcd.color565(140, 32, 55), M5.Lcd.color565(200, 200, 200), mesh, 1);
    // wf22.init(32+16, 16, 40, 40, M5.Lcd.color565(160, 36, 65), M5.Lcd.color565(200, 200, 200), mesh, 1);
    // wf23.init(32+32, 32, 40, 40, M5.Lcd.color565(180, 40, 75), M5.Lcd.color565(200, 200, 200), mesh, 1);
    // wf24.init(32+48, 48, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 1);
    
    
    windowmanager.add(wf1);
    // windowmanager.add(wf2);
    // windowmanager.add(wf3);
    // windowmanager.add(wf4);
    // windowmanager.add(wf21);
    // windowmanager.add(wf22);
    // windowmanager.add(wf23);
    // windowmanager.add(wf24);
    windowmanager.add(wf0);
    
    M5.MPU6886.Init();
}

float accX = 0;
float accY = 0;
float accZ = 0;
float gyroX = 0;
float gyroY = 0;
float gyroZ = 0;
float temp = 0;
void loop() {
    M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
    M5.MPU6886.getAccelData(&accX, &accY, &accZ);
    M5.MPU6886.getTempData(&temp);
    // rot 0
    // int x = accY*20;
    // int y = -accX*20;
    
    // rot 1
    // int x = accY*20;
    // int y = accX*20;
    
    // wf0.sx = max(min(wf0.sx + x, 120), 0);
    // wf0.sy = max(min(wf0.sy + y, 40), 0);
    
    
    windowmanager.draw();
    
    delay(16);
    
}