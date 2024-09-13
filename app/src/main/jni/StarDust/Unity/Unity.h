#include <android/log.h>
#include "Vector3.hpp"
#include "Vector2.hpp"
#include "Quaternion.hpp"

float NormalizeAngle (float angle){
    while (angle>360)
        angle -= 360;
    while (angle<0)
        angle += 360;
    return angle;
}

Vector3 NormalizeAngles (Vector3 angles){
    angles.X = NormalizeAngle (angles.X);
    angles.Y = NormalizeAngle (angles.Y);
    angles.Z = NormalizeAngle (angles.Z);
    return angles;
}

Vector3 ToEulerRad(Quaternion q1){
    float Rad2Deg = 360.0 / (M_PI * 2.0);

    float sqw = q1.W * q1.W;
    float sqx = q1.X * q1.X;
    float sqy = q1.Y * q1.Y;
    float sqz = q1.Z * q1.Z;
    float unit = sqx + sqy + sqz + sqw;
    float test = q1.X * q1.W - q1.Y * q1.Z;
    Vector3 v;

    if (test>0.4995*unit) {
        v.Y = 2.0 * atan2f (q1.Y, q1.X);
        v.X = M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles(v * Rad2Deg);
    }
    if (test<-0.4995*unit) {
        v.Y = -2.0 * atan2f (q1.Y, q1.X);
        v.X = -M_PI / 2.0;
        v.Z = 0;
        return NormalizeAngles (v * Rad2Deg);
    }
    Quaternion q(q1.W, q1.Z, q1.X, q1.Y);
    v.Y = atan2f (2.0 * q.X * q.W + 2.0 * q.Y * q.Z, 1 - 2.0 * (q.Z * q.Z + q.W * q.W)); // yaw
    v.X = asinf (2.0 * (q.X * q.Z - q.W * q.Y)); // pitch
    v.Z = atan2f (2.0 * q.X * q.Y + 2.0 * q.Z * q.W, 1 - 2.0 * (q.Y * q.Y + q.Z * q.Z)); // roll
    return NormalizeAngles (v * Rad2Deg);
}

Quaternion GetRotationToLocation(Vector3 targetLocation, float y_bias, Vector3 myLoc){
    return Quaternion::LookRotation((targetLocation + Vector3(0, y_bias, 0)) - myLoc, Vector3(0, 1, 0));
}

template <typename T>
struct monoArray
{
    void* klass;
    void* monitor;
    void* bounds;
    int   max_length;
    void* vector [1];
    int getLength()
    {
        return max_length;
    }
    T getPointer()
    {
        return (T)vector;
    }
};

template <typename T>
struct monoList {
    void *unk0;
    void *unk1;
    monoArray<T> *items;
    int size;
    int version;

    T getItems(){
        return items->getPointer();
    }

    int getSize(){
        return size;
    }

    int getVersion(){
        return version;
    }
};

template <typename K, typename V>
struct monoDictionary {
    void *unk0;
    void *unk1;
    monoArray<int **> *table;
    monoArray<void **> *linkSlots;
    monoArray<K> *keys;
    monoArray<V> *values;
    int touchedSlots;
    int emptySlot;
    int size;

    K getKeys(){
        return keys->getPointer();
    }

    V getValues(){
        return values->getPointer();
    }

    int getNumKeys(){
        return keys->getLength();
    }

    int getNumValues(){
        return values->getLength();
    }

    int getSize(){
        return size;
    }
};
union intfloat {
	int i;
	float f;
};

using namespace std;
std::string 
utf16le_to_utf8(const std::u16string &u16str) {    
if (u16str.empty()) {
    return std::string();
}

const char16_t *p = u16str.data();   
std::u16string::size_type len = u16str.length();  
if (p[0] == 0xFEFF) {      
    p += 1;      
    len -= 1;   
} 

std::string u8str;   
u8str.reserve(len * 3);    
char16_t u16char;  
for (std::u16string::size_type i = 0; i < len; ++i) {       
     u16char = p[i];        
     if (u16char < 0x0080) {          
         u8str.push_back((char) (u16char & 0x00FF));          
         continue;       
     }      
     if (u16char >= 0x0080 && u16char <= 0x07FF) {          
         u8str.push_back((char) (((u16char >> 6) & 0x1F) | 0xC0));        
         u8str.push_back((char) ((u16char & 0x3F) | 0x80));          
         continue;       
    }      
    if (u16char >= 0xD800 && u16char <= 0xDBFF) {           
        uint32_t highSur = u16char;          
        uint32_t lowSur = p[++i];          
        uint32_t codePoint = highSur - 0xD800;    
        codePoint <<= 10;        
        codePoint |= lowSur - 0xDC00;      
        codePoint += 0x10000;           
        u8str.push_back((char) ((codePoint >> 18) | 0xF0));     
        u8str.push_back((char) (((codePoint >> 12) & 0x3F) | 0x80));         
        u8str.push_back((char) (((codePoint >> 06) & 0x3F) | 0x80));            
        u8str.push_back((char) ((codePoint & 0x3F) | 0x80));        
        continue;       
    } { 
    u8str.push_back((char) (((u16char >> 12) & 0x0F) | 0xE0));          
    u8str.push_back((char) (((u16char >> 6) & 0x3F) | 0x80));      
    u8str.push_back((char) ((u16char & 0x3F) | 0x80));         
    continue;      
    }    }   
    return u8str;
    } typedef struct _monoString {   
    void *klass;   
    void *monitor;    
    int length;    
    const char *toChars() {       
    u16string ss((char16_t *) getChars(), 0, getLength());     
    string str = utf16le_to_utf8(ss);  
    return str.c_str();   
    }    
    char chars[0];   
    char *getChars() {    
    return chars;    
    }  
    int getLength() {       
    return length;  
    }    
    std::string get_string() {             
    return std::string(toChars());  
    }} 
	monoString;
