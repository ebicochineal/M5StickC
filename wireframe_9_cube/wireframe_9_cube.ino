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
    uint16_t* buff = NULL;
    uint16_t* zbuff = NULL;
    int8_t sx = 0;
    int8_t sy = 0;
    int8_t ex = 0;
    int8_t ey = 0;
    uint16_t width, height;
    uint16_t buffsize = 0;
    Object3D obj;
    Camera3D camera;
    uint16_t bgcolor = 0;
    uint16_t linecolor = 0;
    
    int render_type = 0;
    
    WireFrame (int8_t sx, int8_t sy, uint8_t width, uint8_t height, uint16_t bgcolor, uint16_t linecolor, Mesh& mesh, int render_type) {
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
        
        this->render_type = render_type;
    }
    
    ~WireFrame () {
        delete[] this->buff;
        delete[] this->zbuff;
    }
    
    void draw () {
        //
        // this->obj.position.z = -12;
        // this->obj.position.y = -1;
        // this->obj.rotation.y = this->obj.rotation.y + 6;
        
        this->obj.position.z = -6;
        this->obj.rotation.y = this->obj.rotation.y + 6;
        
        //
        
        
        if (this->bnew) {
            this->buff = new uint16_t[this->buffsize];
            this->zbuff = new uint16_t[this->buffsize];
            this->bnew = false;
        }
        
        
        this->clear();
        
        this->worldviewTransform();
        
        if (this->render_type == 0) {
            this->projscreenTransform();
            this->drawWire();
        } else {
            if (this->render_type == 1) {
                this->polygonColor();
            } else {
                this->normalColor();
            }
            this->projscreenTransform();
            this->drawPolygon();
        }
    }
