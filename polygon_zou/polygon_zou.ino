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
    uint16_t array_v_size = 0;
    uint16_t array_f_size = 0;
    Vector3* vertexs = NULL;
    Vector3* tvertexs = NULL;
    Face* faces = NULL;
    uint16_t* colors = NULL;
    Mesh () {
        this->vertexs = new Vector3[8];
        this->tvertexs = new Vector3[8];
        this->faces = new Face[8];
        this->colors = new uint16_t[8];
        this->array_v_size = 8;
        this->array_f_size = 8;
    };
    
    ~Mesh () {
        delete[] this->vertexs;
        delete[] this->tvertexs;
        delete[] this->faces;
        delete[] this->colors;
    };
    
    Mesh (const Mesh& t) {
        Vector3* vertexs = new Vector3[t.array_v_size];
        Vector3* tvertexs = new Vector3[t.array_v_size];
        Face* faces = new Face[t.array_f_size];
        uint16_t* colors = new uint16_t[t.array_f_size];
        for (int i = 0; i < t.v_size; ++i) { vertexs[i] = t.vertexs[i]; }
        for (int i = 0; i < t.f_size; ++i) { faces[i] = t.faces[i]; }
        this->v_size = t.v_size;
        this->f_size = t.f_size;
        this->array_v_size = t.array_v_size;
        this->array_f_size = t.array_f_size;
        this->vertexs = vertexs;
        this->tvertexs = tvertexs;
        this->faces = faces;
        this->colors = colors;
    }
    
    Mesh& operator = (const Mesh& t) {
        Vector3* vertexs = new Vector3[t.array_v_size];
        Vector3* tvertexs = new Vector3[t.array_v_size];
        Face* faces = new Face[t.array_f_size];
        uint16_t* colors = new uint16_t[t.array_f_size];
        for (int i = 0; i < t.v_size; ++i) { vertexs[i] = t.vertexs[i]; }
        for (int i = 0; i < t.f_size; ++i) { faces[i] = t.faces[i]; }
        this->v_size = t.v_size;
        this->f_size = t.f_size;
        this->array_v_size = t.array_v_size;
        this->array_f_size = t.array_f_size;
        this->vertexs = vertexs;
        this->tvertexs = tvertexs;
        this->faces = faces;
        this->colors = colors;
        return *this;
    }
    
    void addVertex (float x, float y, float z) { this->addVertex(Vector3(x, y, z)); }
    
    void addVertex (Vector3 v) {
        if (this->v_size + 1 > this->array_v_size) {
            if (this->array_v_size < 128) {
                this->array_v_size += 8;
            } else if (this->array_v_size < 512) {
                this->array_v_size += 64;
            } else {
                this->array_v_size += 128;
            }
            Vector3* vertexs = new Vector3[this->array_v_size];
            Vector3* tvertexs = new Vector3[this->array_v_size];
            
            for (int i = 0; i < this->v_size; ++i) { vertexs[i] = this->vertexs[i]; }
            vertexs[this->v_size] = v;
            delete[] this->vertexs;
            delete[] this->tvertexs;
            this->vertexs = vertexs;
            this->tvertexs = tvertexs;
            this->v_size += 1;
        } else {
            this->vertexs[this->v_size] = v;
            this->v_size += 1;
        }
    }
    
    void addFace (uint16_t x, uint16_t y, uint16_t z) { this->addFace(Face(x, y, z)); }
    void addFace (Face f) {
        if (this->f_size + 1 > this->array_f_size) {
            if (this->array_f_size < 128) {
                this->array_f_size += 8;
            } else if (this->array_f_size < 512) {
                this->array_f_size += 64;
            } else {
                this->array_f_size += 128;
            }
            Face* faces = new Face[this->array_f_size];
            uint16_t* colors = new uint16_t[this->array_f_size];
            
            for (int i = 0; i < this->f_size; ++i) { faces[i] = this->faces[i]; }
            faces[this->f_size] = f;
            delete[] this->faces;
            delete[] this->colors;
            this->faces = faces;
            this->colors = colors;
            this->f_size += 1;
        } else {
            this->faces[this->f_size] = f;
            this->f_size += 1;
        }
    }
    
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
        this->obj.position.z = -12;
        this->obj.position.y = -1;
        this->obj.rotation.y = this->obj.rotation.y + 6;
        
        // this->obj.position.z = -6;
        // this->obj.rotation.y = this->obj.rotation.y + 6;
        
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
    
    void drawWire () {
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
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
    
    void drawPolygon () {
        for (int i = 0; i < this->obj.mesh->f_size; ++i) {
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
    WireFrame* wireframes[32];
    uint8_t wsize = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    uint64_t prev_time = 0;
    // uint16_t* buff;
    // uint16_t buffsize = 0;
    
    TFT_eSprite* tft_es_buff;
    
    WindowManager (uint16_t width, uint16_t height) {
        this->width = width;
        this->height = height;
        
        // this->buffsize = height * width;
        // this->buff = new uint16_t[this->buffsize];
        
        this->tft_es_buff = new TFT_eSprite(&M5.Lcd);
        this->tft_es_buff->setColorDepth(16);
        this->tft_es_buff->createSprite(width, height);
    }
    
    void add (WireFrame& wf) {
        this->wireframes[this->wsize] = &wf;
        this->wsize += 1;
    }
    
    void add (int8_t sx, int8_t sy, uint8_t width, uint8_t height, uint16_t bgcolor, uint16_t linecolor, Mesh& mesh, int render_type) {
        this->wireframes[this->wsize] = new WireFrame(sx, sy, width, height, bgcolor, linecolor, mesh, render_type);
        this->wsize += 1;
    }
    
    
    void buffUpdate () {
        // for (int i = 0; i < this->buffsize; ++i) { this->buff[i] = 0; }
        // for (int i = 0; i < this->wsize; ++i) {
        //     WireFrame& w = *this->wireframes[i];
        //     w.draw();
        //     for (int y = 0; y < w.height; ++y) {
        //         const int16_t wy = w.sy+y;
        //         if (wy < 0 || wy >= this->height) { continue; }
        //         for (int x = 0; x < w.width; ++x) {
        //             const int16_t wx = w.sx+x;
        //             if (wx < 0 || wx >= this->width) { continue; }
        //             this->buff[wx + wy * this->width] = w.buff[x + y*w.width];
        //         }
        //     }
        // }
        
        
        
        this->tft_es_buff->fillSprite(0);
        for (int i = 0; i < this->wsize; ++i) {
            WireFrame& w = *this->wireframes[i];
            w.draw();
            this->tft_es_buff->pushImage(w.sx, w.sy, w.width, w.height, w.buff);
        }
    }
    
    void screenDraw () {
        // M5.Lcd.setAddrWindow(0, 0, this->width, this->height);
        // M5.Lcd.pushColors(this->buff, this->buffsize);
        
        
        this->tft_es_buff->pushSprite(0, 0);
    }
    
    
    void fixedDraw () {
        this->buffUpdate();
        uint64_t t = millis();
        while (t - this->prev_time < 33) {
            delay(1);
            t = millis();
        }
        this->prev_time = t;
        screenDraw();
    }
    
    void reDraw () {
        this->buffUpdate();
        this->prev_time = millis();
        screenDraw();
    }
};

WindowManager wm(160, 80);

Mesh mesh1;
Mesh mesh2;

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    
    {
        mesh1.addVertex( 1.0f, -1.0f, -1.0f);
        mesh1.addVertex( 1.0f, -1.0f,  1.0f);
        mesh1.addVertex(-1.0f, -1.0f,  1.0f);
        mesh1.addVertex(-1.0f, -1.0f, -1.0f);
        mesh1.addVertex( 1.0f,  1.0f, -1.0f);
        mesh1.addVertex( 1.0f,  1.0f,  1.0f);
        mesh1.addVertex(-1.0f,  1.0f,  1.0f);
        mesh1.addVertex(-1.0f,  1.0f, -1.0f);
        mesh1.addFace(1, 3, 0);
        mesh1.addFace(7, 5, 4);
        mesh1.addFace(4, 1, 0);
        mesh1.addFace(5, 2, 1);
        mesh1.addFace(2, 7, 3);
        mesh1.addFace(0, 7, 4);
        mesh1.addFace(1, 2, 3);
        mesh1.addFace(7, 6, 5);
        mesh1.addFace(4, 5, 1);
        mesh1.addFace(5, 6, 2);
        mesh1.addFace(2, 6, 7);
        mesh1.addFace(0, 3, 7);
    }
    
    {
        mesh2.addVertex(0.712124, 0.343239, -1.955276);
        mesh2.addVertex(-0.712124, 0.343239, -1.955276);
        mesh2.addVertex(0.995829, 0.263676, -1.207646);
        mesh2.addVertex(-0.995829, 0.263676, -1.207646);
        mesh2.addVertex(1.212771, 1.505506, -2.251083);
        mesh2.addVertex(-1.212771, 1.505506, -2.251083);
        mesh2.addVertex(1.334074, 2.829149, -1.216997);
        mesh2.addVertex(-1.334073, 2.829149, -1.216997);
        mesh2.addVertex(0.0, 0.186174, -1.225716);
        mesh2.addVertex(0.0, 3.727206, -1.271224);
        mesh2.addVertex(1.418549, 1.827501, -1.214256);
        mesh2.addVertex(-1.418549, 1.827501, -1.214256);
        mesh2.addVertex(0.190204, 0.01997, -2.893315);
        mesh2.addVertex(-0.190204, 0.01997, -2.893315);
        mesh2.addVertex(0.0, 0.489414, -2.801201);
        mesh2.addVertex(0.164448, 0.405893, -3.320767);
        mesh2.addVertex(-0.164448, 0.405893, -3.320767);
        mesh2.addVertex(0.0, 0.625957, -3.118722);
        mesh2.addVertex(1.068152, 0.238261, -2.143327);
        mesh2.addVertex(-1.068152, 0.238261, -2.143327);
        mesh2.addVertex(1.097245, 0.045193, -2.84028);
        mesh2.addVertex(-1.097245, 0.045193, -2.84028);
        mesh2.addVertex(0.251435, -0.28231, -2.854148);
        mesh2.addVertex(-0.251435, -0.28231, -2.854148);
        mesh2.addVertex(1.000889, -0.156508, -2.79419);
        mesh2.addVertex(-1.000888, -0.156508, -2.79419);
        mesh2.addVertex(0.916633, -0.102694, -2.301797);
        mesh2.addVertex(-0.916632, -0.102694, -2.301797);
        mesh2.addVertex(0.660387, -0.13562, -2.176559);
        mesh2.addVertex(-0.660387, -0.13562, -2.176559);
        mesh2.addVertex(0.204997, -2.00994, -2.812823);
        mesh2.addVertex(-0.204997, -2.00994, -2.812823);
        mesh2.addVertex(0.910759, -2.00994, -2.923685);
        mesh2.addVertex(-0.910759, -2.00994, -2.923685);
        mesh2.addVertex(0.902642, -2.00994, -2.32242);
        mesh2.addVertex(-0.902642, -2.00994, -2.32242);
        mesh2.addVertex(0.545798, -2.00994, -2.121508);
        mesh2.addVertex(-0.545798, -2.00994, -2.121508);
        mesh2.addVertex(0.0, 1.174393, 1.575172);
        mesh2.addVertex(0.0, 0.799726, 0.77398);
        mesh2.addVertex(1.069965, 0.90622, 0.780193);
        mesh2.addVertex(-1.069964, 0.90622, 0.780193);
        mesh2.addVertex(0.270909, 0.008363, 0.259453);
        mesh2.addVertex(-0.270909, 0.008363, 0.259453);
        mesh2.addVertex(0.844588, 0.191564, 0.712435);
        mesh2.addVertex(-0.844587, 0.191564, 0.712435);
        mesh2.addVertex(0.184621, 0.122916, 0.567233);
        mesh2.addVertex(-0.18462, 0.122916, 0.567233);
        mesh2.addVertex(0.161068, -2.00994, 0.330695);
        mesh2.addVertex(-0.161068, -2.00994, 0.330695);
        mesh2.addVertex(0.892018, -2.00994, 0.869805);
        mesh2.addVertex(-0.892018, -2.00994, 0.869805);
        mesh2.addVertex(0.145175, -2.00994, 0.89084);
        mesh2.addVertex(-0.145175, -2.00994, 0.89084);
        mesh2.addVertex(0.0, 2.932565, 2.840091);
        mesh2.addVertex(0.0, 3.427747, 1.835111);
        mesh2.addVertex(0.0, 1.849925, 3.10356);
        mesh2.addVertex(0.877904, 0.297041, -1.652074);
        mesh2.addVertex(-0.877904, 0.297041, -1.652074);
        mesh2.addVertex(0.989254, 0.329146, -0.681729);
        mesh2.addVertex(-0.989254, 0.329146, -0.681729);
        mesh2.addVertex(0.995563, 0.734413, -1.929289);
        mesh2.addVertex(-0.995562, 0.734413, -1.929289);
        mesh2.addVertex(1.095616, 2.941073, 0.356661);
        mesh2.addVertex(-1.095615, 2.941073, 0.356661);
        mesh2.addVertex(1.247486, 2.600116, -1.999972);
        mesh2.addVertex(-1.247486, 2.600116, -1.999972);
        mesh2.addVertex(1.248877, 2.889464, -0.49111);
        mesh2.addVertex(-1.248877, 2.889464, -0.49111);
        mesh2.addVertex(0.672605, 0.219104, -1.22074);
        mesh2.addVertex(-0.672605, 0.219104, -1.22074);
        mesh2.addVertex(0.0, 0.244707, -0.730095);
        mesh2.addVertex(0.0, 0.217067, -1.710199);
        mesh2.addVertex(0.0, 3.487749, -2.206993);
        mesh2.addVertex(0.0, 3.772162, -0.327733);
        mesh2.addVertex(0.843413, 3.412975, -1.224012);
        mesh2.addVertex(-0.843413, 3.412975, -1.224012);
        mesh2.addVertex(1.29095, 1.64247, -1.788828);
        mesh2.addVertex(-1.290949, 1.64247, -1.788828);
        mesh2.addVertex(1.35652, 1.921824, -0.65598);
        mesh2.addVertex(-1.356519, 1.921824, -0.65598);
        mesh2.addVertex(1.251067, 0.791004, -1.194947);
        mesh2.addVertex(-1.251067, 0.791004, -1.194947);
        mesh2.addVertex(0.0, 2.933173, -3.355259);
        mesh2.addVertex(1.112218, 2.383843, -2.729163);
        mesh2.addVertex(-1.112218, 2.383843, -2.729163);
        mesh2.addVertex(0.231035, 0.361439, -2.316163);
        mesh2.addVertex(-0.231034, 0.361439, -2.316163);
        mesh2.addVertex(0.0, 0.359988, -2.323414);
        mesh2.addVertex(0.14517, 0.987087, -3.534714);
        mesh2.addVertex(-0.14517, 0.987087, -3.534714);
        mesh2.addVertex(0.0, 1.836306, -3.760285);
        mesh2.addVertex(0.0, 0.917459, -3.518806);
        mesh2.addVertex(1.144795, 0.66829, -2.190932);
        mesh2.addVertex(-1.144795, 0.66829, -2.190932);
        mesh2.addVertex(0.660897, -0.050216, -3.067415);
        mesh2.addVertex(-0.660897, -0.050216, -3.067415);
        mesh2.addVertex(1.182624, 0.859658, -3.032264);
        mesh2.addVertex(-1.182624, 0.859658, -3.032264);
        mesh2.addVertex(1.185273, 0.339247, -2.918175);
        mesh2.addVertex(-1.185272, 0.339247, -2.918175);
        mesh2.addVertex(0.631473, -0.284753, -2.982881);
        mesh2.addVertex(-0.631473, -0.284753, -2.982881);
        mesh2.addVertex(0.37229, -0.107534, -2.272141);
        mesh2.addVertex(-0.37229, -0.107534, -2.272141);
        mesh2.addVertex(0.851708, -1.607755, -2.850754);
        mesh2.addVertex(-0.851708, -1.607755, -2.850754);
        mesh2.addVertex(0.546894, -2.00994, -3.019514);
        mesh2.addVertex(-0.546894, -2.00994, -3.019514);
        mesh2.addVertex(0.296683, -1.61319, -2.77205);
        mesh2.addVertex(-0.296683, -1.61319, -2.77205);
        mesh2.addVertex(0.850109, -1.61437, -2.420256);
        mesh2.addVertex(-0.850109, -1.61437, -2.420256);
        mesh2.addVertex(0.534877, -1.609278, -2.26268);
        mesh2.addVertex(-0.534877, -1.609278, -2.26268);
        mesh2.addVertex(0.215571, -2.00994, -2.273956);
        mesh2.addVertex(-0.21557, -2.00994, -2.273956);
        mesh2.addVertex(0.347413, 1.232219, 1.533897);
        mesh2.addVertex(-0.347413, 1.232219, 1.533897);
        mesh2.addVertex(0.631074, 1.219622, 1.317006);
        mesh2.addVertex(-0.631074, 1.219622, 1.317006);
        mesh2.addVertex(0.0, 1.127572, 1.366131);
        mesh2.addVertex(1.345901, 2.166615, 0.50938);
        mesh2.addVertex(-1.3459, 2.166615, 0.50938);
        mesh2.addVertex(0.855045, 0.579804, -0.082523);
        mesh2.addVertex(-0.855045, 0.579804, -0.082523);
        mesh2.addVertex(1.144174, 0.653919, 0.162359);
        mesh2.addVertex(-1.144173, 0.653919, 0.162359);
        mesh2.addVertex(0.691814, 0.05254, 0.070559);
        mesh2.addVertex(-0.691814, 0.05254, 0.070559);
        mesh2.addVertex(0.264487, 0.229818, 0.128576);
        mesh2.addVertex(-0.264487, 0.229818, 0.128576);
        mesh2.addVertex(0.925502, 0.560337, 0.697674);
        mesh2.addVertex(-0.925502, 0.560337, 0.697674);
        mesh2.addVertex(0.909784, 0.166099, 0.227564);
        mesh2.addVertex(-0.909784, 0.166099, 0.227564);
        mesh2.addVertex(0.176062, 0.439541, 0.68627);
        mesh2.addVertex(-0.176062, 0.439541, 0.68627);
        mesh2.addVertex(0.555513, 0.193176, 0.927516);
        mesh2.addVertex(-0.555513, 0.193176, 0.927516);
        mesh2.addVertex(0.551165, -2.00994, 0.212188);
        mesh2.addVertex(-0.551165, -2.00994, 0.212188);
        mesh2.addVertex(0.273969, -1.129256, 0.415094);
        mesh2.addVertex(-0.273969, -1.129256, 0.415094);
        mesh2.addVertex(0.815743, -1.12111, 0.75436);
        mesh2.addVertex(-0.815742, -1.12111, 0.75436);
        mesh2.addVertex(0.872811, -2.00994, 0.324614);
        mesh2.addVertex(-0.872811, -2.00994, 0.324614);
        mesh2.addVertex(0.209956, -1.12111, 0.751308);
        mesh2.addVertex(-0.209956, -1.12111, 0.751308);
        mesh2.addVertex(0.5, -2.00994, 1.043779);
        mesh2.addVertex(-0.5, -2.00994, 1.043779);
        mesh2.addVertex(0.494629, 2.941544, 2.908058);
        mesh2.addVertex(-0.494628, 2.941544, 2.908058);
        mesh2.addVertex(0.0, 3.435638, 1.278223);
        mesh2.addVertex(0.578098, 3.427358, 1.841468);
        mesh2.addVertex(-0.578097, 3.427358, 1.841468);
        mesh2.addVertex(0.0, 3.387841, 2.529556);
        mesh2.addVertex(0.0, 2.476262, 3.059709);
        mesh2.addVertex(0.454832, 1.706591, 2.936781);
        mesh2.addVertex(-0.454832, 1.706591, 2.936781);
        mesh2.addVertex(0.0, 1.099739, 1.805865);
        mesh2.addVertex(0.0, 1.050898, 2.273259);
        mesh2.addVertex(0.518014, 0.249286, -1.680986);
        mesh2.addVertex(-0.518013, 0.249286, -1.680986);
        mesh2.addVertex(0.655495, 0.290537, -0.706458);
        mesh2.addVertex(-0.655495, 0.290537, -0.706458);
        mesh2.addVertex(0.74723, 3.136852, -2.112849);
        mesh2.addVertex(-0.74723, 3.136852, -2.112849);
        mesh2.addVertex(0.831393, 3.476235, -0.38437);
        mesh2.addVertex(-0.831393, 3.476235, -0.38437);
        mesh2.addVertex(1.121891, 0.722922, -1.63841);
        mesh2.addVertex(-1.121891, 0.722922, -1.63841);
        mesh2.addVertex(1.253155, 0.928584, -0.709996);
        mesh2.addVertex(-1.253154, 0.928584, -0.709996);
        mesh2.addVertex(0.614869, 2.771739, -3.14813);
        mesh2.addVertex(-0.614869, 2.771739, -3.14813);
        mesh2.addVertex(0.716122, 0.778791, -3.461078);
        mesh2.addVertex(-0.716122, 0.778791, -3.461078);
        mesh2.addVertex(0.96828, 1.363974, -3.255801);
        mesh2.addVertex(-0.96828, 1.363974, -3.255801);
        mesh2.addVertex(0.20705, 1.792587, -3.506803);
        mesh2.addVertex(-0.20705, 1.792587, -3.506803);
        mesh2.addVertex(0.647175, 0.276252, -3.323141);
        mesh2.addVertex(-0.647175, 0.276252, -3.323141);
        mesh2.addVertex(1.240355, 1.345373, -2.373479);
        mesh2.addVertex(-1.240355, 1.345373, -2.373479);
        mesh2.addVertex(0.551869, -1.615068, -2.965128);
        mesh2.addVertex(-0.551868, -1.615068, -2.965128);
        mesh2.addVertex(0.300653, -1.609263, -2.404913);
        mesh2.addVertex(-0.300653, -1.609263, -2.404913);
        mesh2.addVertex(0.964932, 2.932086, 0.956221);
        mesh2.addVertex(-0.964932, 2.932086, 0.956221);
        mesh2.addVertex(0.229687, 1.135904, 1.363954);
        mesh2.addVertex(-0.229686, 1.135904, 1.363954);
        mesh2.addVertex(1.135738, 1.155481, -0.026897);
        mesh2.addVertex(-1.135738, 1.155481, -0.026897);
        mesh2.addVertex(1.088992, 2.353637, 1.043545);
        mesh2.addVertex(-1.088992, 2.353637, 1.043545);
        mesh2.addVertex(0.728775, 0.239847, 0.010336);
        mesh2.addVertex(-0.728775, 0.239847, 0.010336);
        mesh2.addVertex(0.999041, 0.390784, 0.205899);
        mesh2.addVertex(-0.999041, 0.390784, 0.205899);
        mesh2.addVertex(0.5869, 0.521603, 1.012711);
        mesh2.addVertex(-0.586899, 0.521603, 1.012711);
        mesh2.addVertex(0.571853, -1.13102, 0.28497);
        mesh2.addVertex(-0.571853, -1.13102, 0.28497);
        mesh2.addVertex(0.794631, -1.13102, 0.38404);
        mesh2.addVertex(-0.794631, -1.13102, 0.38404);
        mesh2.addVertex(0.48999, -1.13102, 0.900447);
        mesh2.addVertex(-0.48999, -1.13102, 0.900447);
        mesh2.addVertex(0.486515, 3.333708, 1.315016);
        mesh2.addVertex(-0.486515, 3.333708, 1.315016);
        mesh2.addVertex(0.542219, 3.474965, 2.507201);
        mesh2.addVertex(-0.542219, 3.474965, 2.507201);
        mesh2.addVertex(0.466965, 2.589664, 3.041276);
        mesh2.addVertex(-0.466965, 2.589664, 3.041276);
        mesh2.addVertex(0.440591, 1.028584, 1.851222);
        mesh2.addVertex(-0.440591, 1.028584, 1.851222);
        mesh2.addVertex(0.387348, 1.031212, 2.234724);
        mesh2.addVertex(-0.387348, 1.031212, 2.234724);
        mesh2.addVertex(1.239835, 1.527501, 0.919053);
        mesh2.addVertex(-1.239834, 1.527501, 0.919053);
        mesh2.addVertex(1.304781, 1.207592, 0.239758);
        mesh2.addVertex(-1.304781, 1.207592, 0.239758);
        mesh2.addVertex(0.664619, 0.800481, 1.209078);
        mesh2.addVertex(-0.664618, 0.800481, 1.209078);
        mesh2.addVertex(0.157761, 0.766436, 0.811604);
        mesh2.addVertex(-0.15776, 0.766436, 0.811604);
        mesh2.addVertex(0.0, 3.66741, 0.309168);
        mesh2.addVertex(0.7144, 3.446259, 0.357958);
        mesh2.addVertex(-0.7144, 3.446259, 0.357958);
        mesh2.addVertex(0.0, 3.58107, 0.840363);
        mesh2.addVertex(0.615492, 3.403351, 0.851765);
        mesh2.addVertex(-0.615492, 3.403351, 0.851765);
        mesh2.addVertex(0.259645, 0.422033, -0.001566);
        mesh2.addVertex(-0.259645, 0.422033, -0.001566);
        mesh2.addVertex(0.0, 0.312022, -0.180404);
        mesh2.addVertex(1.293545, 1.92276, 0.098737);
        mesh2.addVertex(-1.293544, 1.92276, 0.098737);
        mesh2.addVertex(0.0, 3.507984, 1.065972);
        mesh2.addVertex(0.518685, 3.318544, 1.078709);
        mesh2.addVertex(-0.518685, 3.318544, 1.078709);
        mesh2.addVertex(0.627094, 1.417171, 1.442486);
        mesh2.addVertex(-0.627094, 1.417171, 1.442486);
        mesh2.addVertex(0.948655, 2.395952, 1.238207);
        mesh2.addVertex(-0.948655, 2.395952, 1.238207);
        mesh2.addVertex(0.922582, 2.901012, 1.195493);
        mesh2.addVertex(-0.922582, 2.901012, 1.195493);
        mesh2.addVertex(0.0, 0.923234, -3.84126);
        mesh2.addVertex(0.096467, 0.038973, -3.584176);
        mesh2.addVertex(-0.096467, 0.038973, -3.584176);
        mesh2.addVertex(0.0, 0.043228, -3.522604);
        mesh2.addVertex(0.0, 0.043228, -3.745495);
        mesh2.addVertex(0.074719, -0.608265, -3.55001);
        mesh2.addVertex(-0.074719, -0.608265, -3.55001);
        mesh2.addVertex(0.0, -0.60401, -3.62653);
        mesh2.addVertex(0.0, -0.60401, -3.486953);
        mesh2.addVertex(0.057031, -1.294491, -3.371183);
        mesh2.addVertex(-0.057031, -1.294491, -3.371183);
        mesh2.addVertex(0.0, -1.290236, -3.425111);
        mesh2.addVertex(0.0, -1.290236, -3.314561);
        mesh2.addVertex(0.793838, 3.300557, 1.971249);
        mesh2.addVertex(-0.793838, 3.300557, 1.971249);
        mesh2.addVertex(0.923073, 2.994228, 2.183931);
        mesh2.addVertex(-0.923072, 2.994228, 2.183931);
        mesh2.addVertex(0.841346, 2.816506, 2.671191);
        mesh2.addVertex(-0.841346, 2.816506, 2.671191);
        mesh2.addVertex(0.919967, 2.906421, 1.36882);
        mesh2.addVertex(-0.919967, 2.906421, 1.36882);
        mesh2.addVertex(0.772379, 2.506588, 2.585693);
        mesh2.addVertex(-0.772379, 2.506588, 2.585693);
        mesh2.addVertex(0.783211, 2.02597, 2.193675);
        mesh2.addVertex(-0.783211, 2.02597, 2.193675);
        mesh2.addVertex(0.0, 2.187331, 3.129442);
        mesh2.addVertex(0.822428, 2.40799, 1.88782);
        mesh2.addVertex(-0.822428, 2.40799, 1.88782);
        mesh2.addVertex(0.980347, 2.347098, 1.431852);
        mesh2.addVertex(-0.980346, 2.347098, 1.431852);
        mesh2.addVertex(0.684757, 1.254725, 1.728601);
        mesh2.addVertex(-0.684757, 1.254725, 1.728601);
        mesh2.addVertex(0.594825, 1.656858, 2.32419);
        mesh2.addVertex(-0.594824, 1.656858, 2.32419);
        mesh2.addVertex(0.0, 1.280301, 3.236645);
        mesh2.addVertex(0.409625, 1.251048, 2.955123);
        mesh2.addVertex(-0.409624, 1.251048, 2.955123);
        mesh2.addVertex(0.0, 0.232732, 3.198842);
        mesh2.addVertex(0.254545, 0.169726, 3.004738);
        mesh2.addVertex(-0.254545, 0.169726, 3.004738);
        mesh2.addVertex(0.483905, 1.43686, 2.640788);
        mesh2.addVertex(-0.483905, 1.43686, 2.640788);
        mesh2.addVertex(0.427262, 1.088948, 2.66592);
        mesh2.addVertex(-0.427262, 1.088948, 2.66592);
        mesh2.addVertex(0.260698, 0.091254, 2.764201);
        mesh2.addVertex(-0.260697, 0.091254, 2.764201);
        mesh2.addVertex(0.0, 1.184703, 2.464109);
        mesh2.addVertex(0.0, 0.092903, 2.652447);
        mesh2.addVertex(0.159055, 0.748817, 2.604924);
        mesh2.addVertex(-0.159054, 0.748817, 2.604924);
        mesh2.addVertex(0.0, 0.573897, 2.568213);
        mesh2.addVertex(0.243038, 1.098148, 2.58079);
        mesh2.addVertex(-0.243038, 1.098148, 2.58079);
        mesh2.addVertex(0.0, 0.969701, 2.365579);
        mesh2.addVertex(0.0, 1.285505, 2.25781);
        mesh2.addVertex(0.0, 1.339451, 2.444381);
        mesh2.addVertex(0.724738, 1.584358, 2.076583);
        mesh2.addVertex(-0.724737, 1.584358, 2.076583);
        mesh2.addVertex(0.653917, 1.275397, 2.106506);
        mesh2.addVertex(-0.653917, 1.275397, 2.106506);
        mesh2.addVertex(0.953592, 1.702016, 1.197974);
        mesh2.addVertex(-0.953592, 1.702016, 1.197974);
        mesh2.addVertex(0.748768, 1.725008, 1.363499);
        mesh2.addVertex(-0.748768, 1.725008, 1.363499);
        mesh2.addVertex(0.777751, 1.541251, 1.549228);
        mesh2.addVertex(-0.77775, 1.541251, 1.549228);
        mesh2.addVertex(0.0, -0.603584, 3.42382);
        mesh2.addVertex(0.181946, -0.690372, 3.343028);
        mesh2.addVertex(-0.181946, -0.690372, 3.343028);
        mesh2.addVertex(0.188098, -0.845165, 3.138151);
        mesh2.addVertex(-0.188098, -0.845165, 3.138151);
        mesh2.addVertex(0.0, -0.804155, 3.096275);
        mesh2.addVertex(0.144291, -1.355688, 3.823553);
        mesh2.addVertex(-0.14429, -1.355688, 3.823553);
        mesh2.addVertex(0.0, -1.283736, 3.868135);
        mesh2.addVertex(0.150443, -1.530599, 3.747084);
        mesh2.addVertex(-0.150443, -1.530599, 3.747084);
        mesh2.addVertex(0.0, -1.500103, 3.764431);
        mesh2.addVertex(0.159379, -1.634308, 3.919721);
        mesh2.addVertex(-0.159379, -1.634308, 3.919721);
        mesh2.addVertex(0.0, -1.553479, 3.992316);
        mesh2.addVertex(0.179773, -1.678559, 3.774787);
        mesh2.addVertex(-0.179772, -1.678559, 3.774787);
        mesh2.addVertex(0.0, -1.671046, 3.782751);
        mesh2.addVertex(1.398977, 3.075111, 1.630607);
        mesh2.addVertex(-1.398977, 3.075111, 1.630607);
        mesh2.addVertex(1.024983, 3.230716, 1.91905);
        mesh2.addVertex(-1.024983, 3.230716, 1.91905);
        mesh2.addVertex(1.149545, 3.060814, 2.002972);
        mesh2.addVertex(-1.149545, 3.060814, 2.002972);
        mesh2.addVertex(0.497734, 1.886112, 2.937304);
        mesh2.addVertex(-0.497733, 1.886112, 2.937304);
        mesh2.addVertex(0.643978, 2.198878, 3.044978);
        mesh2.addVertex(-0.643977, 2.198878, 3.044978);
        mesh2.addVertex(0.809641, 2.163587, 2.573313);
        mesh2.addVertex(-0.809641, 2.163587, 2.573313);
        mesh2.addVertex(0.581339, 1.849421, 2.604416);
        mesh2.addVertex(-0.581338, 1.849421, 2.604416);
        mesh2.addVertex(0.645346, 2.038764, 2.648478);
        mesh2.addVertex(-0.645346, 2.038764, 2.648478);
        mesh2.addVertex(0.632916, 2.013321, 2.869752);
        mesh2.addVertex(-0.632916, 2.013321, 2.869752);
        mesh2.addVertex(0.541719, 1.921597, 2.860386);
        mesh2.addVertex(-0.541719, 1.921597, 2.860386);
        mesh2.addVertex(0.570961, 1.958197, 2.687148);
        mesh2.addVertex(-0.57096, 1.958197, 2.687148);
        mesh2.addVertex(0.565215, 2.012091, 2.746957);
        mesh2.addVertex(-0.565214, 2.012091, 2.746957);
        mesh2.addVertex(0.351174, 1.320581, 2.581601);
        mesh2.addVertex(-0.351174, 1.320581, 2.581601);
        mesh2.addVertex(0.0, -1.703652, 3.974355);
        mesh2.addVertex(0.0, -1.653813, 3.925617);
        mesh2.addVertex(0.292585, -0.902294, 0.412668);
        mesh2.addVertex(-0.292585, -0.902294, 0.412668);
        mesh2.addVertex(0.738537, -0.884757, 0.724892);
        mesh2.addVertex(-0.738537, -0.884757, 0.724892);
        mesh2.addVertex(0.285478, -0.893702, 0.733306);
        mesh2.addVertex(-0.285477, -0.893702, 0.733306);
        mesh2.addVertex(0.58085, -0.908148, 0.267129);
        mesh2.addVertex(-0.58085, -0.908148, 0.267129);
        mesh2.addVertex(0.780689, -0.899631, 0.382191);
        mesh2.addVertex(-0.780689, -0.899631, 0.382191);
        mesh2.addVertex(0.494905, -0.8976, 0.840375);
        mesh2.addVertex(-0.494904, -0.8976, 0.840375);
        mesh2.addVertex(0.831403, -1.336095, -2.815172);
        mesh2.addVertex(-0.831403, -1.336095, -2.815172);
        mesh2.addVertex(0.313411, -1.354315, -2.731287);
        mesh2.addVertex(-0.313411, -1.354315, -2.731287);
        mesh2.addVertex(0.824901, -1.318808, -2.470135);
        mesh2.addVertex(-0.824901, -1.318808, -2.470135);
        mesh2.addVertex(0.580175, -1.332931, -2.363902);
        mesh2.addVertex(-0.580175, -1.332931, -2.363902);
        mesh2.addVertex(0.569524, -1.337606, -2.917296);
        mesh2.addVertex(-0.569524, -1.337606, -2.917296);
        mesh2.addVertex(0.325062, -1.329548, -2.469038);
        mesh2.addVertex(-0.325062, -1.329548, -2.469038);
        mesh2.addVertex(1.298641, 3.132874, 1.738933);
        mesh2.addVertex(-1.29864, 3.132874, 1.738933);
        mesh2.addVertex(1.3374, 3.09547, 1.770139);
        mesh2.addVertex(-1.3374, 3.09547, 1.770139);
        mesh2.addVertex(1.128002, 3.174523, 1.859516);
        mesh2.addVertex(-1.128002, 3.174523, 1.859516);
        mesh2.addVertex(1.231236, 3.06408, 1.89145);
        mesh2.addVertex(-1.231236, 3.06408, 1.89145);
        mesh2.addVertex(1.051525, 2.826521, 1.833987);
        mesh2.addVertex(-1.051525, 2.826521, 1.833987);
        mesh2.addVertex(1.140819, 2.957812, 1.791227);
        mesh2.addVertex(-1.140819, 2.957812, 1.791227);
        mesh2.addVertex(1.293728, 3.065808, 1.698867);
        mesh2.addVertex(-1.293728, 3.065808, 1.698867);
        mesh2.addVertex(1.163861, 1.875032, 1.444103);
        mesh2.addVertex(-1.163861, 1.875032, 1.444103);
        mesh2.addVertex(0.857274, 2.610662, 1.98695);
        mesh2.addVertex(-0.857273, 2.610662, 1.98695);
        mesh2.addVertex(1.735361, 2.376928, 0.986597);
        mesh2.addVertex(-1.735361, 2.376928, 0.986597);
        mesh2.addVertex(1.530894, 2.236285, 1.177014);
        mesh2.addVertex(-1.530893, 2.236285, 1.177014);
        mesh2.addVertex(1.369421, 2.960101, 1.637927);
        mesh2.addVertex(-1.369421, 2.960101, 1.637927);
        mesh2.addVertex(1.47883, 3.015835, 1.536478);
        mesh2.addVertex(-1.47883, 3.015835, 1.536478);
        mesh2.addVertex(1.404114, 2.640053, 1.430707);
        mesh2.addVertex(-1.404114, 2.640053, 1.430707);
        mesh2.addVertex(1.59969, 2.726244, 1.34324);
        mesh2.addVertex(-1.59969, 2.726244, 1.34324);
        mesh2.addVertex(1.075485, 2.406625, 1.733168);
        mesh2.addVertex(-1.075485, 2.406625, 1.733168);
        mesh2.addVertex(1.0181, 1.675889, 1.613462);
        mesh2.addVertex(-1.018099, 1.675889, 1.613462);
        mesh2.addVertex(1.317696, 2.038561, 1.307079);
        mesh2.addVertex(-1.317696, 2.038561, 1.307079);
        mesh2.addVertex(1.234846, 2.853651, 1.707559);
        mesh2.addVertex(-1.234846, 2.853651, 1.707559);
        mesh2.addVertex(1.232531, 2.53436, 1.554136);
        mesh2.addVertex(-1.232531, 2.53436, 1.554136);
        mesh2.addVertex(0.287109, -1.729859, 0.421716);
        mesh2.addVertex(-0.287108, -1.729859, 0.421716);
        mesh2.addVertex(0.714472, -1.729859, 0.677072);
        mesh2.addVertex(-0.714472, -1.729859, 0.677072);
        mesh2.addVertex(0.298364, -1.729859, 0.695593);
        mesh2.addVertex(-0.298364, -1.729859, 0.695593);
        mesh2.addVertex(0.527955, -1.732981, 0.30153);
        mesh2.addVertex(-0.527955, -1.732981, 0.30153);
        mesh2.addVertex(0.700879, -1.732981, 0.36447);
        mesh2.addVertex(-0.700879, -1.732981, 0.36447);
        mesh2.addVertex(0.500433, -1.732981, 0.80619);
        mesh2.addVertex(-0.500433, -1.732981, 0.80619);
        mesh2.addVertex(0.881222, -1.814743, -2.872721);
        mesh2.addVertex(-0.881222, -1.814743, -2.872721);
        mesh2.addVertex(0.239461, -1.817653, -2.789424);
        mesh2.addVertex(-0.23946, -1.817653, -2.789424);
        mesh2.addVertex(0.878865, -1.81805, -2.3871);
        mesh2.addVertex(-0.878864, -1.81805, -2.3871);
        mesh2.addVertex(0.554665, -1.815504, -2.213596);
        mesh2.addVertex(-0.554664, -1.815504, -2.213596);
        mesh2.addVertex(0.554913, -1.818399, -3.00054);
        mesh2.addVertex(-0.554913, -1.818399, -3.00054);
        mesh2.addVertex(0.246326, -1.815497, -2.358441);
        mesh2.addVertex(-0.246326, -1.815497, -2.358441);
        mesh2.addVertex(0.159482, -1.833082, 0.329568);
        mesh2.addVertex(-0.159482, -1.833082, 0.329568);
        mesh2.addVertex(0.883991, -1.833082, 0.858866);
        mesh2.addVertex(-0.883991, -1.833082, 0.858866);
        mesh2.addVertex(0.14274, -1.833082, 0.889953);
        mesh2.addVertex(-0.142739, -1.833082, 0.889953);
        mesh2.addVertex(0.550381, -1.834643, 0.201434);
        mesh2.addVertex(-0.55038, -1.834643, 0.201434);
        mesh2.addVertex(0.866262, -1.834643, 0.325799);
        mesh2.addVertex(-0.866262, -1.834643, 0.325799);
        mesh2.addVertex(0.483831, -1.834643, 1.031705);
        mesh2.addVertex(-0.48383, -1.834643, 1.031705);
        mesh2.addVertex(0.892384, -1.881585, -2.88981);
        mesh2.addVertex(-0.892384, -1.881585, -2.88981);
        mesh2.addVertex(0.220468, -1.88304, -2.792981);
        mesh2.addVertex(-0.220467, -1.88304, -2.792981);
        mesh2.addVertex(0.898962, -1.883238, -2.355534);
        mesh2.addVertex(-0.898962, -1.883238, -2.355534);
        mesh2.addVertex(0.550931, -1.881965, -2.123231);
        mesh2.addVertex(-0.550931, -1.881965, -2.123231);
        mesh2.addVertex(0.553544, -1.883413, -3.011523);
        mesh2.addVertex(-0.553543, -1.883413, -3.011523);
        mesh2.addVertex(0.235744, -1.881962, -2.308993);
        mesh2.addVertex(-0.235743, -1.881962, -2.308993);
        mesh2.addFace(258, 261, 260);
        mesh2.addFace(260, 261, 259);
        mesh2.addFace(97, 177, 179);
        mesh2.addFace(180, 178, 98);
        mesh2.addFace(4, 185, 84);
        mesh2.addFace(85, 186, 5);
        mesh2.addFace(63, 122, 238);
        mesh2.addFace(239, 123, 64);
        mesh2.addFace(297, 302, 299);
        mesh2.addFace(299, 302, 298);
        mesh2.addFace(162, 297, 299);
        mesh2.addFace(299, 298, 162);
        mesh2.addFace(155, 213, 262);
        mesh2.addFace(263, 214, 156);
        mesh2.addFace(152, 266, 213);
        mesh2.addFace(214, 267, 153);
        mesh2.addFace(159, 339, 56);
        mesh2.addFace(56, 340, 160);
        mesh2.addFace(349, 355, 347);
        mesh2.addFace(348, 356, 350);
        mesh2.addFace(353, 355, 351);
        mesh2.addFace(352, 356, 354);
        mesh2.addFace(289, 304, 357);
        mesh2.addFace(358, 304, 290);
        mesh2.addFace(349, 351, 355);
        mesh2.addFace(356, 352, 350);
        mesh2.addFace(347, 355, 353);
        mesh2.addFace(354, 356, 348);
        mesh2.addFace(327, 359, 360);
        mesh2.addFace(360, 359, 328);
        mesh2.addFace(327, 329, 359);
        mesh2.addFace(359, 329, 328);
        mesh2.addFace(387, 333, 385);
        mesh2.addFace(386, 334, 388);
        mesh2.addFace(333, 387, 397);
        mesh2.addFace(398, 388, 334);
        mesh2.addFace(401, 264, 266);
        mesh2.addFace(267, 265, 402);
        mesh2.addFace(393, 421, 395);
        mesh2.addFace(396, 422, 394);
        mesh2.addFace(48, 140, 146);
        mesh2.addFace(147, 141, 49);
        mesh2.addFace(50, 150, 52);
        mesh2.addFace(53, 151, 51);
        mesh2.addFace(34, 36, 115);
        mesh2.addFace(116, 37, 35);
        mesh2.addFace(30, 107, 32);
        mesh2.addFace(33, 108, 31);
        mesh2.addFace(333, 397, 385);
        mesh2.addFace(386, 398, 334);
        mesh2.addFace(262, 401, 268);
        mesh2.addFace(269, 402, 263);
        mesh2.addFace(276, 402, 278);
        mesh2.addFace(402, 269, 278);
        mesh2.addFace(268, 401, 277);
        mesh2.addFace(401, 275, 277);
        mesh2.addFace(394, 336, 263);
        mesh2.addFace(394, 263, 402);
        mesh2.addFace(262, 335, 393);
        mesh2.addFace(262, 393, 401);
        mesh2.addFace(396, 390, 336);
        mesh2.addFace(396, 336, 394);
        mesh2.addFace(335, 389, 395);
        mesh2.addFace(335, 395, 393);
        mesh2.addFace(396, 398, 386);
        mesh2.addFace(396, 386, 390);
        mesh2.addFace(385, 397, 395);
        mesh2.addFace(385, 395, 389);
        mesh2.addFace(35, 33, 31);
        mesh2.addFace(35, 31, 116);
        mesh2.addFace(30, 32, 34);
        mesh2.addFace(30, 34, 115);
        mesh2.addFace(468, 466, 35);
        mesh2.addFace(468, 35, 37);
        mesh2.addFace(34, 465, 467);
        mesh2.addFace(34, 467, 36);
        mesh2.addFace(468, 444, 466);
        mesh2.addFace(444, 442, 466);
        mesh2.addFace(441, 443, 465);
        mesh2.addFace(443, 467, 465);
        mesh2.addFace(444, 114, 112);
        mesh2.addFace(444, 112, 442);
        mesh2.addFace(111, 113, 443);
        mesh2.addFace(111, 443, 441);
        mesh2.addFace(380, 378, 112);
        mesh2.addFace(380, 112, 114);
        mesh2.addFace(111, 377, 379);
        mesh2.addFace(111, 379, 113);
        mesh2.addFace(380, 29, 378);
        mesh2.addFace(29, 27, 378);
        mesh2.addFace(26, 28, 377);
        mesh2.addFace(28, 379, 377);
        mesh2.addFace(27, 29, 19);
        mesh2.addFace(29, 1, 19);
        mesh2.addFace(0, 28, 18);
        mesh2.addFace(28, 26, 18);
        mesh2.addFace(94, 19, 62);
        mesh2.addFace(19, 1, 62);
        mesh2.addFace(0, 18, 61);
        mesh2.addFace(18, 93, 61);
        mesh2.addFace(94, 62, 186);
        mesh2.addFace(62, 5, 186);
        mesh2.addFace(4, 61, 185);
        mesh2.addFace(61, 93, 185);
        mesh2.addFace(51, 147, 49);
        mesh2.addFace(51, 49, 53);
        mesh2.addFace(48, 146, 50);
        mesh2.addFace(48, 50, 52);
        mesh2.addFace(458, 456, 141);
        mesh2.addFace(458, 141, 147);
        mesh2.addFace(140, 455, 457);
        mesh2.addFace(140, 457, 146);
        mesh2.addFace(458, 434, 456);
        mesh2.addFace(434, 432, 456);
        mesh2.addFace(431, 433, 455);
        mesh2.addFace(433, 457, 455);
        mesh2.addFace(434, 208, 206);
        mesh2.addFace(434, 206, 432);
        mesh2.addFace(205, 207, 433);
        mesh2.addFace(205, 433, 431);
        mesh2.addFace(370, 368, 206);
        mesh2.addFace(370, 206, 208);
        mesh2.addFace(205, 367, 369);
        mesh2.addFace(205, 369, 207);
        mesh2.addFace(370, 135, 129);
        mesh2.addFace(370, 129, 368);
        mesh2.addFace(128, 134, 369);
        mesh2.addFace(128, 369, 367);
        mesh2.addFace(202, 200, 135);
        mesh2.addFace(200, 129, 135);
        mesh2.addFace(128, 199, 134);
        mesh2.addFace(199, 201, 134);
        mesh2.addFace(202, 127, 125);
        mesh2.addFace(202, 125, 200);
        mesh2.addFace(124, 126, 201);
        mesh2.addFace(124, 201, 199);
        mesh2.addFace(224, 196, 127);
        mesh2.addFace(196, 125, 127);
        mesh2.addFace(124, 195, 126);
        mesh2.addFace(195, 223, 126);
        mesh2.addFace(196, 224, 239);
        mesh2.addFace(224, 123, 239);
        mesh2.addFace(122, 223, 238);
        mesh2.addFace(223, 195, 238);
        mesh2.addFace(442, 438, 462);
        mesh2.addFace(442, 462, 466);
        mesh2.addFace(461, 437, 441);
        mesh2.addFace(461, 441, 465);
        mesh2.addFace(466, 462, 35);
        mesh2.addFace(462, 33, 35);
        mesh2.addFace(32, 461, 34);
        mesh2.addFace(461, 465, 34);
        mesh2.addFace(448, 444, 472);
        mesh2.addFace(444, 468, 472);
        mesh2.addFace(467, 443, 471);
        mesh2.addFace(443, 447, 471);
        mesh2.addFace(472, 468, 37);
        mesh2.addFace(472, 37, 116);
        mesh2.addFace(36, 467, 471);
        mesh2.addFace(36, 471, 115);
        mesh2.addFace(116, 31, 472);
        mesh2.addFace(31, 464, 472);
        mesh2.addFace(463, 30, 471);
        mesh2.addFace(30, 115, 471);
        mesh2.addFace(472, 464, 448);
        mesh2.addFace(464, 440, 448);
        mesh2.addFace(439, 463, 447);
        mesh2.addFace(463, 471, 447);
        mesh2.addFace(446, 440, 470);
        mesh2.addFace(440, 464, 470);
        mesh2.addFace(463, 439, 469);
        mesh2.addFace(439, 445, 469);
        mesh2.addFace(470, 464, 108);
        mesh2.addFace(464, 31, 108);
        mesh2.addFace(30, 463, 107);
        mesh2.addFace(463, 469, 107);
        mesh2.addFace(108, 33, 470);
        mesh2.addFace(33, 462, 470);
        mesh2.addFace(461, 32, 469);
        mesh2.addFace(32, 107, 469);
        mesh2.addFace(470, 462, 446);
        mesh2.addFace(462, 438, 446);
        mesh2.addFace(437, 461, 445);
        mesh2.addFace(461, 469, 445);
        mesh2.addFace(426, 430, 450);
        mesh2.addFace(430, 454, 450);
        mesh2.addFace(453, 429, 449);
        mesh2.addFace(429, 425, 449);
        mesh2.addFace(450, 454, 49);
        mesh2.addFace(454, 53, 49);
        mesh2.addFace(52, 453, 48);
        mesh2.addFace(453, 449, 48);
        mesh2.addFace(436, 428, 452);
        mesh2.addFace(436, 452, 460);
        mesh2.addFace(451, 427, 435);
        mesh2.addFace(451, 435, 459);
        mesh2.addFace(460, 452, 151);
        mesh2.addFace(452, 51, 151);
        mesh2.addFace(50, 451, 150);
        mesh2.addFace(451, 459, 150);
        mesh2.addFace(151, 53, 460);
        mesh2.addFace(53, 454, 460);
        mesh2.addFace(453, 52, 459);
        mesh2.addFace(52, 150, 459);
        mesh2.addFace(460, 454, 436);
        mesh2.addFace(454, 430, 436);
        mesh2.addFace(429, 453, 435);
        mesh2.addFace(453, 459, 435);
        mesh2.addFace(147, 51, 452);
        mesh2.addFace(147, 452, 458);
        mesh2.addFace(451, 50, 146);
        mesh2.addFace(451, 146, 457);
        mesh2.addFace(458, 452, 428);
        mesh2.addFace(458, 428, 434);
        mesh2.addFace(427, 451, 457);
        mesh2.addFace(427, 457, 433);
        mesh2.addFace(432, 426, 456);
        mesh2.addFace(426, 450, 456);
        mesh2.addFace(449, 425, 455);
        mesh2.addFace(425, 431, 455);
        mesh2.addFace(456, 450, 141);
        mesh2.addFace(450, 49, 141);
        mesh2.addFace(48, 449, 140);
        mesh2.addFace(449, 455, 140);
        mesh2.addFace(112, 106, 438);
        mesh2.addFace(112, 438, 442);
        mesh2.addFace(437, 105, 111);
        mesh2.addFace(437, 111, 441);
        mesh2.addFace(190, 114, 448);
        mesh2.addFace(114, 444, 448);
        mesh2.addFace(443, 113, 447);
        mesh2.addFace(113, 189, 447);
        mesh2.addFace(448, 440, 190);
        mesh2.addFace(440, 110, 190);
        mesh2.addFace(109, 439, 189);
        mesh2.addFace(439, 447, 189);
        mesh2.addFace(188, 110, 446);
        mesh2.addFace(110, 440, 446);
        mesh2.addFace(439, 109, 445);
        mesh2.addFace(109, 187, 445);
        mesh2.addFace(446, 438, 106);
        mesh2.addFace(446, 106, 188);
        mesh2.addFace(105, 437, 445);
        mesh2.addFace(105, 445, 187);
        mesh2.addFace(143, 149, 430);
        mesh2.addFace(143, 430, 426);
        mesh2.addFace(429, 148, 142);
        mesh2.addFace(429, 142, 425);
        mesh2.addFace(210, 145, 428);
        mesh2.addFace(210, 428, 436);
        mesh2.addFace(427, 144, 209);
        mesh2.addFace(427, 209, 435);
        mesh2.addFace(436, 430, 210);
        mesh2.addFace(430, 149, 210);
        mesh2.addFace(148, 429, 209);
        mesh2.addFace(429, 435, 209);
        mesh2.addFace(434, 428, 208);
        mesh2.addFace(428, 145, 208);
        mesh2.addFace(144, 427, 207);
        mesh2.addFace(427, 433, 207);
        mesh2.addFace(206, 143, 432);
        mesh2.addFace(143, 426, 432);
        mesh2.addFace(425, 142, 431);
        mesh2.addFace(142, 205, 431);
        mesh2.addFace(408, 422, 398);
        mesh2.addFace(422, 396, 398);
        mesh2.addFace(395, 421, 397);
        mesh2.addFace(421, 407, 397);
        mesh2.addFace(416, 394, 424);
        mesh2.addFace(394, 422, 424);
        mesh2.addFace(421, 393, 423);
        mesh2.addFace(393, 415, 423);
        mesh2.addFace(424, 422, 412);
        mesh2.addFace(422, 408, 412);
        mesh2.addFace(407, 421, 411);
        mesh2.addFace(421, 423, 411);
        mesh2.addFace(400, 416, 420);
        mesh2.addFace(416, 424, 420);
        mesh2.addFace(423, 415, 419);
        mesh2.addFace(415, 399, 419);
        mesh2.addFace(420, 424, 406);
        mesh2.addFace(424, 412, 406);
        mesh2.addFace(411, 423, 405);
        mesh2.addFace(423, 419, 405);
        mesh2.addFace(400, 418, 416);
        mesh2.addFace(418, 276, 416);
        mesh2.addFace(275, 417, 415);
        mesh2.addFace(417, 399, 415);
        mesh2.addFace(394, 416, 402);
        mesh2.addFace(416, 276, 402);
        mesh2.addFace(275, 415, 401);
        mesh2.addFace(415, 393, 401);
        mesh2.addFace(404, 406, 414);
        mesh2.addFace(406, 412, 414);
        mesh2.addFace(411, 405, 413);
        mesh2.addFace(405, 403, 413);
        mesh2.addFace(414, 412, 410);
        mesh2.addFace(412, 408, 410);
        mesh2.addFace(407, 411, 409);
        mesh2.addFace(411, 413, 409);
        mesh2.addFace(410, 408, 334);
        mesh2.addFace(408, 398, 334);
        mesh2.addFace(397, 407, 333);
        mesh2.addFace(407, 409, 333);
        mesh2.addFace(402, 265, 394);
        mesh2.addFace(265, 338, 394);
        mesh2.addFace(337, 264, 393);
        mesh2.addFace(264, 401, 393);
        mesh2.addFace(276, 271, 402);
        mesh2.addFace(271, 267, 402);
        mesh2.addFace(266, 270, 401);
        mesh2.addFace(270, 275, 401);
        mesh2.addFace(396, 392, 398);
        mesh2.addFace(392, 388, 398);
        mesh2.addFace(387, 391, 397);
        mesh2.addFace(391, 395, 397);
        mesh2.addFace(396, 394, 338);
        mesh2.addFace(396, 338, 392);
        mesh2.addFace(337, 393, 395);
        mesh2.addFace(337, 395, 391);
        mesh2.addFace(386, 388, 392);
        mesh2.addFace(386, 392, 390);
        mesh2.addFace(391, 387, 385);
        mesh2.addFace(391, 385, 389);
        mesh2.addFace(390, 392, 338);
        mesh2.addFace(390, 338, 336);
        mesh2.addFace(337, 391, 389);
        mesh2.addFace(337, 389, 335);
        mesh2.addFace(106, 112, 378);
        mesh2.addFace(106, 378, 374);
        mesh2.addFace(377, 111, 105);
        mesh2.addFace(377, 105, 373);
        mesh2.addFace(374, 378, 25);
        mesh2.addFace(378, 27, 25);
        mesh2.addFace(26, 377, 24);
        mesh2.addFace(377, 373, 24);
        mesh2.addFace(104, 29, 380);
        mesh2.addFace(104, 380, 384);
        mesh2.addFace(379, 28, 103);
        mesh2.addFace(379, 103, 383);
        mesh2.addFace(384, 380, 190);
        mesh2.addFace(380, 114, 190);
        mesh2.addFace(113, 379, 189);
        mesh2.addFace(379, 383, 189);
        mesh2.addFace(190, 110, 376);
        mesh2.addFace(190, 376, 384);
        mesh2.addFace(375, 109, 189);
        mesh2.addFace(375, 189, 383);
        mesh2.addFace(384, 376, 23);
        mesh2.addFace(384, 23, 104);
        mesh2.addFace(22, 375, 383);
        mesh2.addFace(22, 383, 103);
        mesh2.addFace(102, 23, 382);
        mesh2.addFace(23, 376, 382);
        mesh2.addFace(375, 22, 381);
        mesh2.addFace(22, 101, 381);
        mesh2.addFace(382, 376, 110);
        mesh2.addFace(382, 110, 188);
        mesh2.addFace(109, 375, 381);
        mesh2.addFace(109, 381, 187);
        mesh2.addFace(188, 106, 382);
        mesh2.addFace(106, 374, 382);
        mesh2.addFace(373, 105, 381);
        mesh2.addFace(105, 187, 381);
        mesh2.addFace(382, 374, 102);
        mesh2.addFace(374, 25, 102);
        mesh2.addFace(24, 373, 101);
        mesh2.addFace(373, 381, 101);
        mesh2.addFace(149, 143, 366);
        mesh2.addFace(143, 362, 366);
        mesh2.addFace(361, 142, 365);
        mesh2.addFace(142, 148, 365);
        mesh2.addFace(366, 362, 43);
        mesh2.addFace(366, 43, 47);
        mesh2.addFace(42, 361, 365);
        mesh2.addFace(42, 365, 46);
        mesh2.addFace(139, 45, 364);
        mesh2.addFace(139, 364, 372);
        mesh2.addFace(363, 44, 138);
        mesh2.addFace(363, 138, 371);
        mesh2.addFace(372, 364, 210);
        mesh2.addFace(364, 145, 210);
        mesh2.addFace(144, 363, 209);
        mesh2.addFace(363, 371, 209);
        mesh2.addFace(210, 149, 366);
        mesh2.addFace(210, 366, 372);
        mesh2.addFace(365, 148, 209);
        mesh2.addFace(365, 209, 371);
        mesh2.addFace(372, 366, 47);
        mesh2.addFace(372, 47, 139);
        mesh2.addFace(46, 365, 371);
        mesh2.addFace(46, 371, 138);
        mesh2.addFace(208, 145, 364);
        mesh2.addFace(208, 364, 370);
        mesh2.addFace(363, 144, 207);
        mesh2.addFace(363, 207, 369);
        mesh2.addFace(370, 364, 45);
        mesh2.addFace(370, 45, 135);
        mesh2.addFace(44, 363, 369);
        mesh2.addFace(44, 369, 134);
        mesh2.addFace(129, 43, 368);
        mesh2.addFace(43, 362, 368);
        mesh2.addFace(361, 42, 367);
        mesh2.addFace(42, 128, 367);
        mesh2.addFace(368, 362, 206);
        mesh2.addFace(362, 143, 206);
        mesh2.addFace(142, 361, 205);
        mesh2.addFace(361, 367, 205);
        mesh2.addFace(332, 360, 328);
        mesh2.addFace(332, 328, 331);
        mesh2.addFace(327, 360, 332);
        mesh2.addFace(327, 332, 330);
        mesh2.addFace(236, 237, 39);
        mesh2.addFace(236, 39, 228);
        mesh2.addFace(39, 237, 235);
        mesh2.addFace(39, 235, 227);
        mesh2.addFace(228, 137, 236);
        mesh2.addFace(137, 131, 236);
        mesh2.addFace(130, 136, 235);
        mesh2.addFace(136, 227, 235);
        mesh2.addFace(137, 47, 131);
        mesh2.addFace(47, 43, 131);
        mesh2.addFace(42, 46, 130);
        mesh2.addFace(46, 136, 130);
        mesh2.addFace(346, 282, 160);
        mesh2.addFace(346, 160, 340);
        mesh2.addFace(159, 281, 345);
        mesh2.addFace(159, 345, 339);
        mesh2.addFace(354, 346, 352);
        mesh2.addFace(346, 340, 352);
        mesh2.addFace(339, 345, 351);
        mesh2.addFace(345, 353, 351);
        mesh2.addFace(354, 348, 346);
        mesh2.addFace(348, 344, 346);
        mesh2.addFace(343, 347, 345);
        mesh2.addFace(347, 353, 345);
        mesh2.addFace(346, 344, 273);
        mesh2.addFace(346, 273, 282);
        mesh2.addFace(272, 343, 345);
        mesh2.addFace(272, 345, 281);
        mesh2.addFace(273, 344, 271);
        mesh2.addFace(273, 271, 276);
        mesh2.addFace(270, 343, 272);
        mesh2.addFace(270, 272, 275);
        mesh2.addFace(350, 352, 340);
        mesh2.addFace(350, 340, 342);
        mesh2.addFace(339, 351, 349);
        mesh2.addFace(339, 349, 341);
        mesh2.addFace(348, 350, 344);
        mesh2.addFace(350, 342, 344);
        mesh2.addFace(341, 349, 343);
        mesh2.addFace(349, 347, 343);
        mesh2.addFace(344, 342, 271);
        mesh2.addFace(342, 216, 271);
        mesh2.addFace(215, 341, 270);
        mesh2.addFace(341, 343, 270);
        mesh2.addFace(102, 25, 96);
        mesh2.addFace(25, 21, 96);
        mesh2.addFace(20, 24, 95);
        mesh2.addFace(24, 101, 95);
        mesh2.addFace(102, 96, 23);
        mesh2.addFace(96, 13, 23);
        mesh2.addFace(12, 95, 22);
        mesh2.addFace(95, 101, 22);
        mesh2.addFace(104, 23, 13);
        mesh2.addFace(104, 13, 87);
        mesh2.addFace(12, 22, 103);
        mesh2.addFace(12, 103, 86);
        mesh2.addFace(104, 87, 1);
        mesh2.addFace(104, 1, 29);
        mesh2.addFace(0, 86, 103);
        mesh2.addFace(0, 103, 28);
        mesh2.addFace(25, 27, 21);
        mesh2.addFace(27, 19, 21);
        mesh2.addFace(18, 26, 20);
        mesh2.addFace(26, 24, 20);
        mesh2.addFace(342, 274, 216);
        mesh2.addFace(274, 158, 216);
        mesh2.addFace(158, 274, 215);
        mesh2.addFace(274, 341, 215);
        mesh2.addFace(304, 358, 301);
        mesh2.addFace(304, 301, 303);
        mesh2.addFace(300, 357, 304);
        mesh2.addFace(300, 304, 303);
        mesh2.addFace(358, 290, 308);
        mesh2.addFace(290, 306, 308);
        mesh2.addFace(305, 289, 307);
        mesh2.addFace(289, 357, 307);
        mesh2.addFace(358, 308, 220);
        mesh2.addFace(358, 220, 301);
        mesh2.addFace(219, 307, 357);
        mesh2.addFace(219, 357, 300);
        mesh2.addFace(306, 290, 282);
        mesh2.addFace(290, 160, 282);
        mesh2.addFace(159, 289, 281);
        mesh2.addFace(289, 305, 281);
        mesh2.addFace(274, 342, 340);
        mesh2.addFace(274, 340, 56);
        mesh2.addFace(339, 341, 274);
        mesh2.addFace(339, 274, 56);
        mesh2.addFace(336, 338, 265);
        mesh2.addFace(336, 265, 263);
        mesh2.addFace(264, 337, 335);
        mesh2.addFace(264, 335, 262);
        mesh2.addFace(265, 267, 214);
        mesh2.addFace(265, 214, 263);
        mesh2.addFace(213, 266, 264);
        mesh2.addFace(213, 264, 262);
        mesh2.addFace(301, 220, 162);
        mesh2.addFace(301, 162, 298);
        mesh2.addFace(162, 219, 300);
        mesh2.addFace(162, 300, 297);
        mesh2.addFace(331, 325, 332);
        mesh2.addFace(325, 326, 332);
        mesh2.addFace(326, 324, 332);
        mesh2.addFace(324, 330, 332);
        mesh2.addFace(328, 322, 325);
        mesh2.addFace(328, 325, 331);
        mesh2.addFace(324, 321, 327);
        mesh2.addFace(324, 327, 330);
        mesh2.addFace(329, 323, 322);
        mesh2.addFace(329, 322, 328);
        mesh2.addFace(321, 323, 329);
        mesh2.addFace(321, 329, 327);
        mesh2.addFace(325, 319, 326);
        mesh2.addFace(319, 320, 326);
        mesh2.addFace(320, 318, 326);
        mesh2.addFace(318, 324, 326);
        mesh2.addFace(322, 317, 319);
        mesh2.addFace(322, 319, 325);
        mesh2.addFace(318, 316, 321);
        mesh2.addFace(318, 321, 324);
        mesh2.addFace(323, 315, 317);
        mesh2.addFace(323, 317, 322);
        mesh2.addFace(316, 315, 323);
        mesh2.addFace(316, 323, 321);
        mesh2.addFace(319, 294, 320);
        mesh2.addFace(294, 296, 320);
        mesh2.addFace(296, 293, 320);
        mesh2.addFace(293, 318, 320);
        mesh2.addFace(317, 288, 294);
        mesh2.addFace(317, 294, 319);
        mesh2.addFace(293, 287, 316);
        mesh2.addFace(293, 316, 318);
        mesh2.addFace(315, 286, 288);
        mesh2.addFace(315, 288, 317);
        mesh2.addFace(287, 286, 315);
        mesh2.addFace(287, 315, 316);
        mesh2.addFace(220, 218, 161);
        mesh2.addFace(220, 161, 162);
        mesh2.addFace(161, 217, 219);
        mesh2.addFace(161, 219, 162);
        mesh2.addFace(308, 280, 218);
        mesh2.addFace(308, 218, 220);
        mesh2.addFace(217, 279, 307);
        mesh2.addFace(217, 307, 219);
        mesh2.addFace(306, 314, 280);
        mesh2.addFace(306, 280, 308);
        mesh2.addFace(279, 313, 305);
        mesh2.addFace(279, 305, 307);
        mesh2.addFace(314, 306, 276);
        mesh2.addFace(314, 276, 278);
        mesh2.addFace(275, 305, 313);
        mesh2.addFace(275, 313, 277);
        mesh2.addFace(120, 310, 222);
        mesh2.addFace(310, 198, 222);
        mesh2.addFace(197, 309, 221);
        mesh2.addFace(309, 119, 221);
        mesh2.addFace(278, 246, 312);
        mesh2.addFace(278, 312, 314);
        mesh2.addFace(311, 245, 277);
        mesh2.addFace(311, 277, 313);
        mesh2.addFace(314, 312, 244);
        mesh2.addFace(314, 244, 280);
        mesh2.addFace(243, 311, 313);
        mesh2.addFace(243, 313, 279);
        mesh2.addFace(246, 198, 310);
        mesh2.addFace(246, 310, 312);
        mesh2.addFace(309, 197, 245);
        mesh2.addFace(309, 245, 311);
        mesh2.addFace(312, 310, 244);
        mesh2.addFace(310, 120, 244);
        mesh2.addFace(119, 309, 243);
        mesh2.addFace(309, 311, 243);
        mesh2.addFace(292, 285, 290);
        mesh2.addFace(285, 160, 290);
        mesh2.addFace(159, 284, 289);
        mesh2.addFace(284, 291, 289);
        mesh2.addFace(295, 292, 304);
        mesh2.addFace(292, 290, 304);
        mesh2.addFace(289, 291, 304);
        mesh2.addFace(291, 295, 304);
        mesh2.addFace(303, 301, 302);
        mesh2.addFace(301, 298, 302);
        mesh2.addFace(297, 300, 302);
        mesh2.addFace(300, 303, 302);
        mesh2.addFace(296, 294, 292);
        mesh2.addFace(296, 292, 295);
        mesh2.addFace(291, 293, 296);
        mesh2.addFace(291, 296, 295);
        mesh2.addFace(294, 288, 292);
        mesh2.addFace(288, 285, 292);
        mesh2.addFace(284, 287, 291);
        mesh2.addFace(287, 293, 291);
        mesh2.addFace(286, 283, 285);
        mesh2.addFace(286, 285, 288);
        mesh2.addFace(284, 283, 286);
        mesh2.addFace(284, 286, 287);
        mesh2.addFace(283, 56, 160);
        mesh2.addFace(283, 160, 285);
        mesh2.addFace(159, 56, 283);
        mesh2.addFace(159, 283, 284);
        mesh2.addFace(306, 282, 273);
        mesh2.addFace(306, 273, 276);
        mesh2.addFace(272, 281, 305);
        mesh2.addFace(272, 305, 275);
        mesh2.addFace(280, 244, 118);
        mesh2.addFace(280, 118, 218);
        mesh2.addFace(117, 243, 279);
        mesh2.addFace(117, 279, 217);
        mesh2.addFace(269, 248, 246);
        mesh2.addFace(269, 246, 278);
        mesh2.addFace(245, 247, 268);
        mesh2.addFace(245, 268, 277);
        mesh2.addFace(271, 216, 267);
        mesh2.addFace(216, 153, 267);
        mesh2.addFace(152, 215, 266);
        mesh2.addFace(215, 270, 266);
        mesh2.addFace(269, 263, 212);
        mesh2.addFace(263, 156, 212);
        mesh2.addFace(155, 262, 211);
        mesh2.addFace(262, 268, 211);
        mesh2.addFace(248, 269, 212);
        mesh2.addFace(248, 212, 242);
        mesh2.addFace(211, 268, 247);
        mesh2.addFace(211, 247, 241);
        mesh2.addFace(242, 240, 232);
        mesh2.addFace(242, 232, 234);
        mesh2.addFace(232, 240, 241);
        mesh2.addFace(232, 241, 233);
        mesh2.addFace(242, 234, 192);
        mesh2.addFace(242, 192, 248);
        mesh2.addFace(191, 233, 241);
        mesh2.addFace(191, 241, 247);
        mesh2.addFace(246, 248, 198);
        mesh2.addFace(248, 192, 198);
        mesh2.addFace(191, 247, 197);
        mesh2.addFace(247, 245, 197);
        mesh2.addFace(194, 228, 121);
        mesh2.addFace(228, 39, 121);
        mesh2.addFace(39, 227, 121);
        mesh2.addFace(227, 193, 121);
        mesh2.addFace(228, 194, 226);
        mesh2.addFace(194, 120, 226);
        mesh2.addFace(119, 193, 225);
        mesh2.addFace(193, 227, 225);
        mesh2.addFace(120, 222, 41);
        mesh2.addFace(120, 41, 226);
        mesh2.addFace(40, 221, 119);
        mesh2.addFace(40, 119, 225);
        mesh2.addFace(87, 88, 72);
        mesh2.addFace(87, 72, 164);
        mesh2.addFace(72, 88, 86);
        mesh2.addFace(72, 86, 163);
        mesh2.addFace(164, 58, 1);
        mesh2.addFace(164, 1, 87);
        mesh2.addFace(0, 57, 163);
        mesh2.addFace(0, 163, 86);
        mesh2.addFace(198, 192, 64);
        mesh2.addFace(198, 64, 123);
        mesh2.addFace(63, 191, 197);
        mesh2.addFace(63, 197, 122);
        mesh2.addFace(80, 239, 68);
        mesh2.addFace(239, 64, 68);
        mesh2.addFace(63, 238, 67);
        mesh2.addFace(238, 79, 67);
        mesh2.addFace(80, 68, 7);
        mesh2.addFace(80, 7, 11);
        mesh2.addFace(6, 67, 79);
        mesh2.addFace(6, 79, 10);
        mesh2.addFace(78, 11, 66);
        mesh2.addFace(11, 7, 66);
        mesh2.addFace(6, 10, 65);
        mesh2.addFace(10, 77, 65);
        mesh2.addFace(66, 85, 5);
        mesh2.addFace(66, 5, 78);
        mesh2.addFace(4, 84, 65);
        mesh2.addFace(4, 65, 77);
        mesh2.addFace(98, 186, 180);
        mesh2.addFace(186, 85, 180);
        mesh2.addFace(84, 185, 179);
        mesh2.addFace(185, 97, 179);
        mesh2.addFace(182, 176, 83);
        mesh2.addFace(182, 83, 91);
        mesh2.addFace(83, 175, 181);
        mesh2.addFace(83, 181, 91);
        mesh2.addFace(182, 180, 85);
        mesh2.addFace(182, 85, 176);
        mesh2.addFace(84, 179, 181);
        mesh2.addFace(84, 181, 175);
        mesh2.addFace(94, 186, 98);
        mesh2.addFace(94, 98, 100);
        mesh2.addFace(97, 185, 93);
        mesh2.addFace(97, 93, 99);
        mesh2.addFace(100, 21, 19);
        mesh2.addFace(100, 19, 94);
        mesh2.addFace(18, 20, 99);
        mesh2.addFace(18, 99, 93);
        mesh2.addFace(261, 257, 259);
        mesh2.addFace(257, 255, 259);
        mesh2.addFace(254, 257, 258);
        mesh2.addFace(257, 261, 258);
        mesh2.addFace(259, 255, 260);
        mesh2.addFace(255, 256, 260);
        mesh2.addFace(256, 254, 260);
        mesh2.addFace(254, 258, 260);
        mesh2.addFace(257, 252, 255);
        mesh2.addFace(252, 251, 255);
        mesh2.addFace(250, 252, 254);
        mesh2.addFace(252, 257, 254);
        mesh2.addFace(255, 251, 256);
        mesh2.addFace(251, 253, 256);
        mesh2.addFace(253, 250, 256);
        mesh2.addFace(250, 254, 256);
        mesh2.addFace(252, 92, 251);
        mesh2.addFace(92, 90, 251);
        mesh2.addFace(89, 92, 250);
        mesh2.addFace(92, 252, 250);
        mesh2.addFace(253, 251, 249);
        mesh2.addFace(251, 90, 249);
        mesh2.addFace(89, 250, 249);
        mesh2.addFace(250, 253, 249);
        mesh2.addFace(91, 249, 90);
        mesh2.addFace(91, 90, 182);
        mesh2.addFace(89, 249, 91);
        mesh2.addFace(89, 91, 181);
        mesh2.addFace(180, 182, 90);
        mesh2.addFace(180, 90, 178);
        mesh2.addFace(89, 181, 179);
        mesh2.addFace(89, 179, 177);
        mesh2.addFace(92, 17, 90);
        mesh2.addFace(17, 16, 90);
        mesh2.addFace(15, 17, 89);
        mesh2.addFace(17, 92, 89);
        mesh2.addFace(17, 14, 16);
        mesh2.addFace(14, 13, 16);
        mesh2.addFace(12, 14, 15);
        mesh2.addFace(14, 17, 15);
        mesh2.addFace(88, 87, 14);
        mesh2.addFace(87, 13, 14);
        mesh2.addFace(12, 86, 14);
        mesh2.addFace(86, 88, 14);
        mesh2.addFace(176, 85, 168);
        mesh2.addFace(85, 66, 168);
        mesh2.addFace(65, 84, 167);
        mesh2.addFace(84, 175, 167);
        mesh2.addFace(176, 168, 73);
        mesh2.addFace(176, 73, 83);
        mesh2.addFace(73, 167, 175);
        mesh2.addFace(73, 175, 83);
        mesh2.addFace(184, 100, 178);
        mesh2.addFace(100, 98, 178);
        mesh2.addFace(97, 99, 177);
        mesh2.addFace(99, 183, 177);
        mesh2.addFace(178, 90, 16);
        mesh2.addFace(178, 16, 184);
        mesh2.addFace(15, 89, 177);
        mesh2.addFace(15, 177, 183);
        mesh2.addFace(236, 166, 71);
        mesh2.addFace(236, 71, 237);
        mesh2.addFace(71, 165, 235);
        mesh2.addFace(71, 235, 237);
        mesh2.addFace(236, 125, 166);
        mesh2.addFace(125, 60, 166);
        mesh2.addFace(59, 124, 165);
        mesh2.addFace(124, 235, 165);
        mesh2.addFace(231, 229, 74);
        mesh2.addFace(231, 74, 170);
        mesh2.addFace(74, 229, 230);
        mesh2.addFace(74, 230, 169);
        mesh2.addFace(170, 68, 64);
        mesh2.addFace(170, 64, 231);
        mesh2.addFace(63, 67, 169);
        mesh2.addFace(63, 169, 230);
        mesh2.addFace(196, 239, 80);
        mesh2.addFace(196, 80, 174);
        mesh2.addFace(79, 238, 195);
        mesh2.addFace(79, 195, 173);
        mesh2.addFace(196, 174, 125);
        mesh2.addFace(174, 60, 125);
        mesh2.addFace(59, 173, 124);
        mesh2.addFace(173, 195, 124);
        mesh2.addFace(120, 194, 118);
        mesh2.addFace(120, 118, 244);
        mesh2.addFace(117, 193, 119);
        mesh2.addFace(117, 119, 243);
        mesh2.addFace(212, 154, 242);
        mesh2.addFace(154, 240, 242);
        mesh2.addFace(240, 154, 241);
        mesh2.addFace(154, 211, 241);
        mesh2.addFace(131, 200, 236);
        mesh2.addFace(200, 125, 236);
        mesh2.addFace(124, 199, 235);
        mesh2.addFace(199, 130, 235);
        mesh2.addFace(234, 231, 192);
        mesh2.addFace(231, 64, 192);
        mesh2.addFace(63, 230, 191);
        mesh2.addFace(230, 233, 191);
        mesh2.addFace(229, 231, 234);
        mesh2.addFace(229, 234, 232);
        mesh2.addFace(233, 230, 229);
        mesh2.addFace(233, 229, 232);
        mesh2.addFace(16, 13, 184);
        mesh2.addFace(13, 96, 184);
        mesh2.addFace(95, 12, 183);
        mesh2.addFace(12, 15, 183);
        mesh2.addFace(222, 198, 123);
        mesh2.addFace(222, 123, 224);
        mesh2.addFace(122, 197, 221);
        mesh2.addFace(122, 221, 223);
        mesh2.addFace(226, 204, 228);
        mesh2.addFace(204, 137, 228);
        mesh2.addFace(136, 203, 227);
        mesh2.addFace(203, 225, 227);
        mesh2.addFace(41, 133, 226);
        mesh2.addFace(133, 204, 226);
        mesh2.addFace(203, 132, 225);
        mesh2.addFace(132, 40, 225);
        mesh2.addFace(224, 127, 41);
        mesh2.addFace(224, 41, 222);
        mesh2.addFace(40, 126, 223);
        mesh2.addFace(40, 223, 221);
        mesh2.addFace(118, 38, 161);
        mesh2.addFace(118, 161, 218);
        mesh2.addFace(161, 38, 117);
        mesh2.addFace(161, 117, 217);
        mesh2.addFace(158, 54, 216);
        mesh2.addFace(54, 153, 216);
        mesh2.addFace(152, 54, 215);
        mesh2.addFace(54, 158, 215);
        mesh2.addFace(153, 54, 157);
        mesh2.addFace(153, 157, 214);
        mesh2.addFace(157, 54, 152);
        mesh2.addFace(157, 152, 213);
        mesh2.addFace(157, 55, 214);
        mesh2.addFace(55, 156, 214);
        mesh2.addFace(155, 55, 213);
        mesh2.addFace(55, 157, 213);
        mesh2.addFace(156, 55, 212);
        mesh2.addFace(55, 154, 212);
        mesh2.addFace(154, 55, 211);
        mesh2.addFace(55, 155, 211);
        mesh2.addFace(133, 45, 204);
        mesh2.addFace(45, 139, 204);
        mesh2.addFace(138, 44, 203);
        mesh2.addFace(44, 132, 203);
        mesh2.addFace(139, 47, 137);
        mesh2.addFace(139, 137, 204);
        mesh2.addFace(136, 46, 138);
        mesh2.addFace(136, 138, 203);
        mesh2.addFace(135, 45, 202);
        mesh2.addFace(45, 133, 202);
        mesh2.addFace(132, 44, 201);
        mesh2.addFace(44, 134, 201);
        mesh2.addFace(133, 41, 127);
        mesh2.addFace(133, 127, 202);
        mesh2.addFace(126, 40, 132);
        mesh2.addFace(126, 132, 201);
        mesh2.addFace(131, 43, 129);
        mesh2.addFace(131, 129, 200);
        mesh2.addFace(128, 42, 130);
        mesh2.addFace(128, 130, 199);
        mesh2.addFace(121, 38, 194);
        mesh2.addFace(38, 118, 194);
        mesh2.addFace(117, 38, 193);
        mesh2.addFace(38, 121, 193);
        mesh2.addFace(96, 21, 100);
        mesh2.addFace(96, 100, 184);
        mesh2.addFace(99, 20, 95);
        mesh2.addFace(99, 95, 183);
        mesh2.addFace(80, 11, 174);
        mesh2.addFace(11, 82, 174);
        mesh2.addFace(81, 10, 173);
        mesh2.addFace(10, 79, 173);
        mesh2.addFace(82, 3, 60);
        mesh2.addFace(82, 60, 174);
        mesh2.addFace(59, 2, 81);
        mesh2.addFace(59, 81, 173);
        mesh2.addFace(82, 11, 78);
        mesh2.addFace(82, 78, 172);
        mesh2.addFace(77, 10, 81);
        mesh2.addFace(77, 81, 171);
        mesh2.addFace(78, 5, 62);
        mesh2.addFace(78, 62, 172);
        mesh2.addFace(61, 4, 77);
        mesh2.addFace(61, 77, 171);
        mesh2.addFace(62, 1, 58);
        mesh2.addFace(62, 58, 172);
        mesh2.addFace(57, 0, 61);
        mesh2.addFace(57, 61, 171);
        mesh2.addFace(58, 3, 172);
        mesh2.addFace(3, 82, 172);
        mesh2.addFace(81, 2, 171);
        mesh2.addFace(2, 57, 171);
        mesh2.addFace(74, 9, 170);
        mesh2.addFace(9, 76, 170);
        mesh2.addFace(75, 9, 169);
        mesh2.addFace(9, 74, 169);
        mesh2.addFace(76, 7, 68);
        mesh2.addFace(76, 68, 170);
        mesh2.addFace(67, 6, 75);
        mesh2.addFace(67, 75, 169);
        mesh2.addFace(76, 9, 168);
        mesh2.addFace(9, 73, 168);
        mesh2.addFace(73, 9, 167);
        mesh2.addFace(9, 75, 167);
        mesh2.addFace(66, 7, 168);
        mesh2.addFace(7, 76, 168);
        mesh2.addFace(75, 6, 167);
        mesh2.addFace(6, 65, 167);
        mesh2.addFace(70, 8, 71);
        mesh2.addFace(70, 71, 166);
        mesh2.addFace(71, 8, 69);
        mesh2.addFace(71, 69, 165);
        mesh2.addFace(60, 3, 166);
        mesh2.addFace(3, 70, 166);
        mesh2.addFace(69, 2, 165);
        mesh2.addFace(2, 59, 165);
        mesh2.addFace(72, 8, 164);
        mesh2.addFace(8, 70, 164);
        mesh2.addFace(69, 8, 163);
        mesh2.addFace(8, 72, 163);
        mesh2.addFace(70, 3, 58);
        mesh2.addFace(70, 58, 164);
        mesh2.addFace(57, 2, 69);
        mesh2.addFace(57, 69, 163);
    }
    
    wm.add( 0, 0, 80, 80, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh2, 1);
    wm.add(80, 0, 80, 80, M5.Lcd.color565(200, 44, 85), M5.Lcd.color565(200, 200, 200), mesh2, 2);
}

void loop() {
    wm.fixedDraw();
}