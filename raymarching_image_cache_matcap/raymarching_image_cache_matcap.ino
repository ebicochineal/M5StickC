#include <M5StickC.h>
#include "matcapdata.h"

byte render_status[80];
uint16_t* dbuff[32];
uint16_t* buff;
uint16_t frame = 0;
bool rf = false;

// struct Vector2;
// struct Vector3;

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

Vector3 normalize(Vector3 v) {
    float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (d == 0) { return Vector3(0.0f, 0.0f, 0.0f); }
    return v / d;
}

Vector3 cross(const Vector3 a, const Vector3 b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float dot(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float distance(Vector3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float distance(Vector2 v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

Vector3 abs3 (Vector3 p) {
    p.x = abs(p.x); p.y = abs(p.y); p.z = abs(p.z);
    return p;
}

Vector3 fmod3 (Vector3 p, float m) {
    p.x = fmod(p.x, m);
    p.y = fmod(p.y, m);
    p.z = fmod(p.z, m);
    return p;
}

Vector3 matmul (Vector3 v, float m[]) {
    return Vector3(v.x*m[0]+v.y*m[1]+v.z*m[2], v.x*m[3]+v.y*m[4]+v.z*m[5], v.x*m[6]+v.y*m[7]+v.z*m[8]);
}

float radian (float d) {
    return d * 0.01745329251f;
}

Vector3 rotate (Vector3 p, float angle, Vector3 axis){
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0f - c;
    float m[9] = {
        axis.x * axis.x * r + c,
        axis.y * axis.x * r + axis.z * s,
        axis.z * axis.x * r - axis.y * s,
        axis.x * axis.y * r - axis.z * s,
        axis.y * axis.y * r + c,
        axis.z * axis.y * r + axis.x * s,
        axis.x * axis.z * r + axis.y * s,
        axis.y * axis.z * r - axis.x * s,
        axis.z * axis.z * r + c
    };
    
    return matmul(p, m);
}

Vector3 repeat (Vector3 p, float d){ return fmod3(abs3(p), d) - d * 0.5f; }

float mix (float x, float y, float a) { return x-x*a+y*a; }

float distanceFunction (Vector3 p){
    p = repeat(p, 8.0);
    Vector3 t(2.0, 0.6, 0.6);
    Vector2 q(distance(Vector2(p.x, p.z))-t.x, p.y);
    return distance(q)-t.y;
}

Vector3 worldNormal(Vector3 p){
    float d = 0.0001f;
    return normalize(Vector3(
        distanceFunction(p + Vector3(   d, 0.0f, 0.0f)) - distanceFunction(p + Vector3(  -d,  0.0f, 0.0f)),
        distanceFunction(p + Vector3(0.0f,    d, 0.0f)) - distanceFunction(p + Vector3(0.0f,    -d, 0.0f)),
        distanceFunction(p + Vector3(0.0f, 0.0f,    d)) - distanceFunction(p + Vector3(0.0f,  0.0f,   -d))
    ));
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float lerp (float ca, float cb, float v) {
    return fmap(min(v, 0.5f), 0.0f, 0.5f, ca, cb);
}

uint16_t rayMarching (const byte& x, const byte& y) {
    const Vector2 resolution(160, 80);
    const Vector2 xy(x, 80-y);
    
    Vector2 p = (xy * 2.0f - resolution) / min(resolution.x, resolution.y);
    
    Vector3 cpos;
    if (frame < 16) {
        cpos = Vector3(0.0f, 0.0f, 16.0f - (frame*0.5));
    } else {
        cpos = Vector3(0.0f + (frame*0.5), 0.0f, 16.0f);
    }
    
    Vector3 cup   = normalize(Vector3(-1.0f, 1.0f, 1.0f));
    Vector3 cdir  = normalize(Vector3(1.0f, 1.0f, -1.0f));
    
    Vector3 cside = cross(cdir, cup);
    float targetdepth = 2.0f;
    Vector3 ray = normalize(cside * p.x + cup * p.y + cdir * targetdepth);
    
    float dist = 0.0f;
    float rlen = 0.0f;
    Vector3 rpos = cpos;
    float count = 0.0f;
    byte material = 0;
    
    for (int i = 0; i < 64; ++i) {
        dist = distanceFunction(rpos);
        rlen += dist;
        rpos = cpos +  ray * rlen;
        if (dist < 0.001f || rlen > 100.0f) { break; }
    }
    
    Vector3 normal = worldNormal(rpos);
    // Vector3 ldir   = normalize(Vector3(0.0f, 1.0f, 1.0f));
    // float diff = min(max(dot(ldir, normal), 0.1f), 1.0f);
    float mat[9] = {cside.x, cside.y, cside.z, cup.x, cup.y, cup.z, -cdir.x, -cdir.y, -cdir.z};
    normal = matmul(normal, mat);
    
    int cx = normal.x * 16 + 16;
    int cy = -normal.y * 16 + 16;
    int c = cx + cy * 32;
    Vector3 color = normal *0.5+0.5;
    color = Vector3(texr2[c]*0.00392156f, texg2[c]*0.00392156f, texb2[c]*0.00392156f)  * (100.0f-rlen)/100.0f;
    
    // color = normal * 0.5f + 0.5f;
    if (rlen > 100.0f) { color = Vector3(0.0f, 0.0f, 0.0f); }
    
    return M5.Lcd.color565(color.x*255, color.y*255, color.z*255);
}

void drawBuffer (void* param) {
    while (1) {
        delay(1);
        if (rf) { continue; }
        
        // x2
        for (int y = 0; y < 80; y += 2) {
            if (render_status[y] < 2) { continue; }
            render_status[y] = 1;
            render_status[y+1] = 1;
            const int k = frame % 32;
            for (int x = 0; x < 160; x += 2) {
                const uint16_t c = rayMarching(x, y);
                dbuff[k][x/2 + y*40] = c;
            }
            render_status[y] = 0;
            render_status[y+1] = 0;
            delay(1);
        }
        
    }
}

bool isRender () {
    for (int i = 0; i < 80; i += 1) {
        if (render_status[i] > 0) { return false; }
    }
    
    return true;
}

void drawScreen (int tbuff) {
    M5.Lcd.setAddrWindow(0, 0, 160, 80);
    
    for (int y = 0; y < 80; y += 2) {
        for (int x = 0; x < 160; x += 2) {
            const uint16_t c = dbuff[tbuff][x/2 + y*40];
            buff[x + y*160] = c;
            buff[(x+1) + y*160] = c;
            buff[x + (y+1)*160] = c;
            buff[(x+1) + (y+1)*160] = c;
        }
    }
    
    M5.Lcd.pushColors(buff, 12800);
}

void setup() {
    M5.begin();
    M5.Lcd.setRotation(1);
    M5.Axp.ScreenBreath(8);
    
    for (int i = 0; i < 80; ++i) { render_status[i] = 2; }
    
    for (int i = 0; i < 32; ++i) {
        dbuff[i] = new uint16_t[3200];
        for (int j = 0; j < 3200; ++j) { dbuff[i][j] = 0; }
    }
    buff = new uint16_t[12800];
    for (int i = 0; i < 12800; ++i) { buff[i] = 0; }
    
    xTaskCreatePinnedToCore(drawBuffer, "Task0", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(drawBuffer, "Task1", 4096, NULL, 1, NULL, 1);
}

void loop() {
    if (isRender() || rf) {
        int tbuff = frame % 32;
        if (frame > 31) { rf = true; }
        frame += 1;
        for (int i = 0; i < 80; ++i) { render_status[i] = 2; }
        drawScreen(tbuff);
    }
    delay(16);
}