private:
    bool bnew = true;
    
    void polygonColor () {
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            const Vector3 n = Vector3::normalize(Vector3::cross(v1-v0, v2-v0));
            const float d = min(max(Vector3::dot(Vector3(1.0f, 0.5f, 0.5f), n), 0.1f), 1.0f) * 255.0f;
            this->obj.mesh->colors[i] = M5.Lcd.color565(d, d, d);
        }
    }
    
    void normalColor () {
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            const Vector3 n = (Vector3::normalize(Vector3::cross(v1-v0, v2-v0)) * 0.5f + 0.5f) * 255.0f;
            this->obj.mesh->colors[i] = M5.Lcd.color565(n.x, n.y, n.z);
        }
    }
    
    void projscreenTransform () {
        Matrix4x4 mat = Matrix4x4::projscreenMatrix(sx, sy, this->width, this->height);
        
        for (int i = 0; i < this->obj.mesh->v_size; ++i) {
            Vector3 t = this->obj.mesh->tvertexs[i];
            t = Matrix4x4::mul(t, mat);
            this->obj.mesh->tvertexs[i] = t;
        }
    }
    
    void worldviewTransform () {
        Matrix4x4 mat = Matrix4x4::identity();
        
        // world
        mat = Matrix4x4::mul(mat, Matrix4x4::rotMatrix(this->obj.rotation));
        mat = Matrix4x4::mul(mat, Matrix4x4::movMatrix(this->obj.position));
        
        // viwe
        mat = Matrix4x4::mul(mat, Matrix4x4::rotMatrix(Vector3() - this->camera.rotation));
        mat = Matrix4x4::mul(mat, Matrix4x4::movMatrix(Vector3() - this->camera.position));
        
        // WorldViewTransform
        for (int i = 0; i < this->obj.mesh->v_size; ++i) {
            Vector3 t = this->obj.mesh->vertexs[i];
            t = Matrix4x4::mul(t, mat);
            this->obj.mesh->tvertexs[i] = t;
        }
    }
    
    void clear () {
        for (int y = 0; y < this->height; ++y) {
            for (int x = 0; x < this->width; ++x) {
                this->buff[y*this->width+x] = this->bgcolor;
                this->zbuff[y*this->width+x] = 0;
            }
        }
    }
    
    void drawWire () {for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            
            if (Vector3::cross(v1 - v0, v2 - v1).z > 0) { continue; }
            
            if (!((v0.z > 0 && v0.z < 1) || (v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1))) { continue; }
            if (!((v0.x >= 0 && v0.x < this->width) || (v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width))) { continue; }
            if (!((v0.y >= 0 && v0.y < this->height) || (v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height))) { continue; }
            
            this->drawBuffLine(v0.x, v0.y, v1.x, v1.y, this->linecolor);
            this->drawBuffLine(v1.x, v1.y, v2.x, v2.y, this->linecolor);
            this->drawBuffLine(v2.x, v2.y, v0.x, v0.y, this->linecolor);
        }
    }
    
    void drawPolygon () {for (int i = 0; i < this->obj.mesh->f_size; ++i) {
            const Face& f = this->obj.mesh->faces[i];
            const Vector3& v0 = this->obj.mesh->tvertexs[f.x];
            const Vector3& v1 = this->obj.mesh->tvertexs[f.y];
            const Vector3& v2 = this->obj.mesh->tvertexs[f.z];
            
            if (Vector3::cross(v1 - v0, v2 - v1).z > 0) { continue; }
            if (!((v0.z > 0 && v0.z < 1) || (v1.z > 0 && v1.z < 1) || (v2.z > 0 && v2.z < 1))) { continue; }
            if (!((v0.x >= 0 && v0.x < this->width) || (v1.x >= 0 && v1.x < this->width) || (v2.x >= 0 && v2.x < this->width))) { continue; }
            if (!((v0.y >= 0 && v0.y < this->height) || (v1.y >= 0 && v1.y < this->height) || (v2.y >= 0 && v2.y < this->height))) { continue; }
            uint16_t z = (1.0f-(v0.z+v1.z+v2.z)*0.333f) * 32767;
            this->fillTriangle(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y, this->obj.mesh->colors[i], z);
        }
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
    uint16_t* buff;
    WireFrame* wireframes[32];
    uint8_t wsize = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint16_t buffsize = 0;
    WindowManager (uint16_t width, uint16_t height) {
        this->width = width;
        this->height = height;
        this->buffsize = height * width;
        this->buff = new uint16_t[this->buffsize];
    }
    
    void add (WireFrame& wf) {
        this->wireframes[this->wsize] = &wf;
        this->wsize += 1;
    }
    
    void add (int8_t sx, int8_t sy, uint8_t width, uint8_t height, uint16_t bgcolor, uint16_t linecolor, Mesh& mesh, int rtype) {
        WireFrame* wf = new WireFrame(sx, sy, width, height, bgcolor, linecolor, mesh, rtype);
        this->wireframes[this->wsize] = wf;
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


WindowManager wm(160, 80);

Mesh mesh;
WireFrame wf0( 0,  0, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf1( 0,  0, 40, 40, M5.Lcd.color565(140, 32, 55), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf2(16, 16, 40, 40, M5.Lcd.color565(160, 36, 65), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf3(32, 32, 40, 40, M5.Lcd.color565(180, 40, 75), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf4(48, 48, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf21(32+ 0,  0, 40, 40, M5.Lcd.color565(140, 32, 55), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf22(32+16, 16, 40, 40, M5.Lcd.color565(160, 36, 65), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf23(32+32, 32, 40, 40, M5.Lcd.color565(180, 40, 75), M5.Lcd.color565(200, 200, 200), mesh, 0);
WireFrame wf24(32+48, 48, 40, 40, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh, 0);

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    
    {
        mesh.vertexs[0] = Vector3( 1.0f, -1.0f, -1.0f);
        mesh.vertexs[1] = Vector3( 1.0f, -1.0f,  1.0f);
        mesh.vertexs[2] = Vector3(-1.0f, -1.0f,  1.0f);
        mesh.vertexs[3] = Vector3(-1.0f, -1.0f, -1.0f);
        mesh.vertexs[4] = Vector3( 1.0f,  1.0f, -1.0f);
        mesh.vertexs[5] = Vector3( 1.0f,  1.0f,  1.0f);
        mesh.vertexs[6] = Vector3(-1.0f,  1.0f,  1.0f);
        mesh.vertexs[7] = Vector3(-1.0f,  1.0f, -1.0f);
        mesh.v_size = 8;
        
        mesh.faces[0]  = Face(1, 3, 0);
        mesh.faces[1]  = Face(7, 5, 4);
        mesh.faces[2]  = Face(4, 1, 0);
        mesh.faces[3]  = Face(5, 2, 1);
        mesh.faces[4]  = Face(2, 7, 3);
        mesh.faces[5]  = Face(0, 7, 4);
        mesh.faces[6]  = Face(1, 2, 3);
        mesh.faces[7]  = Face(7, 6, 5);
        mesh.faces[8]  = Face(4, 5, 1);
        mesh.faces[9]  = Face(5, 6, 2);
        mesh.faces[10] = Face(2, 6, 7);
        mesh.faces[11] = Face(0, 3, 7);
        mesh.f_size = 12;
    }
    
    wm.add(wf1);
    wm.add(wf2);
    wm.add(wf3);
    wm.add(wf4);
    wm.add(wf21);
    wm.add(wf22);
    wm.add(wf23);
    wm.add(wf24);
    wm.add(wf0);
    
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
    int x = accY*20;
    int y = accX*20;
    
    wf0.sx = max(min(wf0.sx + x, 120), 0);
    wf0.sy = max(min(wf0.sy + y, 40), 0);
    
    
    wm.draw();
    
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print(wf0.sx);
    M5.Lcd.print(", ");
    M5.Lcd.print(wf0.sy);
    
    delay(16);
}