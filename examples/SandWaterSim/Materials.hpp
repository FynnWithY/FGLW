constexpr int WIDTH = FGLW_RESOLUTION_W / 4;
constexpr int HEIGHT = FGLW_RESOLUTION_H / 4;

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
struct Color
{
    Color() = default;
    Color(uint32 col) : color(col) {}
    Color(uint8 a, uint8 b, uint8 g, uint8 r) : a(a), b(b), g(g), r(r) {}
    union
    {
        uint32 color;
        struct
        {
            uint8 a, b, g, r;
        };
    };
    operator uint32 &() { return color; };
    Color operator*(float val) { return Color(a, b * val, g * val, r * val); }
};
enum MaterialType
{
    MAT_AIR,
    MAT_SAND,
    MAT_WATER,
    MAT_LAST
};
template <MaterialType T>
struct Material
{
    static Color color;
    static MaterialType type;
    static void Update(MaterialType *map, int x, int y) {}
};

template <>
Color Material<MAT_AIR>::color = 0xFF111111;
template <>
MaterialType Material<MAT_AIR>::type = MAT_AIR;

template <>
Color Material<MAT_SAND>::color = 0xFF47c6d1;
template <>
MaterialType Material<MAT_SAND>::type = MAT_SAND;
template <>
void Material<MAT_SAND>::Update(MaterialType *map, int x, int y)
{
    map[x + y * WIDTH] = MAT_SAND;
    if (y >= (HEIGHT) || y < 0)
    {
        return;
    }
    map[x + y * WIDTH] = MAT_AIR;
    if (map[x + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x + (y - 1) * WIDTH] = MAT_SAND;
    }
    else if (map[x + (y - 1) * WIDTH] == MAT_WATER)
    {
        map[x + y * WIDTH] = MAT_WATER;
        map[x + (y - 1) * WIDTH] = MAT_SAND;
    }
    else if (map[x + 1 + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x + 1 + (y - 1) * WIDTH] = MAT_SAND;
    }
    else if (map[x - 1 + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x - 1 + (y - 1) * WIDTH] = MAT_SAND;
    }
    else
    {
        map[x + (y)*WIDTH] = MAT_SAND;
    }
};

template <>
Color Material<MAT_WATER>::color = 0xFFFF0000;
template <>
MaterialType Material<MAT_WATER>::type = MAT_WATER;
template <>
void Material<MAT_WATER>::Update(MaterialType *map, int x, int y)
{
    map[x + y * WIDTH] = MAT_WATER;
    if (y >= (HEIGHT) || y < 0)
    {
        return;
    }
    map[x + y * WIDTH] = MAT_AIR;
    if (map[x + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x + (y - 1) * WIDTH] = MAT_WATER;
    }
    else if (map[x - 1 + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x - 1 + (y - 1) * WIDTH] = MAT_WATER;
    }
    else if (map[x + 1 + (y - 1) * WIDTH] == MAT_AIR)
    {
        map[x + 1 + (y - 1) * WIDTH] = MAT_WATER;
    }
    else if (map[x + 1 + y * WIDTH] == MAT_AIR)
    {
        map[x + 1 + y * WIDTH] = MAT_WATER;
    }
    else if (map[x - 1 + y * WIDTH] == MAT_AIR)
    {
        map[x - 1 + y * WIDTH] = MAT_WATER;
    }
    else
    {
        map[x + (y)*WIDTH] = MAT_WATER;
    }
};