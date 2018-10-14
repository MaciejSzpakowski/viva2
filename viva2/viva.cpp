/*@ H E A D E R S ******************************************************************************************************@*/
// headers needed in definitions
#include <functional>
#include <algorithm>
#include <future>
#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include <map>
// headers needed in code
#include <fstream>
#include <random>
#include <chrono>
#include <cstring>
// image loading library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// math library
#include "math2.h"
// windows
#define WIN32_LEAN_AND_MEAN
#include <Ws2tcpip.h> // winsock
#include <WinSock2.h> // winsock
#include <Windows.h> // winapi
#include <d3d11.h> // d3d11
#include <d3dcompiler.h> // compile shaders
#include <Xinput.h> // xbox 360/one controller
// link libraries
#pragma comment(lib, "ws2_32.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")
#pragma comment(lib, "Xinput9_1_0.lib")
// compile as windowed app without changing any settings
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")

namespace viva
{
    /*@ U S I N G S   ******************************************************************************************************@*/
    typedef unsigned char byte;
    typedef unsigned int uint;

    using std::vector;

    /*@// F O R W A R D     D E C L A R A T I O N S **************************************************************************@*/
    class Animation;
    class Engine;
    class Surface;
    class PixelShader;
    class Camera;
    class Creator;
    class DrawManager;
    class Window;
    class ResourceManager;
    class RoutineManager;
    class Time;
    class Sprite;
    class Text;

    typedef math::vector Vector;
    typedef math::matrix Matrix;

    namespace input
    {
        class Mouse;
        class Keyboard;
    }

    namespace net
    {
        class NetworkManager;
        class Server;
        class Client;
    }

    namespace ui
    {
        class UIManager;
    }

    extern Engine* engine;
    extern Camera* camera;
    extern Creator* creator;
    extern DrawManager* drawManager;
    extern Window* window;
    extern ResourceManager* resourceManager;
    extern input::Mouse* mouse;
    extern input::Keyboard* keyboard;
    extern RoutineManager* routineManager;
    extern Time* time;
    extern net::NetworkManager* networkManager;
    extern ui::UIManager* uiManager;

    struct D3D11
    {
        ID3D11BlendState* blendState;
        IDXGISwapChain* swapChain;
        ID3D11RenderTargetView* backBuffer;
        ID3D11Device* device;
        ID3D11DeviceContext* context;
        ID3D11InputLayout* layout; //vertex input layout: float[3] pos, float[3] col, float[2] uv
        ID3D11DepthStencilView* depthStencil;
        ID3D11Texture2D* depthStencilBuffer;
        ID3D11RasterizerState* rsSolid;
        ID3D11RasterizerState* rsWire;
        ID3D11VertexShader* defaultVS;
        PixelShader* defaultPS;
        PixelShader* defaultPost;
        ID3D11Buffer* constantBufferVS; // shared cb for worldViewProj matrix
        ID3D11Buffer* constantBufferPS; // shared cb for color
        ID3D11Buffer* constantBufferUV; // shared cb for uv
        ID3D11Buffer* constantBufferPSExtra; // shared cb for user varsiables for ps
        ID3D11Buffer* indexBuffer; // shared for sprites and surfaces
        ID3D11Buffer* vertexBuffer; // shared for sprites
        ID3D11Buffer* vertexBufferSurface; // shared for surfaces
        ID3D11SamplerState* samplerPoint;
        ID3D11SamplerState* samplerLinear;
    };

    extern D3D11 d3d;

    /*@// E N U M S      *****************************************************************************************************@*/
    // xyz 
    enum class CloseReason : int
    {
        // xyz 
        WindowClose = 1,
        // xyz 
        EngineClose
    };

    // xyz 
    enum class TextureFilter
    {
        // xyz 
        Point,
        // xyz 
        Linear
    };

    // xyz 
    enum class TransformMode
    {
        // xyz 
        World,
        // xyz 
        Screen
    };

    namespace input
    {
        // xyz 
        enum class MouseKey : int
        {
            Left = 0x01,
            Right = 0x02,
            Middle = 0x04
        };

        // xyz 
        enum class KeyboardKey : int
        {
            BackSpace = 0x08,
            Tab = 0x09,
            Enter = 0x0D, //both enter and num enter
            Shift = 0x10, //both left and right
            Control = 0x11,//ctrl, both l and r
            Alt = 0x12, //alt, both l and r
            PauseBreak = 0x13, //better dont use this
            CapsLock = 0x14,
            Escape = 0x1B,
            Space = 0x20,
            PageUp = 0x21,
            PageDown = 0x22,
            End = 0x23,
            Home = 0x24,
            Left = 0x25,
            Up = 0x26,
            Right = 0x27,
            Down = 0x28,
            PrintScreen = 0x2C,
            Insert = 0x2D,
            Delete = 0x2E,
            Key0 = '0',
            Key1 = '1',
            Key2 = '2',
            Key3 = '3',
            Key4 = '4',
            Key5 = '5',
            Key6 = '6',
            Key7 = '7',
            Key8 = '8',
            Key9 = '9',
            KeyA = 'A',
            KeyB = 'B',
            KeyC = 'C',
            KeyD = 'D',
            KeyE = 'E',
            KeyF = 'F',
            KeyG = 'G',
            KeyH = 'H',
            KeyI = 'I',
            KeyJ = 'J',
            KeyK = 'K',
            KeyL = 'L',
            KeyM = 'M',
            KeyN = 'N',
            KeyO = 'O',
            KeyP = 'P',
            KeyQ = 'Q',
            KeyR = 'R',
            KeyS = 'S',
            KeyT = 'T',
            KeyU = 'U',
            KeyV = 'V',
            KeyW = 'W',
            KeyX = 'X',
            KeyY = 'Y',
            KeyZ = 'Z',
            WinLeft = 0x5B,
            WinRight = 0x5C,
            Menu = 0x5D,
            RightMenu = 0x5D, //same as menu
            Num0 = 0x60,
            Num1 = 0x61,
            Num2 = 0x62,
            Num3 = 0x63,
            Num4 = 0x64,
            Num5 = 0x65,
            Num6 = 0x66,
            Num7 = 0x67,
            Num8 = 0x68,
            Num9 = 0x69,
            NumMultiply = 0x6A,
            NumPlus = 0x6B,
            NumMinus = 0x6D,
            NumDot = 0x6E,
            NumDivide = 0x6F,
            F1 = 0x70,
            F2 = 0x71,
            F3 = 0x72,
            F4 = 0x73,
            F5 = 0x74,
            F6 = 0x75,
            F7 = 0x76,
            F8 = 0x77,
            F9 = 0x78,
            F10 = 0x79,
            F11 = 0x7A,
            F12 = 0x7B,
            F13 = 0x7C,
            F14 = 0x7D,
            F15 = 0x7E,
            F16 = 0x7F,
            F17 = 0x80,
            F18 = 0x81,
            F19 = 0x82,
            F20 = 0x83,
            F21 = 0x84,
            F22 = 0x85,
            F23 = 0x86,
            F24 = 0x87,
            Numlock = 0x90,
            ScrollLock = 0x91,
            LeftShift = 0xA0,
            RightShift = 0xA1,
            LeftControl = 0xA2,
            RightControl = 0xA3,
            LeftAlt = 0xA4,
            RightAlt = 0xA5,
            Semicolon = 0xBA,
            Plus = 0xBB,
            Equals = 0xBB, //same as Plus
            Comma = 0xBC,
            Minus = 0xBD,
            Period = 0xBE,
            Slash = 0xBF,
            Tilde = 0xC0,
            LeftBracket = 0xDB,
            BackSlash = 0xDC,
            RightBracket = 0xDD,
            Apostrophe = 0xDE,
        };

        // xyz 
        enum class GamepadKey : int
        {
            Up = 1,
            Down = 2,
            Left = 4,
            Right = 8,
            Start = 16,
            Back = 32,
            LeftStick = 64,
            RightStick = 128,
            LB = 256,
            RB = 512,
            _1024 = 1024,
            _2048 = 2048,
            A = 4096,
            B = 8192,
            X = 16384,
            Y = 32768,
        };
    }
}

/*@ Rect ********************************************************************************************************@*/
namespace viva
{
    // xyz
    struct Rect
    {
        float left, top, right, bottom;
        // xyz 
        Rect();
        // xyz 
        Rect(float _left, float _top, float _right, float _bottom);
    };
}

#pragma region code
namespace viva
{
    Rect::Rect()
    {
    }

    Rect::Rect(float _left, float _top, float _right, float _bottom)
        : left(_left), top(_top), right(_right), bottom(_bottom)
    {
    }
}
#pragma endregion

/*@ Action *******************************************************************************************************@*/
namespace viva
{
    // xyz 
    struct Action
    {
        vector<Rect> uvTable;
        double speed;
        int length;

        Action();

        Action(const vector<Rect>& _uvTable, double _speed);
    };
}

#pragma region code
namespace viva
{
    Action::Action()
    {}

    Action::Action(const vector<Rect>& _uvTable, double _speed)
        : uvTable(_uvTable), speed(_speed), length((int)_uvTable.size())
    {
    }
}
#pragma endregion

    /*@/ Point ********************************************************************************************************@*/
namespace viva
{
    // xyz 
    struct Point
    {
        float x, y;
        // xy
        Point();
        // xy
        Point(float x, float y);
    };
}

#pragma region code
namespace viva
{
    Point::Point()
    {
    }

    Point::Point(float x, float y) : x(x), y(y)
    {
    }
}
#pragma endregion

    /*@// Color ********************************************************************************************************@*/
namespace viva
{
    // xyz 
    struct Color
    {
        byte r, g, b, a;

        // color rgba(255,255,255,255)
        Color();
        // each value should be 0 to 1
        Color(byte _r, byte _g, byte _b, byte _a);
    };
}

#pragma region code
namespace viva
{
    Color::Color()
    {
    }

    Color::Color(byte _r, byte _g, byte _b, byte _a) : r(_r), g(_g), b(_b), a(_a)
    {
    }
}
#pragma endregion

/*@// Size *********************************************************************************************************@*/
namespace viva
{
    // xyz 
    struct Size
    {
        float width;
        float height;

        // creates size(0,0)
        Size();
        // xyz 
        Size(float _width, float _height);
    };
}

#pragma region code
namespace viva
{
    Size::Size()
    {
    }

    Size::Size(float _width, float _height) : width(_width), height(_height)
    {
    }
}
#pragma endregion

/*@// Vertex *******************************************************************************************************@*/
namespace viva
{
    // xyz 
    struct Vertex
    {
        float x, y, z, r, g, b, u, v;
        // xyz 
        Vertex();
        // xyz 
        Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v);
    };
}

#pragma region code
namespace viva
{
    Vertex::Vertex()
    {
    }

    Vertex::Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v)
        : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), u(_u), v(_v)
    {
    }
}
#pragma endregion

/*@// Error ********************************************************************************************************@*/
namespace viva
{
    // Viva specific excpetions.
    class Error : public std::runtime_error
    {
    public:
        const char* function;

        // Ctor.
        // fun: should be a function that throws that exception
        // msg: custom message, like what happened ?
        Error(const char* fun, const char* msg);
    };
}

#pragma region code
namespace viva
{
    Error::Error(const char* fun, const char* msg)
        : runtime_error(msg), function(fun)
    {
    }
}
#pragma endregion

/*@// math function ******************************************************************************************************@*/
namespace viva
{
    // xyz
    namespace math
    {
        const float PI = 3.1415926f;
        // xyz 
        float Deg2Rad(float deg);
        // xyz 
        float Rad2Deg(float rad);
        // xyz
        double RandomDouble();
        // xyz
        int RandomInt(int min, int max);
    }
}

#pragma region code
namespace viva
{
    namespace math
    {
        float Deg2Rad(float deg)
        {
            return deg * 2 * PI / 360;
        }

        float Rad2Deg(float rad)
        {
            return rad * 360 / 2 / PI;
        }

        double RandomDouble()
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_real_distribution<> dis(0, 1);
            return dis(gen);
        }

        int RandomInt(int min, int max)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(min, max);
            return dis(gen);
        }
    }
}
#pragma endregion

/*@// util functions ******************************************************************************************************@*/
namespace viva
{
    namespace util
    {
        // Read file contents to ASCII string.
        // filename: path to file
        std::string ReadFileToStringA(const char* filename);

        // Read file to byte vector.
        // dst: destination vector
        void ReadFileToBytes(const char* filename, vector<byte>& dst);

        // Reads image at 'filename' to array of pixels
        // It's 3rd party library written in C, that's why raw array is used.
        // fileparth: filename
        // dst: destination. This function creates a pointer to data and has to write it somewhere.
        Size ReadImageToPixels(const char* filename, Color** dst);

        // Throws exception if hr is erroneous
        // hr: input error code
        // function: name of the function that generated hr
        void Checkhr(HRESULT hr, const char* function);

        ID3D11Buffer* CreateConstantBuffer(UINT size);
    }
}

#pragma region code
namespace viva
{
    namespace util
    {
        std::string ReadFileToStringA(const char* filename)
        {
            std::ifstream file(filename);

            if (!file)
                throw viva::Error("ReadFileToString()", "could not open the file");

            return std::string(std::istreambuf_iterator<char>(file),
                std::istreambuf_iterator<char>());
        }

        void ReadFileToBytes(const char* filename, vector<byte>& dst)
        {
            std::ifstream file(filename, std::ios::binary);

            if (!file)
                throw viva::Error("ReadFileToString()", "could not open the file");

            byte c;
            while (!file.eof())
            {
                c = file.get();
                dst.push_back(c);
            }
            dst.pop_back();
        }

        Size ReadImageToPixels(const char* filename, Color** dst)
        {
            int x = -1, y = -1, n = -1;
            const int components = 4; // components means how many elements from 'RGBA'
                                      // you want to return, I want 4 (RGBA) even in not all 4 are present
            byte* data = stbi_load(filename, &x, &y, &n, components);

            if (data == nullptr)
            {
                std::string msg = "could not load: " + std::string(filename) + " reason: ";
                msg += stbi_failure_reason();
                throw Error(__FUNCTION__, msg.c_str());
            }

            *dst = (Color*)data;

            return{ (float)x,(float)y };
        }

        void Checkhr(HRESULT hr, const char* function)
        {
            if (hr == 0)
                return;

            char str[128];
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
                MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), str, 128, 0);
            std::string message = "";
            message += function + std::string("\n") + str;

            throw viva::Error(function, message.c_str());
        }

        ID3D11Buffer* CreateConstantBuffer(UINT size)
        {
            if (size == 0 || size % 16 != 0)
                throw Error(__FUNCTION__, "size must be multiply of 16");

            ID3D11Buffer* cb;

            D3D11_BUFFER_DESC cbbd;
            ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
            cbbd.Usage = D3D11_USAGE_DEFAULT;
            cbbd.ByteWidth = size;
            cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            cbbd.CPUAccessFlags = 0;
            cbbd.MiscFlags = 0;
            HRESULT hr = d3d.device->CreateBuffer(&cbbd, NULL, &cb);

            return cb;
        }
    }
}
#pragma endregion

/*@ Object Pool @*/
namespace viva
{
    template <typename T>
    class ObjectPool
    {
    private:
        vector<T> pool;
        vector<T*> freeObjects;
    public:
        ObjectPool(int capacity)
        {
            pool.resize(capacity);
            freeObjects.reserve(capacity);

            for (int i = 0; i < capacity; i++)
                freeObjects.push_back(&pool.at(i));
        }

        T* Alloc()
        {
            if (freeObjects.size() == 0)
                return nullptr;

            T* retval = freeObjects.back();
            freeObjects.pop_back();
            return retval;
        }

        void Free(T* ptr)
        {
            freeObjects.push_back(ptr);
        }
    };
}

/*@// Destroyable **********************************************************************************************************@*/
namespace viva
{
    // Tranformable and destroyable.
    class Destroyable
    {
    public:
        // Remove from all managers and memory.
        virtual void Destroy() = 0;
    };
}

#pragma region code
namespace viva
{
}
#pragma endregion

/*@// VertexBuffer *************************************************************************************************@*/
namespace viva
{
    class VertexBuffer : public Destroyable
    {
    protected:
        ID3D11Buffer* vertexBuffer;
        uint vertexCount;
        bool shared;
    public:
        VertexBuffer(ID3D11Buffer* vb, uint vertexCount, bool shared);

        void Destroy();

        int GetVertexCount() const;

        bool IsShared() const;

        ID3D11Buffer** GetVB();
    };
}

#pragma region code
namespace viva
{
    VertexBuffer::VertexBuffer(ID3D11Buffer* vb, uint vertexCount, bool shared)
        : vertexCount(vertexCount), vertexBuffer(vb), shared(shared)
    {
    }

    int VertexBuffer::GetVertexCount() const
    {
        return this->vertexCount;
    }

    bool VertexBuffer::IsShared() const
    {
        return this->shared;
    }

    void VertexBuffer::Destroy()
    {
        this->vertexBuffer->Release();
        delete this;
    }

    ID3D11Buffer** VertexBuffer::GetVB()
    {
        return &(this->vertexBuffer);
    }
}
#pragma endregion

    /*@// Engine *******************************************************************************************************@*/
namespace viva
{
    // 8L Twin Turbo W16
    class Engine
    {
    protected:
        Color backgroundColor;
        Size clientSize;
        long long frame;
    public:
        // Ctor.
        // path: used to load some resources
        // size: viewport size
        Engine(const Size& size);

        void _Activity();
        
        // Completely destroys engine.
        void _Destroy();

        // Start engine (game loop).
        // gameloop: user's function called every frame
        CloseReason Run(const std::function<void()>& gameloop = []() {});

        long long GetFrame() const;

        void GetScreenshot(vector<Color>& dst) const;

        // Get background color. Background color is the color being drawn if there's nothing there.
        const Color& GetBackgroundColor() const;

        // Set background color. Background color is the color being drawn if there's nothing there.
        // color: the color
        void SetBackgroundColor(const Color& color);
        
        // Get viewport/client size. Client is the drawing area in window.
        const Size& GetClientSize() const;
        
        // Stop engine. Makes Run method return.
        void Exit();

        // Open console.
        void OpenConsole();

        // Close console.
        void CloseConsole();
    };
}

#pragma region code
namespace viva
{
    Engine::Engine(const Size& size)
        : backgroundColor(0, 64, 128, 1), clientSize(size), frame(0)
    {
        HRESULT hr = 0;

        const char* strPixelShader = "cbuffer cbBufferPS {float4 color;};"
            "Texture2D ObjTexture;"
            "SamplerState ObjSamplerState;"
            "struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };"
            "float4 main(VS_OUTPUT input):SV_TARGET{"
            "if(input.Col.r == 0){"
            "float4 result = ObjTexture.Sample(ObjSamplerState,input.TexCoord);"
            "clip(result.a-0.001f);"
            "return result*color;"
            "return result;"
            "}"
            "else{"
            "return color;"
            "}"
            "}";

        const char* strVertexShader = "cbuffer cbBufferVS { float4x4 transformation; };"
            "cbuffer cbBufferUV { float4 uv; };"
            "struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };"
            "VS_OUTPUT main(float4 inPos:POSITION, float3 inCol:COLOR, float2 inTexCoord:TEXCOORD){"
            "VS_OUTPUT output;"
            "output.Pos = mul(inPos, transformation);"
            "output.Col = inCol;"
            "output.TexCoord = inTexCoord;"
            "if(inTexCoord[0] == 0 && inTexCoord[1] == 0) output.TexCoord = float2(uv[0], 1 - uv[1]);"
            "if(inTexCoord[0] == 1 && inTexCoord[1] == 0) output.TexCoord = float2(uv[2], 1 - uv[1]);"
            "if(inTexCoord[0] == 0 && inTexCoord[1] == 1) output.TexCoord = float2(uv[0], 1 - uv[3]);"
            "if(inTexCoord[0] == 1 && inTexCoord[1] == 1) output.TexCoord = float2(uv[2], 1 - uv[3]);"
            "return output;}";

        const char* strPostShader = "cbuffer cbBufferPS{};"
            "Texture2D ObjTexture;"
            "SamplerState ObjSamplerState;"
            "struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };"
            "float4 main(VS_OUTPUT input):SV_TARGET{"
            "float4 result = ObjTexture.Sample(ObjSamplerState, input.TexCoord);"
            "clip(result.a-0.001f);"
            "return result;}";

        //    DEVICE, DEVICE CONTEXT AND SWAP CHAIN    ////
        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
        scd.BufferCount = 1;                                    // one back buffer
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
        scd.OutputWindow = (HWND)window->GetHandle();     // the window to be used
        scd.SampleDesc.Quality = 0;
        scd.SampleDesc.Count = 1;                               // no anti aliasing
        scd.Windowed = TRUE;                                    // windowed/full-screen mode
                                                                //scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;   // alternative fullscreen mode

        hr = D3D11CreateDeviceAndSwapChain(NULL,
            D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
            D3D11_SDK_VERSION, &scd, &d3d.swapChain, &d3d.device, NULL,
            &d3d.context);
        util::Checkhr(hr, "D3D11CreateDeviceAndSwapChain()");

        ////    BACK BUFFER AS RENDER TARGET, DEPTH STENCIL   ////
        ID3D11Texture2D* buf;
        d3d.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&buf);
        // use the back buffer address to create the render target
        hr = d3d.device->CreateRenderTargetView(buf, NULL, &d3d.backBuffer);
        util::Checkhr(hr, "CreateRenderTargetView()");
        buf->Release();

        D3D11_TEXTURE2D_DESC depthStencilDesc;
        depthStencilDesc.Width = (UINT)clientSize.width;
        depthStencilDesc.Height = (UINT)clientSize.height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1; // ANTIALIASING, increase to 2
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;

        hr = d3d.device->CreateTexture2D(&depthStencilDesc, NULL, &d3d.depthStencilBuffer);
        util::Checkhr(hr, "CreateTexture2D() for depthStencilDesc");
        hr = d3d.device->CreateDepthStencilView(d3d.depthStencilBuffer, NULL, &d3d.depthStencil);
        util::Checkhr(hr, "CreateDepthStencilView()");

        ////   VIEWPORT    ////
        // Set the viewport
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)clientSize.width;
        viewport.Height = (float)clientSize.height;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        d3d.context->RSSetViewports(1, &viewport);

        ////    VS   ////
        ID3D10Blob *vs; //release vs after CreateInputLayout()
                        //alternative to loading shader from cso file
        hr = D3DCompile(strVertexShader, strlen(strVertexShader), 0, 0, 0, "main", "vs_5_0", D3DCOMPILE_DEBUG, 0, &vs, 0);
        util::Checkhr(hr, "D3DCompile() vs");
        hr = d3d.device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), 0,
            &d3d.defaultVS);
        util::Checkhr(hr, "CreateVertexShader()");
        d3d.context->VSSetShader(d3d.defaultVS, 0, 0);
        // TODO: why is this here ?
        util::Checkhr(hr, "CreateVertexShader()");

        //    INPUT LAYOUT    ////
        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
            0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,
            0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,
            0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            //if you need to pass something on your own to PS or VS per vertex
            //{ "SOME_MORE_DATA", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        hr = d3d.device->CreateInputLayout(ied, 3, vs->GetBufferPointer(), vs->GetBufferSize(),
            &d3d.layout);
        util::Checkhr(hr, "CreateInputLayout()");
        vs->Release();
        d3d.context->IASetInputLayout(d3d.layout);

        ///    BLEND STATE    ////
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(blendDesc));
        D3D11_RENDER_TARGET_BLEND_DESC rtbd;
        ZeroMemory(&rtbd, sizeof(rtbd));
        rtbd.BlendEnable = true;
        rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        rtbd.BlendOp = D3D11_BLEND_OP_ADD;
        rtbd.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;// D3D11_BLEND_ONE;
        rtbd.DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO;
        rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0] = rtbd;
        hr = d3d.device->CreateBlendState(&blendDesc, &d3d.blendState);
        util::Checkhr(hr, "CreateBlendState()");

        ////    RASTERIZERS     ////
        D3D11_RASTERIZER_DESC rd;
        ZeroMemory(&rd, sizeof(rd));
        rd.FillMode = D3D11_FILL_WIREFRAME;
        rd.CullMode = D3D11_CULL_NONE;
        hr = d3d.device->CreateRasterizerState(&rd, &d3d.rsWire);
        util::Checkhr(hr, "CreateRasterizerState()");
        rd.FillMode = D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_FRONT;
        //rd.AntialiasedLineEnable = true; // ANTIALIASING
        //rd.MultisampleEnable = true; // ANTIALIASING
        hr = d3d.device->CreateRasterizerState(&rd, &d3d.rsSolid);
        util::Checkhr(hr, "CreateRasterizerState()");

        ////    SAMPLERS    //////
        D3D11_SAMPLER_DESC sampDesc;
        ZeroMemory(&sampDesc, sizeof(sampDesc));
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        d3d.device->CreateSamplerState(&sampDesc, &d3d.samplerPoint);
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        d3d.device->CreateSamplerState(&sampDesc, &d3d.samplerLinear);

        //// SO FAR ONLY INDEX BUFFER ////
        vector<int> indices({ 0, 1, 2, 0, 2, 3, });

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(int) * (UINT)indices.size();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA srd;
        srd.pSysMem = indices.data();
        d3d.device->CreateBuffer(&indexBufferDesc, &srd, &d3d.indexBuffer);
        d3d.context->IASetIndexBuffer(d3d.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
        
        //////   PS    ///////
        d3d.defaultPS = creator->CreatePixelShader(strPixelShader);
        d3d.defaultPost = creator->CreatePixelShader(strPostShader);

        /////// CONSTANT BUFFERS ///////
        d3d.constantBufferVS = util::CreateConstantBuffer(sizeof(Matrix));
        d3d.context->VSSetConstantBuffers(0, 1, &d3d.constantBufferVS);

        d3d.constantBufferUV = util::CreateConstantBuffer(sizeof(Rect));
        d3d.context->VSSetConstantBuffers(1, 1, &d3d.constantBufferUV);

        d3d.constantBufferPS = util::CreateConstantBuffer(16);
        d3d.context->PSSetConstantBuffers(0, 1, &d3d.constantBufferPS);

        // NOTES
        // there's one extra buffer for the entire project
        // it meamns it might be updated for every many objects
        // it's fine because if object had separated buffers, they all would be updated
        // one optimization might be to have a separate buffer if it has a lot of data that is not updated frequently
        d3d.constantBufferPSExtra = util::CreateConstantBuffer(16);
        d3d.context->PSSetConstantBuffers(1, 1, &d3d.constantBufferPSExtra);

        /////// SQUARE VERTEX BUFFER //////
        vector<Vertex> verticesSprite({
            Vertex(0, 0, 0, 0, 0, 0, 0, 1),
            Vertex(1, 0, 0, 0, 0, 0, 1, 1),
            Vertex(1, 1, 0, 0, 0, 0, 1, 0),
            Vertex(0, 1, 0, 0, 0, 0, 0, 0) });

        vector<Vertex> verticesSurface({
            Vertex(-1, -1, 0, 0, 0, 0, 0, 1),
            Vertex(1, -1, 0, 0, 0, 0, 1, 1),
            Vertex(1, 1, 0, 0, 0, 0, 1, 0),
            Vertex(-1, 1, 0, 0, 0, 0, 0, 0) });

        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA vertexBufferData;
        ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
        vertexBufferData.pSysMem = verticesSprite.data();
        d3d.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d3d.vertexBuffer);

        vertexBufferData.pSysMem = verticesSurface.data();
        d3d.device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d3d.vertexBufferSurface);
    }

    void Engine::_Activity()
    {
        this->frame++;

        // camear
        camera->_Activity();

        // time
        time->_Activity();

        // events
        routineManager->_Activity();

        // input
        mouse->_Activity();
        keyboard->_Activity();

        // render
        drawManager->_DrawNodes();

        // this snippet is here because
        // also this is common for all render targets, I need to set this up only once
        float col[4] = { this->backgroundColor.r / 255.0f,this->backgroundColor.g / 255.0f,this->backgroundColor.b / 255.0f,this->backgroundColor.a / 255.0f };
        d3d.context->ClearRenderTargetView(d3d.backBuffer, col);
        d3d.context->ClearDepthStencilView(d3d.depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        d3d.context->OMSetRenderTargets(1, &d3d.backBuffer, d3d.depthStencil);
        d3d.context->RSSetState(d3d.rsSolid);
        d3d.context->PSSetSamplers(0, 1, &d3d.samplerPoint);
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        d3d.context->IASetVertexBuffers(0, 1, &d3d.vertexBufferSurface, &stride, &offset);
        d3d.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        float identityMatrix[] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
        Rect surfaceuv(0, 0, 1, 1);
        d3d.context->UpdateSubresource(d3d.constantBufferUV, 0, 0, &surfaceuv, 0, 0);
        d3d.context->UpdateSubresource(d3d.constantBufferVS, 0, NULL, identityMatrix, 0, 0);
        
        drawManager->_DrawSurfaces();

        d3d.swapChain->Present(0, 0);
    }

    long long Engine::GetFrame() const
    {
        return this->frame;
    }

    // Get background color. Background color is the color being drawn if there's nothing there.
    const Color& Engine::GetBackgroundColor() const
    {
        return this->backgroundColor;
    }

    // Set background color. Background color is the color being drawn if there's nothing there.
    // color: the color
    void Engine::SetBackgroundColor(const Color& color)
    {
        this->backgroundColor = color;
    }

    // Get viewport/client size. Client is the drawing area in window.
    const Size& Engine::GetClientSize() const
    {
        return this->clientSize;
    }

    void Engine::GetScreenshot(vector<Color>& dst) const
    {
        RECT r;
        GetClientRect(window->GetHandle(), &r);
        POINT p = { 0,0 };
        ClientToScreen(window->GetHandle(), &p);
        int w = r.right - r.left;
        int h = r.bottom - r.top;
        dst.resize(w*h * 4);

        HDC primaryScreenDC = GetDC(NULL);
        HDC desktopDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
        HDC captureDC = CreateCompatibleDC(desktopDC);
        HBITMAP bmp = CreateCompatibleBitmap(desktopDC, w, h);
        SelectObject(captureDC, bmp);
        BitBlt(captureDC, 0, 0, w, h, desktopDC, p.x, p.y, SRCCOPY);
        GetBitmapBits(bmp, w*h * 4, dst.data());
    }

    void Engine::_Destroy()
    {
        // destroy objects
        d3d.defaultPS->Destroy();
        d3d.defaultPost->Destroy();

        // release interfaces
        d3d.constantBufferPS->Release();
        d3d.constantBufferPSExtra->Release();
        d3d.constantBufferUV->Release();
        d3d.constantBufferVS->Release();
        d3d.vertexBuffer->Release();
        d3d.vertexBufferSurface->Release();
        d3d.samplerLinear->Release();
        d3d.samplerPoint->Release();
        d3d.indexBuffer->Release();
        d3d.blendState->Release();
        d3d.rsSolid->Release();
        d3d.rsWire->Release();
        d3d.layout->Release();
        d3d.defaultVS->Release();
        d3d.depthStencilBuffer->Release();
        d3d.depthStencil->Release();
        d3d.backBuffer->Release();
        d3d.swapChain->Release();
        d3d.context->Release();
        d3d.device->Release();

        delete this;
    }

    void Engine::OpenConsole()
    {
        ::AllocConsole();
        ::SetConsoleTitle("Console");
        ::freopen("CONOUT$", "w", stdout);
        ::freopen("CONIN$", "r", stdin);
    }

    void Engine::CloseConsole()
    {
        ::FreeConsole();
    }

    void Engine::Exit()
    {
        ::PostMessage(window->GetHandle(), WM_CLOSE, (int)CloseReason::EngineClose, 0);
    }

    CloseReason Engine::Run(const std::function<void()>& gameloop)
    {
        return window->Run(gameloop, [&]() {this->_Activity(); });
    }
}
#pragma endregion

    /*@// Time *********************************************************************************************************@*/
namespace viva
{
    class Time
    {
    protected:
        double gameTime;
        double frameTime;
        double frequency;
        long long startTime;
        long long prevFrameTime;
    public:
        Time();

        void _Activity();

        // Time elapsed in seconds since the beginning of the game.
        double GetGameTime() const;

        // Time elapsed since last frame.
        double GetFrameTime() const;

        // Frames per second.
        double GetFps() const;

        void _Destroy();
    };
}

#pragma region code
namespace viva
{
    Time::Time() : gameTime(0), frameTime(0)
    {
        LARGE_INTEGER li;
        if (!::QueryPerformanceFrequency(&li))
            throw Error(__FUNCTION__, "QueryPerformanceFrequency() failed");
        this->frequency = double(li.QuadPart);
        ::QueryPerformanceCounter(&li);
        this->startTime = li.QuadPart;
        this->prevFrameTime = startTime;
    }

    void Time::_Activity()
    {
        LARGE_INTEGER currentTime;
        long long frameTickCount;
        ::QueryPerformanceCounter(&currentTime);
        frameTickCount = currentTime.QuadPart - this->prevFrameTime;
        this->frameTime = double(frameTickCount) / this->frequency;
        this->prevFrameTime = currentTime.QuadPart;
        this->gameTime = double(currentTime.QuadPart - this->startTime) / this->frequency;
    }

    double Time::GetGameTime() const
    {
        return this->gameTime;
    }

    double Time::GetFrameTime() const
    {
        return this->frameTime;
    }

    // Frames per second.
    double Time::GetFps() const
    {
        return 1 / this->frameTime;
    }

    void Time::_Destroy()
    {
        delete this;
    }
}
#pragma endregion

    /*@// Transform ****************************************************************************************************@*/
namespace viva
{
    // Transform object responsible for position, rotation, scale etc. of its owner.
    class Transform
    {
    private:
        Transform* parent;
        vector<Transform*> children;
        uint index; // index in parents collection
        TransformMode mode;

        Vector origin;
        Vector position;
        float rotation;
        Vector scale;
        float size;

        Vector velocity;
        float angularVelocity;
        Vector scaleVelocity;
        float sizeVelocity;

        Vector acceleration;
        float angularAcceleration;
        Vector scaleAcceleration;
        float sizeAcceleration;

        Vector absolutePosition;
        Vector abosulteScale;
        float absoluteRotation;
    public:
        // Ctor.
        Transform();

        Transform* SetCoordMode(TransformMode m);

        TransformMode GetMode() const;

        // Converts rotation, scale, position and parent relationship to matrix transformation.
        Matrix GetWorld();

        Matrix GetWorldViewProj();

        Matrix GetWorldScreen();

        // Get/Set origin
        Vector& Origin();

        // Get/Set position
        Vector& Pos();

        // Get/Set velocity
        Vector& Vel();

        // Get/Set acceleration
        Vector& Acc();
        
        // Get/Set rotation
        float& Rot();

        // Get/Set angular velocity
        float& RotVel();

        // Get/Set angular acceleration
        float& RotAcc();

        // Get/Set scale
        Vector& Scale();

        // Get/Set scale velocity
        Vector& ScaleVel();

        // Get/Set scale acceleration
        Vector& ScaleAcc();

        // Set scale to match pixel size.
        // _size: desired size in pixels
        Transform* SetPixelScale(const Size& size);

        // Set scale to match pixel size.
        // width: width in pixels
        // height: height in pixels
        Transform* SetPixelScale(float width, float height);

        /////     SIZE     //////
        // Get/Set size
        float& Size();

        // Get/Set size velocity
        float& SizeVel();

        // Get/Set size acceleration
        float& SizeAcc();

        Transform* SetParent(Transform* p);

        Transform* GetParent() const;

        Transform* RemoveChild(Transform* child);

        void _Update();
    };
}

#pragma region code
namespace viva
{
    // Ctor.
    Transform::Transform()
        : parent(nullptr), index(-1), mode(TransformMode::World), position(Vector(0, 0, 0, 1)), rotation(0), scale(Vector(1, 1, 1, 1)), size(1)
    {
    }

    Transform* Transform::SetCoordMode(TransformMode m)
    {
        this->mode = m;
        return this;
    }

    TransformMode Transform::GetMode() const
    {
        return this->mode;
    }

    // Converts rotation, scale, position and parent relationship to matrix transformation.
    Matrix Transform::GetWorld()
    {
        Matrix world = math::matrix::translate_negy(this->origin) *
            math::matrix::scale(this->scale) *
            math::matrix::rotate(this->rotation) *
            math::matrix::translate_negy(this->position);

        if (this->parent != nullptr)
        {
            Matrix parentRotLoc = math::matrix::rotate(this->parent->absoluteRotation) * 
                math::matrix::translate_negy(this->parent->absolutePosition);
            absolutePosition = parentRotLoc.transpose() * this->position;
            absoluteRotation = rotation + this->parent->absoluteRotation;
            world = world * parentRotLoc;
        }
        else
        {
            this->absolutePosition = this->position;
            this->absoluteRotation = this->rotation;
        }

        return world;
    }

    Matrix Transform::GetWorldViewProj()
    {
        if (mode == TransformMode::World)
        {
            return this->GetWorld() * camera->_GetScaLoc();
        }
        else
        {
            return this->GetWorldScreen() * camera->_GetSca();
        }
    }

    Matrix Transform::GetWorldScreen()
    {
        auto& frustumSize = camera->GetFrustumSize();
        auto __scale = camera->Pixel2World({ this->scale.x, this->scale.y });
        auto pos = camera->Pixel2World({ this->position.x, this->position.y });
        Vector _origin(this->origin.x, this->origin.y);
        Vector _scale(__scale.width, __scale.height);
        Vector _translate(pos.width - frustumSize.width / 2, -pos.height + frustumSize.height/2, this->position.z);

        Matrix world = math::matrix::translate_negy(_origin) *
            math::matrix::scale(_scale) *
            math::matrix::rotate(this->rotation) *
            math::matrix::translate_negy(_translate);

        return world;
    }

    // Get/Set origin
    Vector& Transform::Origin()
    {
        return this->origin;
    }

    // Get/Set position
    Vector& Transform::Pos()
    {
        return this->position;
    }

    // Get/Set velocity
    Vector& Transform::Vel()
    {
        return this->velocity;
    }

    // Get/Set acceleration
    Vector& Transform::Acc()
    {
        return this->acceleration;
    }

    // Get/Set rotation
    float& Transform::Rot()
    {
        return this->rotation;
    }

    // Get/Set angular velocity
    float& Transform::RotVel()
    {
        return this->angularVelocity;
    }

    // Get/Set angular acceleration
    float& Transform::RotAcc()
    {
        return this->angularAcceleration;
    }

    // Get/Set scale
    Vector& Transform::Scale()
    {
        return this->scale;
    }

    // Get/Set scale velocity
    Vector& Transform::ScaleVel()
    {
        return this->scaleVelocity;
    }

    // Get/Set scale acceleration
    Vector& Transform::ScaleAcc()
    {
        return this->scaleAcceleration;
    }

    // Get/Set size
    float& Transform::Size()
    {
        return this->size;
    }

    // Get/Set size velocity
    float& Transform::SizeVel()
    {
        return this->sizeVelocity;
    }

    // Get/Set size acceleration
    float& Transform::SizeAcc()
    {
        return this->sizeAcceleration;
    }
    
    Transform* Transform::SetPixelScale(float width, float height)
    {
        return this->SetPixelScale(viva::Size(width, height));
    }

    Transform* Transform::SetPixelScale(const viva::Size& size)
    {
        viva::Size s = camera->Pixel2World(size);
        this->scale.x = s.width;
        this->scale.y = s.height;
        //Size frustum = camera->GetFrustumSize(transform.GetPosition().f.z);
        //Size client = engine->GetClientSize();
        //Point unitsPerPixel = { frustum.Width / client.Width, frustum.Height / client.Height };
        //scale.x = unitsPerPixel.x * _size.width / 2;
        //scale.y = unitsPerPixel.y * _size.height / 2;

        //transform.SetScale(frustum.Width / client.Width * size.Width / 2,
        //    frustum.Height / client.Height * size.Height / 2);

        return this;
    }

    Transform* Transform::SetParent(Transform* p)
    {
        if (index != -1)
            throw Error(__FUNCTION__, "Object can have only one parent");

        this->parent = p;
        this->index = (int)this->parent->children.size();
        this->parent->children.push_back(this);

        return this;
    }

    Transform* Transform::GetParent() const
    {
        return this->parent;
    }

    Transform* Transform::RemoveChild(Transform* child)
    {
        if (child->index == -1)
            return this;

        if (this->children.at(child->index) != child)
            throw Error(__FUNCTION__, "Wrong index");

        this->children.erase(this->children.begin() + child->index);
        child->index = -1;
        child->parent = nullptr;

        return this;
    }

    void Transform::_Update()
    {
        float frameTime = (float)time->GetFrameTime();
        // v += a
        this->velocity += this->acceleration * frameTime;
        // x += v
        this->position += this->velocity * frameTime;
        // rv += ra
        this->angularVelocity += this->angularAcceleration * frameTime;
        // r += rv
        this->rotation += this->angularVelocity * frameTime;

        this->scaleVelocity += this->scaleAcceleration * frameTime;
        this->scale += this->scaleVelocity * frameTime;

        this->sizeVelocity += this->sizeAcceleration * frameTime;
        this->size += this->sizeVelocity * frameTime;
    }
}
#pragma endregion

    /*@// Drawable *******************************************************************************************************@*/
namespace viva
{
    class Drawable : public Destroyable
    {
    protected:
        Color color;
        void* extraBufferPSdata;
    public:
        Drawable();

        // Draw all objects frm this collection.
        virtual void _Draw() = 0;

        virtual Surface* GetSurface() const = 0;

        virtual bool IsVisible() const = 0;

        virtual Drawable* SetVisible(bool val) = 0;

        virtual void _SetSurface(Surface* surface) = 0;

        virtual void _SetIndex(uint i) = 0;

        virtual int _GetIndex() const = 0;

        const Color& GetColor() const;

        Drawable* SetColor(byte r, byte g, byte b, byte a);

        Drawable* SetColor(const Color& c);

        Drawable* SetR(byte r);

        Drawable* SetG(byte g);

        Drawable* SetB(byte b);

        Drawable* SetA(byte a);

        void SetExtraBufferPSdata(void* data);
    };
}

#pragma region code
namespace viva
{
    Drawable::Drawable() :extraBufferPSdata(nullptr)
    {
    }

    void Drawable::SetExtraBufferPSdata(void* data)
    {
        extraBufferPSdata = data; 
    }

    const Color& Drawable::GetColor() const
    {
        return this->color;
    }

    Drawable* Drawable::SetColor(byte r, byte g, byte b, byte a)
    {
        this->color.a = a;
        this->color.b = b;
        this->color.g = g;
        this->color.r = r;

        return this;
    }

    Drawable* Drawable::SetColor(const Color& c)
    {
        this->color = c;
        return this;
    }

    Drawable* Drawable::SetR(byte r)
    {
        this->color.r = r;
        return this;
    }

    Drawable* Drawable::SetG(byte g)
    {
        this->color.g = g;
        return this;
    }

    Drawable* Drawable::SetB(byte b)
    {
        this->color.b = b;
        return this;
    }

    Drawable* Drawable::SetA(byte a)
    {
        this->color.a = a;
        return this;
    }
}
#pragma endregion

    /*@// Surface ********************************************************************************************************@*/
namespace viva
{
    // Special drawable where all of its objects are drawn on it.
    class Surface : public Destroyable
    {
    protected:
        //PixelShader* pixelShader;
        vector<Drawable*> drawables;
        PixelShader* ps;
        ID3D11Texture2D* tex;
        ID3D11RenderTargetView* rtv;
        ID3D11ShaderResourceView* srv;
        void* extraBufferPSdata;
    public:
        Surface(ID3D11Texture2D* t, ID3D11RenderTargetView* r,
            ID3D11ShaderResourceView* s);

        // 
        void _DrawAll();

        // Draw surface itself.
        void _DrawSurface();

        // Get pixel shader.
        PixelShader* GetPixelShader() const;

        // Set pixel shader.
        // ps: ps
        void SetPixelShader(PixelShader* ps);

        // Add drawable into this surface.
        // d: drawable to add
        void Add(Drawable* d);

        // Remove all drawables from the surface without destroying them.
        void RemoveAll();

        //
        void Remove(Drawable* d);

        // Destroy all drawables from this surface.
        void Clear();

        // Destroy the surface and all drawables it contains.
        void Destroy();

        void SetExtraBufferPSdata(void* data);
    };
}

#pragma region code
namespace viva
{
    Surface::Surface(ID3D11Texture2D* t, ID3D11RenderTargetView* r,
        ID3D11ShaderResourceView* s)
        : tex(t), rtv(r), srv(s), extraBufferPSdata(nullptr)
    {
    }

    void Surface::SetExtraBufferPSdata(void* data)
    {
        this->extraBufferPSdata = data;
    }

    void Surface::Add(Drawable* d)
    {
        d->_SetIndex((int)drawables.size());
        d->_SetSurface(this);
        this->drawables.push_back(d);
    }

    // Remove all drawables from the surface without destroying them.
    void Surface::RemoveAll()
    {
        for (uint i = 0; i < this->drawables.size(); i++)
        {
            this->drawables.at(i)->_SetIndex(-1);
            this->drawables.at(i)->_SetSurface(nullptr);
        }

        this->drawables.clear();
    }

    //
    void Surface::Remove(Drawable* d)
    {
        int indexOfd = d->_GetIndex();

        if (indexOfd == -1)
            return;

        // if d is not at the end
        if (indexOfd != this->drawables.size() - 1)
        {
            // move back to where d is and update its index
            this->drawables.back()->_SetIndex(indexOfd);
            this->drawables.at(indexOfd) = this->drawables.back();
        }

        this->drawables.pop_back();
        d->_SetIndex(-1);
        d->_SetSurface(nullptr);
    }

    // Destroy all drawables from this surface.
    void Surface::Clear()
    {
        for (uint i = 0; i < this->drawables.size(); i++)
            this->drawables.at(i)->Destroy();

        this->drawables.clear();
    }

    PixelShader* Surface::GetPixelShader() const
    {
        return this->ps;
    }

    void Surface::SetPixelShader(PixelShader* ps)
    {
        this->ps = ps;
    }

    void Surface::_DrawAll()
    {
        d3d.context->ClearDepthStencilView(d3d.depthStencil,
            D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
        //if (Core->IsAlphaEnabled())
        //   Core->_GetContext()->OMSetBlendState(Core->_GetBlendState(), 0, 0xffffffff);

        d3d.context->OMSetRenderTargets(1, &(this->rtv), d3d.depthStencil);
        float four0[4] = { 0, 0, 0, 0 };
        d3d.context->ClearRenderTargetView(this->rtv, four0);

        for (int i = 0; i < this->drawables.size(); i++)
            this->drawables.at(i)->_Draw();
    }

    void Surface::_DrawSurface()
    {
        //extra buffer
        if (this->extraBufferPSdata != nullptr)
            d3d.context->UpdateSubresource(d3d.constantBufferPSExtra, 0, 0, this->extraBufferPSdata, 0, 0);

        d3d.context->PSSetShader(this->ps->GetPS(), 0, 0);
        //tex
        d3d.context->PSSetShaderResources(0, 1, &(this->srv));
        //draw
        d3d.context->DrawIndexed(6, 0, 0);
    }

    void Surface::Destroy()
    {
        this->Clear();

        this->tex->Release();
        this->rtv->Release();
        this->srv->Release();
    }
}
#pragma endregion

    /*@// Polygon ********************************************************************************************************@*/
namespace viva
{
    // Drawable that consist of lines.
    class Polygon : public Drawable
    {
    protected:
        //float span; //distance from origin to the furthest vertex
        uint vertexCount;
        Transform transform;
        Surface* parent;
        uint index;
        bool visible;
        VertexBuffer* vertexBuffer;
        PixelShader* ps;
    public:
        // Ctor.
        // count: vertex count
        Polygon(VertexBuffer* vb);

        // Get transform of the object.
        Transform* T();

        // Get transform of the object.
        Transform* GetTransform();

        //XMMATRIX _GetScaleMatrix() override;

        //float GetSpan() const { return span; }

        //make exact (but independent) copy and add to draw manager
        //stuff that is not copied: children (since they can have only one parent)
        //virtual Polygon* Clone();

        // Get pixel shader.
        PixelShader* GetPixelShader() const;

        // Set pixel shader.
        // ps: the ps
        void SetPixelShader(PixelShader* ps);

        // Get vertex count.
        uint GetVertexCount() const;

        Surface* GetSurface() const override;

        bool IsVisible() const override;

        Drawable* SetVisible(bool val) override;

        void _SetSurface(Surface* surface) override;

        void _SetIndex(uint i) override;

        int _GetIndex() const override;

        void _Draw() override;

        void Destroy() override;
    };
}

#pragma region code
namespace viva
{
    Polygon::Polygon(VertexBuffer* vb)
        : parent(nullptr), index(-1), visible(true), vertexBuffer(vb), 
        vertexCount(vb->GetVertexCount()), ps(d3d.defaultPS)
    {
    }

    // Get transform of the object.
    Transform* Polygon::T()
    {
        return &(this->transform);
    }

    // Get transform of the object.
    Transform* Polygon::GetTransform()
    {
        return &(this->transform);
    }

    //XMMATRIX _GetScaleMatrix() override;

    //float GetSpan() const { return span; }

    //make exact (but independent) copy and add to draw manager
    //stuff that is not copied: children (since they can have only one parent)
    //virtual Polygon* Clone();
    
    // Get vertex count.
    uint Polygon::GetVertexCount() const
    {
        return this->vertexCount;
    }

    Surface* Polygon::GetSurface() const
    {
        return this->parent;
    }

    bool Polygon::IsVisible() const
    {
        return this->visible;
    }

    Drawable* Polygon::SetVisible(bool val)
    {
        this->visible = val;
        return this;
    }

    void Polygon::_SetSurface(Surface* surface)
    {
        this->parent = surface;
    }

    void Polygon::_SetIndex(uint i)
    {
        this->index = i;
    }

    int Polygon::_GetIndex() const
    {
        return this->index;
    }

    PixelShader* Polygon::GetPixelShader() const
    {
        return this->ps;
    }

    void Polygon::SetPixelShader(PixelShader* _ps)
    {
        this->ps = _ps;
    }

    void Polygon::_Draw()
    {
        this->T()->_Update();

        if (!this->visible)
            return;

        // transform
        Matrix matT = this->transform.GetWorldViewProj().transpose();
        d3d.context->UpdateSubresource(d3d.constantBufferVS, 0, NULL, &matT, 0, 0);
        // color
        float fColor[] = { this->color.r / 255.0f,this->color.g / 255.0f,this->color.b / 255.0f,this->color.a / 255.0f };
        d3d.context->UpdateSubresource(d3d.constantBufferPS, 0, 0, fColor, 0, 0);
        d3d.context->PSSetShader(ps->GetPS(), 0, 0);
        d3d.context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);
        d3d.context->RSSetState(d3d.rsWire);
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        d3d.context->IASetVertexBuffers(0, 1, this->vertexBuffer->GetVB(), &stride, &offset);

        d3d.context->Draw(this->vertexCount, 0);
    }

    void Polygon::Destroy()
    {
        if (!this->vertexBuffer->IsShared())
            this->vertexBuffer->Destroy();

        if (this->index != -1)
            this->parent->Remove(this);

        delete this;
    }
}
#pragma endregion

    /*@// Texture ********************************************************************************************************@*/
namespace viva
{
    class Texture : public Destroyable
    {
    protected:
        ID3D11ShaderResourceView* shaderResource;
        Size size;
    public:
        Texture(ID3D11ShaderResourceView* srv, const Size& size);

        const Size& GetSize() const;

        void Destroy();

        ID3D11ShaderResourceView** GetSRV();
    };
}

#pragma region code
namespace viva
{
    Texture::Texture(ID3D11ShaderResourceView* srv, const Size& size)
        : shaderResource(srv), size(size)
    {
    }

    const Size& Texture::GetSize() const
    {
        return this->size;
    }

    void Texture::Destroy()
    {
        this->shaderResource->Release();
        delete this;
    }

    ID3D11ShaderResourceView** Texture::GetSRV()
    {
        return &(this->shaderResource);
    }
}
#pragma endregion

    /*@// Font ***********************************************************************************************************@*/
namespace viva
{
    struct CharacterMetrics
    {
        int id;
        Rect uv;
        Size size;
        Size sizePx;
        Point offset;
        Point offsetPx;
        float advance;
        float advancePx;
    };

    struct FontMetrics
    {
        float lineHeight;
        float lineHeightPx;
    };

    // Bitmap font. Stores coordinates for where letters are on texture
    class Font : public Destroyable
    {
    private:
        Texture* texture;
        vector<CharacterMetrics> characters;
        FontMetrics fontMetrics;

        void InitFontFromMetrics(const char* fontMetrics);
    public:
        // Create bitmap font from texture. And calc primitive metrics
        // tex: texture to use
        // glyphs: exact uv coordinate for glyphs. It should contain at least ascii 0-126
        Font(Texture* tex, const char* fontMetrics, bool fromString);

        // Create bitmap font from texture.
        Font(Texture* tex, const char* fontMetricsFile);

        // Gets uv coordinate for char 'code'.
        const CharacterMetrics& GetChar(uint code) const;

        const FontMetrics& GetFontMetrics() const;
        
        Texture* GetTexture() const;

        void Destroy();
    };
}

#pragma region code
namespace viva
{
    Font::Font(Texture* tex, const char* fontMetrics, bool fromString)
        : texture(tex)
    {
        this->InitFontFromMetrics(fontMetrics);
    }

    void Font::InitFontFromMetrics(const char* fontMetrics)
    {
        // if string is null or empty
        if (!fontMetrics || !*fontMetrics)
            return;

        int len = (int)strlen(fontMetrics);
        const char* eos = fontMetrics + len;
        const char* it = fontMetrics;
        char buf[500];
        CharacterMetrics cm;
        float lineHeight;
        const Size& texSize = this->texture->GetSize();
        int id, x, y, width, height, xoffset, yoffset, xadvance, maxId = 0;
        std::vector<CharacterMetrics> cms;

        while (it < eos)
        {
            // getline
            const char* eol = strchr(it, '\n');
            if (!eol)
                eol = eos;

            memset(buf, 0, 500);
            memcpy(buf, it, eol - it);
            it = eol + 1;

            // process line
            if (!memcmp(buf, "info", 4))
            {
            }
            else if (!memcmp(buf, "common", 6))
            {
                sscanf(buf, "common lineHeight=%f", &lineHeight);
                this->fontMetrics.lineHeight = camera->Pixel2World({ 0 ,lineHeight }).height;
                this->fontMetrics.lineHeightPx = lineHeight;
            }
            else if (!memcmp(buf, "char ", 5))
            {
                sscanf(buf, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d",
                    &id, &x, &y, &width, &height, &xoffset, &yoffset, &xadvance);

                cm.advance = camera->Pixel2World({ (float)xadvance ,0 }).width;
                cm.advancePx = (float)xadvance;
                cm.id = id;
                auto offset = camera->Pixel2World({ (float)xoffset ,(float)yoffset });
                cm.offset = { offset.width, offset.height };
                cm.offsetPx = { (float)xoffset , -(float)yoffset };
                cm.size = camera->Pixel2World({ (float)width ,(float)height });
                cm.sizePx = { (float)width ,(float)height };
                // TODO why (top = 1 - bottom) and (bottom = 1 - top)
                cm.uv = {
                    x / texSize.width,
                    1 - (y + height) / texSize.height,
                    (x + width) / texSize.width,
                    1 - y / texSize.height,
                };

                if (id > maxId)
                    maxId = id;

                cms.push_back(cm);
            }
        }

        this->characters.resize(maxId + 1);

        for (int i = 0; i < cms.size(); i++)
            this->characters[cms[i].id] = cms[i];
    }

    Font::Font(Texture* tex, const char* fontMetricsFile)
        : texture(tex)
    {
        std::string fontMetrics = util::ReadFileToStringA(fontMetricsFile);
        this->InitFontFromMetrics(fontMetrics.c_str());
    }

    // Gets uv coordinate for char 'code'.
    const CharacterMetrics& Font::GetChar(uint code) const
    {
        return this->characters.at(code);
    }

    const FontMetrics& Font::GetFontMetrics() const
    {
        return this->fontMetrics;
    }
    
    Texture* Font::GetTexture() const
    {
        return this->texture;
    }

    void Font::Destroy()
    {
        this->texture->Destroy();
        delete this;
    }
}
#pragma endregion

    /*@// Routine ********************************************************************************************************@*/
namespace viva
{
    struct Routine
    {
        double tick;
        double lifeTime;
        double delay;
        double startTime;
        std::function<int()> activity;
        int id;
        double lastPulse;
        bool remove;
    };
}

    /*@// RoutineManager *************************************************************************************************@*/
namespace viva
{
    class RoutineManager
    {
    private:
        ObjectPool<Routine> routinePool;
        vector<Routine*> routines;
        std::map<int, vector<std::function<void(int)>>> handlers;
    public:
        RoutineManager();

        void _Activity();

        // Add a new routine.
        //func: code to execute by event (function pointer or closure)
        //      func must return an int. This is to make it easier to remove routines from inside. 
        //      If func returns 0, routine is destroyed.
        //delay: how long to wait before start running
        //lifeTime: destroy event after that time, 0 to never destroy
        //tick: run event once every tick seconds, 0 to run every frame
        Routine* AddRoutine(const std::function<int()>& func, int id, double delay, double lifeTime, double tick);

        Routine* AddRoutine(const std::function<int()>& func);

        void RemoveRoutine(Routine* r);

        // Find routine.
        // Note, this method performs linear search to find by id.
        // name: name of the routine
        Routine* FindRoutine(int id);

        void Trigger(int _event, int data);

        void AddHandler(int _event, const std::function<void(int)>& handler);

        // Remove all handlers for that event.
        void ClearHandlers(int _event);

        // Remove all handlers.
        void ClearHandlers();

        // Remove all routines.
        // Don't call it from inside a routine.
        void ClearRoutines();

        void _Destroy();
    };
}

#pragma region code
namespace viva
{
    RoutineManager::RoutineManager() :routinePool(1000)
    {
    }

    void RoutineManager::_Activity()
    {
        double gameTime = time->GetGameTime();

        //might be removed so backwards iteration
        for (int i = (int)this->routines.size() - 1; i >= 0; i--)
        {
            // destroy if remove flag is up
            if (this->routines.at(i) == nullptr && this->routines.at(i)->remove)
            {
                this->routinePool.Free(this->routines.at(i));
                this->routines.at(i) = nullptr;
            }

            //pop if last routine is empty
            if (this->routines.at(i) == nullptr && i == this->routines.size() - 1)
            {
                this->routines.pop_back();
                continue;
            }
            //if its empty but not last then perform swap and remove
            else if (this->routines.at(i) == nullptr)
            {
                this->routines.at(i) = this->routines.back();
                this->routines.pop_back();
                continue;
            }

            int ret = 1;

            //if delay time has passed...
            if (gameTime - this->routines.at(i)->startTime > this->routines.at(i)->delay)
                //...and it's time for next pulse then run
                if (this->routines.at(i)->tick == 0 ||
                    (gameTime - this->routines.at(i)->lastPulse > this->routines.at(i)->tick))
                {
                    ret = this->routines.at(i)->activity();
                    this->routines.at(i)->lastPulse = gameTime;
                }
            //if returned 0 or expired then remove
            if (ret == 0 || (this->routines.at(i)->lifeTime > 0 &&
                gameTime - this->routines.at(i)->startTime > this->routines.at(i)->lifeTime))
            {
                this->routinePool.Free(this->routines.at(i));
                this->routines.at(i) = this->routines.back();
                this->routines.pop_back();
            }
        }
    }

    // Add a new routine.
    //func: code to execute by event (function pointer or closure)
    //      func must return an int. This is to make it easier to remove routines from inside. 
    //      If func returns 0, routine is destroyed.
    //delay: how long to wait before start running
    //lifeTime: destroy event after that time, 0 to never destroy
    //tick: run event once every tick seconds, 0 to run every frame
    Routine* RoutineManager::AddRoutine(const std::function<int()>& func, int id, double delay, double lifeTime, double tick)
    {
        Routine* newRoutine = this->routinePool.Alloc();

        if (newRoutine == nullptr)
            return nullptr;

        newRoutine->tick = tick;
        newRoutine->lifeTime = lifeTime;
        newRoutine->delay = delay;
        newRoutine->startTime = time->GetGameTime();
        newRoutine->activity = func;
        newRoutine->id = id;
        newRoutine->lastPulse = 0;
        newRoutine->remove = false;
        this->routines.push_back(newRoutine);

        return newRoutine;
    }

    void RoutineManager::RemoveRoutine(Routine* r)
    {
        r->remove = true;
    }

    Routine* RoutineManager::AddRoutine(const std::function<int()>& func)
    {
        return this->AddRoutine(func, 0, 0, 0, 0);
    }

    // Find routine.
    // Note, this method performs linear search to find by name.
    // name: name of the routine
    Routine* RoutineManager::FindRoutine(int id)
    {
        if (id == 0)
            return nullptr;

        for (int i = 0; i < this->routines.size(); i++)
            if (this->routines.at(i)->id == id)
                return this->routines.at(i);

        return nullptr;
    }

    void RoutineManager::Trigger(int _event, int data)
    {
        auto it = this->handlers.find(_event);

        if (it == this->handlers.end())
            return;

        for (auto& handler : it->second)
            handler(data);
    }

    void RoutineManager::AddHandler(int _event, const std::function<void(int)>& handler)
    {
        this->handlers[_event].push_back(handler);
    }

    // Remove all handlers for that event.
    void RoutineManager::ClearHandlers(int _event)
    {
        auto it = this->handlers.find(_event);

        if (it == this->handlers.end())
            return;

        it->second.clear();
    }

    // Remove all handlers.
    void RoutineManager::ClearHandlers()
    {
        this->handlers.clear();
    }

    // Remove all routines.
    // Don't call it from inside a routine.
    void RoutineManager::ClearRoutines()
    {
        this->routines.clear();
    }

    void RoutineManager::_Destroy()
    {
        this->ClearRoutines();
        this->ClearHandlers();

        delete this;
    }
}
#pragma endregion

/*@// PixelShader ****************************************************************************************************@*/
namespace viva
{
    class PixelShader : public Destroyable
    {
    protected:
        ID3D11PixelShader* ps;
    public:
        PixelShader(ID3D11PixelShader* _ps);
        void Destroy();
        ID3D11PixelShader* GetPS() const;
    };
}

#pragma region code
namespace viva
{
    PixelShader::PixelShader(ID3D11PixelShader* _ps) :ps(_ps)
    {
    }

    void PixelShader::Destroy()
    {
        this->ps->Release();
        delete this;
    }

    ID3D11PixelShader* PixelShader::GetPS() const
    {
        return this->ps;
    }
}
#pragma endregion

//class VertexShader
//{
//protected:
//public:
//	virtual void Destroy() = 0;
//};

    /*@// Camera *********************************************************************************************************@*/
namespace viva
{
    class Camera
    {
    private:
        Transform lookAt;
        Matrix sca;
        Matrix scaLoc;
        Size pixel2unit;
        Size unit2pixel;
        // size in world units of the client rect
        Size frustumSize;
    public:
        // Default ctor.
        // size: viewport size
        Camera(const Size& size);

        /*XMFLOAT3 GetCursorWorldPos(float z);
        XMMATRIX GetViewMatrix();
        XMMATRIX GetProjMatrix();*/

        const Matrix& _GetSca() const;

        const Matrix& _GetScaLoc() const;

        Transform* GetLookAt();

        // Returns screen coordinates in pixels of the given world coordinates
        Point WorldToScreen(const Vector& pos) const;

        Vector ScreenToWorld(const Point& pos) const;

        Size Pixel2World(const Size& in) const;

        Size World2Pixel(const Size& in) const;

        const Size& GetPixel2UnitConversion() const;

        const Size& GetUnit2PixelConversion() const;

        const Size& GetFrustumSize() const;

        void _Destroy();

        void _Activity();
    };
}

#pragma region code
namespace viva
{
    Camera::Camera(const Size& unitSize) : 
        unit2pixel(unitSize)
    {
        auto& clientSize = engine->GetClientSize();

        this->pixel2unit = { 1.0f / unitSize.width,
            1.0f / unitSize.height };

        frustumSize = {clientSize.width / unitSize.width, clientSize.height / unitSize.height};
    }

    void Camera::_Activity()
    {
        this->lookAt._Update();

        auto& clientSize = engine->GetClientSize();
        this->sca = math::matrix::scale(
            2.0f / clientSize.width * this->unit2pixel.width,
            2.0f / clientSize.height * this->unit2pixel.height);
        this->scaLoc = this->sca *
            math::matrix::translate(-this->lookAt.Pos().x, this->lookAt.Pos().y);
    }

    Transform* Camera::GetLookAt()
    {
        return &this->lookAt;
    }

    const Size& Camera::GetFrustumSize() const
    {
        return this->frustumSize;
    }

    const Size& Camera::GetPixel2UnitConversion() const
    {
        return this->pixel2unit;
    }

    const Size& Camera::GetUnit2PixelConversion() const
    {
        return this->unit2pixel;
    }

    Size Camera::Pixel2World(const Size& in) const
    {
        return Size(in.width * this->pixel2unit.width, in.height * this->pixel2unit.height);
    }

    Size Camera::World2Pixel(const Size& in) const
    {
        return Size(in.width * this->unit2pixel.width, in.height * this->unit2pixel.height);
    }

    /*XMFLOAT3 GetCursorWorldPos(float z);
    XMMATRIX GetViewMatrix();
    XMMATRIX GetProjMatrix();*/
    
    // Get view matrix.
    const Matrix& Camera::_GetSca() const
    {
        return this->sca;
    }

    const Matrix& Camera::_GetScaLoc() const
    {
        return this->scaLoc;
    }

    // Returns screen coordinates in pixels of the given world coordinates
    Point Camera::WorldToScreen(const Vector& pos) const
    {
        throw Error(__FUNCTION__, "Not implemented");
        return Point(0, 0);
    }

    Vector Camera::ScreenToWorld(const Point& pos) const
    {
        throw Error(__FUNCTION__, "Not implemented");
        return Vector();
    }

    void Camera::_Destroy()
    {
        delete this;
    }
}
#pragma endregion

/*@// Sprite *********************************************************************************************************@*/
namespace viva
{
    // Drawable picture.
    class Sprite : public Drawable
    {
    protected:
        TextureFilter filter;
        Rect uv;
        bool flipHorizontally;
        bool flipVertically;
        Transform transform;
        Surface* parent;
        uint index;
        bool visible;
        Texture* texture;
        PixelShader* ps;
    public:
        // Ctor.
        Sprite(Texture* tex, PixelShader* _ps);

        // Get transform of the object.
        Transform* T();

        // Get transform of the object.
        Transform* GetTransform();

        // Set filter type.
        Sprite* SetTextureFilter(TextureFilter _filter);

        // Get filter type.
        TextureFilter GetTextureFilter() const;

        // Set scale to match texture size.
        Sprite* SetScale2TextureSize();

        // Is sprite flipped horizontally.
        bool IsFlippedHorizontally() const;

        // Set horizontal flip.
        // _flipHorizontally: flip or not
        Sprite* SetFlipHorizontally(bool _flipHorizontally);

        // Is sprite flipped vertically.
        bool IsFlippedVertically() const;

        // Set vertical flip.
        // _flipVertically: flip or not
        Sprite* SetFlipVertically(bool _flipVertically);
        
        // Get uv.
        const Rect& GetUV() const;

        // Get PS.
        PixelShader* GetPixelShader() const;

        //Set Ps.
        Sprite* SetPixelShader(PixelShader* ps);

        // Set uv.
        // _uv: new uv
        //         0          
        //    +---------+     
        //    |         |     
        // 0  |         | 1    
        //    |         |      
        //    +---------+      
        //         1          
        Sprite* SetUV(const Rect& _uv);

        // Set uv.
        Sprite* SetUV(float left, float top, float right, float bottom);

        // Get texture associated with this sprite. Sprites can share a texture.
        Texture* GetTexture() const;

        Surface* GetSurface() const override;

        bool IsVisible() const override;

        Drawable* SetVisible(bool val) override;

        void _SetSurface(Surface* surface) override;

        void _SetIndex(uint i) override;

        int _GetIndex() const override;

        void _Draw() override;

        void Destroy() override;
    };
}

#pragma region code
namespace viva
{
    Sprite::Sprite(Texture* tex, PixelShader* ps) 
        : filter(drawManager->GetDefaultTextureFilter()), uv(0, 0, 1, 1), flipHorizontally(false),
        flipVertically(false), parent(nullptr), index(-1), visible(true), texture(tex), ps(ps)
    {
        this->color = { 255,255,255,255 };
    }

    void Sprite::_Draw()
    {
        //update transform
        this->T()->_Update();

        if (!this->visible)
            return;

        // transform
        Matrix matT = this->transform.GetWorldViewProj().transpose();
        d3d.context->UpdateSubresource(d3d.constantBufferVS, 0, NULL, &matT, 0, 0);
        // uv
        Rect finaluv;
        finaluv.left = flipHorizontally ? this->uv.right : this->uv.left;
        finaluv.right = flipHorizontally ? this->uv.left : this->uv.right;
        finaluv.top = flipVertically ? this->uv.bottom : this->uv.top;
        finaluv.bottom = flipVertically ? this->uv.top : this->uv.bottom;
        d3d.context->UpdateSubresource(d3d.constantBufferUV, 0, 0, &finaluv, 0, 0);
        // rs
        d3d.context->RSSetState(d3d.rsSolid);
        // sampler and color
        d3d.context->PSSetSamplers(0, 1, &d3d.samplerPoint);
        float fColor[] = { this->color.r / 255.0f,this->color.g / 255.0f,this->color.b / 255.0f,this->color.a / 255.0f };
        d3d.context->UpdateSubresource(d3d.constantBufferPS, 0, 0, fColor, 0, 0);
        //extra buffer
        if (extraBufferPSdata != nullptr)
            d3d.context->UpdateSubresource(d3d.constantBufferPSExtra, 0, 0, extraBufferPSdata, 0, 0);
        // ps
        d3d.context->PSSetShader(ps->GetPS(), 0, 0);
        // vb
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        d3d.context->IASetVertexBuffers(0, 1, &d3d.vertexBuffer, &stride, &offset);
        d3d.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        // texture
        d3d.context->PSSetShaderResources(0, 1, this->texture->GetSRV());

        d3d.context->DrawIndexed(6, 0, 0);
    }

    // Get transform of the object.
    Transform* Sprite::T()
    {
        return &(this->transform);
    }

    // Get transform of the object.
    Transform* Sprite::GetTransform()
    {
        return &(this->transform);
    }

    Sprite* Sprite::SetScale2TextureSize()
    {
        this->transform.SetPixelScale(this->texture->GetSize());
        return this;
    }

    // Set filter type.
    Sprite* Sprite::SetTextureFilter(TextureFilter _filter)
    {
        this->filter = _filter;
        return this;
    }

    // Get filter type.
    TextureFilter Sprite::GetTextureFilter() const
    {
        return this->filter;
    }

    // Is sprite flipped horizontally.
    bool Sprite::IsFlippedHorizontally() const
    {
        return this->flipHorizontally;
    }

    // Set horizontal flip.
    // _flipHorizontally: flip or not
    Sprite* Sprite::SetFlipHorizontally(bool _flipHorizontally)
    {
        this->flipHorizontally = _flipHorizontally;
        return this;
    }

    // Is sprite flipped vertically.
    bool Sprite::IsFlippedVertically() const
    {
        return this->flipVertically;
    }

    // Set vertical flip.
    // _flipVertically: flip or not
    Sprite* Sprite::SetFlipVertically(bool _flipVertically)
    {
        this->flipVertically = _flipVertically;
        return this;
    }

    // Get uv.
    const Rect& Sprite::GetUV() const
    {
        return this->uv;
    }
    
    // Set uv.
    // _uv: new uv
    //         0          
    //    +---------+     
    //    |         |     
    // 0  |         | 1    
    //    |         |      
    //    +---------+      
    //         1          
    Sprite* Sprite::SetUV(const Rect& _uv)
    {
        this->uv = Rect(_uv.left, 1 - _uv.bottom, _uv.right, 1 - _uv.top);
        return this;
    }

    // Set uv.
    Sprite* Sprite::SetUV(float left, float top, float right, float bottom)
    {
        return this->SetUV(Rect(left, top, right, bottom));
    }

    Surface* Sprite::GetSurface() const
    {
        return this->parent;
    }

    bool Sprite::IsVisible() const
    {
        return this->visible;
    }

    Drawable* Sprite::SetVisible(bool val)
    {
        this->visible = val;
        return this;
    }

    void Sprite::_SetSurface(Surface* surface)
    {
        this->parent = surface;
    }

    void Sprite::_SetIndex(uint i)
    {
        this->index = i;
    }

    int Sprite::_GetIndex() const
    {
        return this->index;
    }

    void Sprite::Destroy()
    {
        if (this->index != -1)
            drawManager->Remove(this);

        delete this;
    }

    Texture* Sprite::GetTexture() const
    {
        return this->texture;
    }

    // Get PS.
    PixelShader* Sprite::GetPixelShader() const
    {
        return this->ps;
    }

    //Set Ps.
    Sprite* Sprite::SetPixelShader(PixelShader* ps)
    {
        this->ps = ps;
        return this;
    }
}
#pragma endregion

    /*@// Animation ******************************************************************************************************@*/
namespace viva
{
    class Animation : public Drawable
    {
    protected:
        vector<Action> actions;
        vector<std::function<void(int)>> onFrameChangedhandlers;
        vector<std::function<void(int)>> onActionLoopedHandlers;
        Action* currentAction;
        Sprite* sprite;

        // progress of the animation
        double indicator;

        // currently displayed frame
        int currentFrame;
    public:
        Animation(Sprite* _sprite);

        // Get transform of the object.
        Transform* T();

        // Get transform of the object.
        Transform* GetTransform();

        void _Play();

        // Advances animation to the next frame.
        // If the current frame is the last then this method sets current frame to 0.
        void NextFrame();

        // Goes back in the animation.
        // If the current frame is the first then this method sets current frame to the last one.
        void PreviousFrame();

        Sprite* GetSprite() const;

        double GetSpeed() const;

        void SetSpeed(double speed);

        int GetFrame() const;

        void SetFrame(int _currentFrame);

        void SetAction(int id);

        Action* GetAction() const;

        void AddAction(double speed, int columns, int rows, int first, int last);

        void AddAction(double speed, const vector<Rect>& uvTable);

        void AddAction(double speed, const Size& texSizePx, const vector<Rect>& uvTablePx);

        void AddOnFrameChangeHandler(const std::function<void(int)>& fun);

        void AddOnActionLoopedHandler(const std::function<void(int)>& fun);

        void ClearOnFrameChangeHandlers();

        void ClearAddOnActionLoopedHandlers();

        void _Draw() override;

        Surface* GetSurface() const override;

        bool IsVisible() const override;

        Drawable* SetVisible(bool val) override;

        void _SetSurface(Surface* surface) override;

        void _SetIndex(uint i) override;

        int _GetIndex() const override;

        void Destroy() override;
        
        // Set scale to match texture size.
        Animation* SetScale2TextureSize();

        // Is sprite flipped horizontally.
        bool IsFlippedHorizontally() const;

        // Set horizontal flip.
        // _flipHorizontally: flip or not
        Animation* SetFlipHorizontally(bool _flipHorizontally);

        // Is sprite flipped vertically.
        bool IsFlippedVertically() const;

        // Set vertical flip.
        // _flipVertically: flip or not
        Animation* SetFlipVertically(bool _flipVertically);

        // Set filter type.
        Animation* SetTextureFilter(TextureFilter _filter);

        // Get filter type.
        TextureFilter GetTextureFilter() const;
    };
}

#pragma region code
namespace viva
{
    Animation::Animation(Sprite* _sprite)
        : currentAction(nullptr), sprite(_sprite), indicator(0), currentFrame(0)
    {
    }

    // Get transform of the object.
    Transform* Animation::T()
    {
        return this->sprite->T();
    }

    // Get transform of the object.
    Transform* Animation::GetTransform()
    {
        return this->sprite->T();
    }

    void Animation::_Play()
    {
        if (this->currentAction != nullptr && this->currentAction->speed != 0)
        {
            this->indicator += this->currentAction->speed * time->GetFrameTime();
            if (this->indicator >= 1)
            {
                this->indicator = 0;
                this->NextFrame();
            }
            else if (this->indicator < 0)
            {
                this->indicator = 1;
                this->PreviousFrame();
            }
        }

        if (this->currentAction != nullptr)
            this->sprite->SetUV(this->currentAction->uvTable.at(this->currentFrame));
    }

    // Advances animation to the next frame.
    // If the current frame is the last then this method sets current frame to 0.
    void Animation::NextFrame()
    {
        if (this->currentAction == nullptr)
            return;

        this->currentFrame++;

        for (int i = 0; i < this->onFrameChangedhandlers.size(); i++)
            this->onFrameChangedhandlers.at(i)(currentFrame);

        if (this->currentFrame >= this->currentAction->length)
        {
            this->currentFrame = 0;

            for (int i = 0; i < this->onActionLoopedHandlers.size(); i++)
                this->onActionLoopedHandlers.at(i)(this->currentFrame);
        }
    }

    // Goes back in the animation.
    // If the current frame is the first then this method sets current frame to the last one.
    void Animation::PreviousFrame()
    {
        if (this->currentAction == nullptr)
            return;

        this->currentFrame--;

        for (int i = 0; i < this->onFrameChangedhandlers.size(); i++)
            this->onFrameChangedhandlers.at(i)(this->currentFrame);

        if (this->currentFrame < 0)
        {
            this->currentFrame = this->currentAction->length - 1;

            for (int i = 0; i < this->onActionLoopedHandlers.size(); i++)
                this->onActionLoopedHandlers.at(i)(currentFrame);
        }
    }

    Sprite* Animation::GetSprite() const
    {
        return this->sprite;
    }

    double Animation::Animation::GetSpeed() const
    {
        if (this->currentAction == nullptr)
            return INFINITY;

        return this->currentAction->speed;
    }

    void Animation::SetSpeed(double speed)
    {
        if (this->currentAction == nullptr)
            return;

        this->currentAction->speed = speed;
    }

    int Animation::GetFrame() const
    {
        return this->currentFrame;
    }

    void Animation::SetFrame(int _currentFrame)
    {
        if (this->currentAction == nullptr)
            return;

        this->currentFrame = _currentFrame % this->currentAction->length;
    }

    void Animation::SetAction(int index)
    {
        this->currentAction = &this->actions[index];
    }

    Action* Animation::GetAction() const
    {
        if (this->currentAction == nullptr)
            return nullptr;

        return this->currentAction;
    }

    void Animation::AddAction(double speed, int columns, int rows, int first, int last)
    {
        vector<Rect> uvTable;

        float width = 1.0f / columns;
        float height = 1.0f / rows;

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < columns; j++)
            {
                if (columns * i + j >= first)
                    uvTable.push_back(Rect(width*j, height*i, width*(j + 1), height*(i + 1)));
                if (columns * i + j >= last)
                {
                    // break;
                    i = rows;
                    j = columns;
                }
            }

        this->AddAction(speed, uvTable);
    }

    void Animation::AddAction(double speed, const Size& texSizePx, const vector<Rect>& uvTablePx)
    {
        vector<Rect> uvTable;

        for (int i = 0; i < uvTablePx.size(); i++)
        {
            uvTable.push_back({
                uvTablePx.at(i).left / texSizePx.width,
                uvTablePx.at(i).top / texSizePx.height,
                uvTablePx.at(i).right / texSizePx.width,
                uvTablePx.at(i).bottom / texSizePx.height
                });
        }

        this->AddAction(speed, uvTable);
    }

    void Animation::AddAction(double speed, const vector<Rect>& uvTable)
    {
        this->actions.push_back(Action(uvTable, speed));
    }

    void Animation::AddOnFrameChangeHandler(const std::function<void(int)>& fun)
    {
        this->onFrameChangedhandlers.push_back(fun);
    }

    void Animation::AddOnActionLoopedHandler(const std::function<void(int)>& fun)
    {
        this->onActionLoopedHandlers.push_back(fun);
    }

    void Animation::ClearOnFrameChangeHandlers()
    {
        this->onFrameChangedhandlers.clear();
    }

    void Animation::ClearAddOnActionLoopedHandlers()
    {
        this->onActionLoopedHandlers.clear();
    }

    void Animation::_Draw()
    {
        this->_Play();

        if (this->currentAction != nullptr)
            this->sprite->_Draw();
    }

    Surface* Animation::GetSurface() const
    {
        return this->sprite->GetSurface();
    }

    bool Animation::IsVisible() const
    {
        return this->sprite->IsVisible();
    }

    Drawable* Animation::SetVisible(bool val)
    {
        this->sprite->SetVisible(val);
        return this;
    }

    void Animation::_SetSurface(Surface* surface)
    {
        this->sprite->_SetSurface(surface);
    }

    void Animation::_SetIndex(uint i)
    {
        this->sprite->_SetIndex(i);
    }

    int Animation::_GetIndex() const
    {
        return this->sprite->_GetIndex();
    }

    void Animation::Destroy()
    {
        this->sprite->Destroy();
        delete this;
    }

    // Set scale to match texture size.
    Animation* Animation::SetScale2TextureSize()
    {
        this->sprite->SetScale2TextureSize();
        return this;
    }

    // Is sprite flipped horizontally.
    bool Animation::IsFlippedHorizontally() const
    {
        return this->sprite->IsFlippedHorizontally();
    }

    // Set horizontal flip.
    // _flipHorizontally: flip or not
    Animation* Animation::SetFlipHorizontally(bool _flipHorizontally)
    {
        this->sprite->SetFlipHorizontally(_flipHorizontally);
        return this;
    }

    // Is sprite flipped vertically.
    bool Animation::IsFlippedVertically() const
    {
        return this->sprite->IsFlippedVertically();
    }

    // Set vertical flip.
    // _flipVertically: flip or not
    Animation* Animation::SetFlipVertically(bool _flipVertically)
    {
        this->sprite->SetFlipVertically(_flipVertically);
        return this;
    }

    // Set filter type.
    Animation* Animation::SetTextureFilter(TextureFilter _filter)
    {
        this->sprite->SetTextureFilter(_filter);
        return this;
    }

    // Get filter type.
    TextureFilter Animation::GetTextureFilter() const
    {
        return this->sprite->GetTextureFilter();
    }
}
#pragma endregion

    /*@// Creator ********************************************************************************************************@*/
namespace viva
{
    // Factory for all objects.
    class Creator
    {
    private:
        ID3D11ShaderResourceView* SrvFromPixels(const Color* pixels, const Size& _size);
    public:
        // Create pixel shader from file.
        // filename: path to file containing pixel shader.
        PixelShader* CreatePixelShaderFromFile(const char* filename);

        // Create pixel shader from string.
        // str: pixel shader code itself
        PixelShader* CreatePixelShader(const char* str);
        
        Font* CreateFontV(Texture* tex, const char* fontMetrics, bool fromString);

        Font* CreateFontV(Texture* tex, const char* fontMetricsFile);

        // Create texture from file. Supported files BMP, GIF, JPEG, PNG, TIFF, Exif, WMF, EMF.
        // Named (name is given by filename) textures are stored in resource manager automatically. Can be removed by resourceManager::Remove()
        // filename: file path
        Texture* CreateTexture(const char* filename);

        // Create texture from pixels. Noname texture are not stored in resource manager but can be stored by resourceManager::Add()
        // pixels: pixels for the texture starting from left top
        // size: size of the image in pixels
        Texture* CreateTexture(const Color* pixels, const Size& size);

        Text* CreateText(const wchar_t* str);

        Text* CreateText(const wchar_t* str, Font* font);

        // Create sprite from texture.
        // texture: existing texture object
        Sprite* CreateSprite(Texture* texture);

        // Create polygon from points.
        // points: vector of points where each point is x,y in world coordinates
        Polygon* CreatePolygon(const vector<Point>& points);

        // Create polygon from vertex buffer.
        Polygon* CreatePolygon(VertexBuffer* vb);

        // Create shared vertex buffer. That can be used by multiple polygons.
        VertexBuffer* CreateVertexBuffer(const vector<Point>& points, bool shared);

        // Create shared vertex buffer. That can be used by multiple polygons.
        VertexBuffer* CreateVertexBuffer(const vector<Point>& points);

        net::Server* CreateServer(unsigned short port);

        net::Client* CreateClient(const char* ip, unsigned short port);

        // Create surface to render objects on.
        Surface* CreateSurface();

        Animation* CreateAnimation(Sprite* sprite);

        Animation* CreateAnimation(Texture* texture);

        void _Destroy();
    };
}

#pragma region code
namespace viva
{
    Texture* Creator::CreateTexture(const char* filename)
    {
        Color* pixels = nullptr;
        Size size = util::ReadImageToPixels(filename, &pixels);
        Texture* tex = this->CreateTexture(pixels, size);

        // free used because library uses malloc
        if (pixels != nullptr)
            free(pixels);

        return tex;
    }

    Font* Creator::CreateFontV(Texture* tex, const char* fontMetrics, bool fromString)
    {
        return new Font(tex, fontMetrics, fromString);
    }

    Font* Creator::CreateFontV(Texture* tex, const char* fontMetricsFile)
    {
        return new Font(tex, fontMetricsFile);
    }

    Text* Creator::CreateText(const wchar_t* str)
    {
        return new Text(str);
    }

    Text* Creator::CreateText(const wchar_t* str, Font* font)
    {
        return new Text(str, font);
    }

    Animation* Creator::CreateAnimation(Sprite* sprite)
    {
        return new Animation(sprite);
    }

    Animation* Creator::CreateAnimation(Texture* texture)
    {
        Sprite* s = this->CreateSprite(texture);
        return new Animation(s);
    }

    Polygon* Creator::CreatePolygon(const vector<Point>& points)
    {
        VertexBuffer* vb = CreateVertexBuffer(points, false);
        return new Polygon(vb);
    }

    Polygon* Creator::CreatePolygon(VertexBuffer* vb)
    {
        return new Polygon(vb);
    }

    VertexBuffer* Creator::CreateVertexBuffer(const vector<Point>& points)
    {
        return this->CreateVertexBuffer(points, true);
    }

    VertexBuffer* Creator::CreateVertexBuffer(const vector<Point>& points, bool shared)
    {
        uint count = (uint)points.size();

        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;				   // GPU writes and reads
        bd.ByteWidth = (UINT)(sizeof(Vertex) * count);
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bd.CPUAccessFlags = 0;		                   // CPU does nothing

        vector<Vertex> temp;
        for (int i = 0; i < points.size(); i++)
        {
            //float distFromOrigin = sqrtf(points[i].x*points[i].x + points[i].y*points[i].y);
            //if (distFromOrigin > span)
            //    span = distFromOrigin;
            //vertices.push_back(DirectX::XMVectorSet(points[i].x, points[i].y, 0, 0));

            // IMPORTANT, Y is negated because I want +Y to be up not down
            // IMPORTANT, red must be non 0, 0 is reserved for sprites
            temp.push_back({ points.at(i).x, -points.at(i).y, 0, 1, 1, 1, 0, 0 }); 
        }
        //transformedVertices = vertices;

        D3D11_SUBRESOURCE_DATA sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.pSysMem = temp.data();                   //Memory in CPU to copy in to GPU

        ID3D11Buffer* vertexBuffer;
        d3d.device->CreateBuffer(&bd, &sd, &vertexBuffer);

        return new VertexBuffer(vertexBuffer, count, shared);
    }

    /// SURFACE ///
    Surface* Creator::CreateSurface()
    {
        ID3D11Texture2D* tex;
        ID3D11ShaderResourceView* srv;
        ID3D11RenderTargetView* rtv;

        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(textureDesc));
        textureDesc.Width = (UINT)engine->GetClientSize().width;
        textureDesc.Height = (UINT)engine->GetClientSize().height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;
        HRESULT hr = d3d.device->CreateTexture2D(&textureDesc, NULL, &tex);
        util::Checkhr(hr, "CreateTexture2D()");

        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;
        hr = d3d.device->CreateRenderTargetView(tex,
            &renderTargetViewDesc, &rtv);
        util::Checkhr(hr, "CreateRenderTargetView()");

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
        shaderResourceViewDesc.Format = textureDesc.Format;
        shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDesc.Texture2D.MipLevels = 1;
        hr = d3d.device->CreateShaderResourceView(tex,
            &shaderResourceViewDesc, &srv);
        util::Checkhr(hr, "CreateShaderResourceView()");

        Surface* surf = new Surface(tex, rtv, srv);
        surf->SetPixelShader(d3d.defaultPost);
        return surf;
    }

    /// SHADERS ///
    PixelShader* Creator::CreatePixelShaderFromFile(const char* filename)
    {
        std::string ps = util::ReadFileToStringA(filename);
        return CreatePixelShader(ps.c_str());
    }

    PixelShader* Creator::CreatePixelShader(const char* str)
    {
        ID3D11PixelShader* result;

        ID3D10Blob *ps;
        HRESULT hr = D3DCompile(str, strlen(str), 0, 0, 0, "main", "ps_5_0", 0, 0, &ps, 0);
        util::Checkhr(hr, "CreatePixelShader()");

        hr = d3d.device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), 0, &result);
        util::Checkhr(hr, "CreatePixelShader()");
        ps->Release();

        return new PixelShader(result);
    }

    net::Server* Creator::CreateServer(unsigned short port)
    {
        net::Server* s = new net::Server(port);
        networkManager->_Add(s);
        return s;
    }

    net::Client* Creator::CreateClient(const char* ip, unsigned short port)
    {
        net::Client* c = new net::Client(ip, port);
        networkManager->_Add(c);
        return c;
    }

    ID3D11ShaderResourceView* Creator::SrvFromPixels(const Color* pixels, const Size& _size)
    {
        ID3D11Texture2D *tex;
        ID3D11ShaderResourceView* srv;

        D3D11_SUBRESOURCE_DATA sub;
        sub.pSysMem = pixels;
        sub.SysMemPitch = (UINT)_size.width * 4;
        sub.SysMemSlicePitch = (UINT)_size.height*(UINT)_size.width * 4;

        D3D11_TEXTURE2D_DESC desc;
        desc.Width = (UINT)_size.width;
        desc.Height = (UINT)_size.height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.SampleDesc.Count = 1;
        desc.SampleDesc.Quality = 0;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        HRESULT hr = d3d.device->CreateTexture2D(&desc, &sub, &tex);
        util::Checkhr(hr, "CreateTexture2D()");

        D3D11_TEXTURE2D_DESC desc2;
        tex->GetDesc(&desc2);
        hr = d3d.device->CreateShaderResourceView(tex, 0, &srv);
        util::Checkhr(hr, "CreateShaderResourceView()");
        tex->Release();

        return srv;
    }

    /// SPRITE ///
    Sprite* Creator::CreateSprite(Texture* texture)
    {
        return new Sprite(texture, d3d.defaultPS);
    }

    /// TEXTURE ///
    Texture* Creator::CreateTexture(const Color* pixels, const Size& size)
    {
        Texture* t = new Texture(this->SrvFromPixels(pixels, size), size);
        return t;
    }

    //
    void Creator::_Destroy()
    {
        delete this;
    }
}
#pragma endregion

    /*@// Text ***********************************************************************************************************@*/
namespace viva
{
    class Text : public Sprite
    {
    private:
        std::wstring text;
        Font* font;
    public:
        Text(const wchar_t* str, Font* f);

        Text(const wchar_t* str);

        Text* SetText(const wchar_t* _text);

        const wchar_t* GetText() const;

        void _Draw() override;

        // sprite functions that dont make sense
        // SetScale2TextureSize
        // IsFlippedHorizontally
        // SetFlipHorizontally
        // IsFlippedVertically
        // SetFlipVertically
        // GetUV
        // SetUV
        // GetTexture
    };
}

#pragma region code
namespace viva
{
    Text::Text(const wchar_t* str, Font* f)
        : Sprite(f->GetTexture(), d3d.defaultPS), text(str), font(f)
    {
        this->transform.Origin().x = -1;
        this->transform.Origin().y = 1;
    }

    Text::Text(const wchar_t* str)
        : Text(str, drawManager->GetDefaultFont())
    {
    }

    Text* Text::SetText(const wchar_t* str)
    {
        this->text = str;
        return this;
    }

    const wchar_t* Text::GetText() const
    {
        return this->text.c_str();
    }

    void Text::_Draw()
    {
        this->T()->_Update();

        if (!this->visible)
            return;
               
        // state commmon for all letters
        // rs
        d3d.context->RSSetState(d3d.rsSolid);
        // sampler and color
        d3d.context->PSSetSamplers(0, 1, &d3d.samplerPoint);
        float fColor[] = { this->color.r / 255.0f,this->color.g / 255.0f,this->color.b / 255.0f,this->color.a / 255.0f };
        d3d.context->UpdateSubresource(d3d.constantBufferPS, 0, 0, fColor, 0, 0);
        // ps
        d3d.context->PSSetShader(ps->GetPS(), 0, 0);
        // vb
        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        d3d.context->IASetVertexBuffers(0, 1, &d3d.vertexBuffer, &stride, &offset);
        d3d.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        // texture
        d3d.context->PSSetShaderResources(0, 1, this->texture->GetSRV());

        float x = this->transform.Pos().x;
        float y = this->transform.Pos().y;
        float advance = 0;
        float line = 0;
        
        // TODO figure out why origin of letters are in lower right corner instead of upper left
        float _x = x;
        float _y = y;

        if (text.length() > 0 && text[0] != '\n')
        {
            const CharacterMetrics& firstChar = this->font->GetChar(text.at(0));
            _x += this->transform.GetMode() == TransformMode::World ? firstChar.size.width : firstChar.sizePx.width;
            _y += this->transform.GetMode() == TransformMode::World ? firstChar.size.height : firstChar.sizePx.height;
        }


        //// only transform changes
        for (int i = 0; i < text.length(); i++)
        {
            if (text.at(i) == '\n')
            {
                advance = 0;
                line += this->transform.GetMode() == TransformMode::World ? this->font->GetFontMetrics().lineHeight : 
                    -this->font->GetFontMetrics().lineHeightPx;
                continue;
            }

            const CharacterMetrics& cm = this->font->GetChar(text.at(i));
            const Rect curUv = cm.uv;

            if (this->transform.GetMode() == TransformMode::World)
            {
                this->transform.Pos().x = _x + advance + cm.offset.x;
                this->transform.Pos().y = _y - line - cm.offset.y;
                this->transform.Scale().x = cm.size.width;
                this->transform.Scale().y = cm.size.height;
                advance += cm.advance;
            }
            else
            {
                this->transform.Pos().x = _x + advance + cm.offsetPx.x;
                this->transform.Pos().y = _y - line - cm.offsetPx.y;
                this->transform.Scale().x = cm.sizePx.width;
                this->transform.Scale().y = cm.sizePx.height;
                advance += cm.advancePx;
            }

            Matrix matT = this->transform.GetWorldViewProj().transpose();
            d3d.context->UpdateSubresource(d3d.constantBufferVS, 0, NULL, &matT, 0, 0);
            d3d.context->UpdateSubresource(d3d.constantBufferUV, 0, 0, &curUv, 0, 0);
            d3d.context->DrawIndexed(6, 0, 0);
        }

        this->transform.Pos().x = x;
        this->transform.Pos().y = y;
    }
}
#pragma endregion

    /*@// DrawManager ****************************************************************************************************@*/
namespace viva
{
    // Wrapper for adding and removing drawables.
    class DrawManager
    {
    private:
        Surface* defaultSurface;
        Texture* whitePixel;
        vector<Surface*> surfaces;
        Font* defaultFont;
        TextureFilter defaultFilter;
        VertexBuffer* rectVertexBuffer;
        VertexBuffer* circleVertexBuffer;
    public:
        DrawManager();

        // Destroys DrawManager and all drawables.
        void _Destroy();

        // Draw all objects on their surfaces.
        void _DrawNodes();

        // Draw surfaces.
        void _DrawSurfaces();

        Surface* GetDefaultSurface();

        Surface* AddSurface();

        void AddSurface(Surface* s);

        void RemoveSurface(Surface* s);

        void MoveSurfaceUp(Surface* s);

        void MoveSurfaceDown(Surface* s);

        void MoveSurfaceTop(Surface* s);

        void MoveSurfaceBottom(Surface* s);

        // Create and add polygon to be drawn.
        // points: list of points
        // surface: which surface
        Polygon* AddPolygon(const vector<Point>& points, Surface* surface);

        // Create and add polygon to be drawn.
        // points: list of points
        // surface: which surface
        Polygon* AddPolygon(VertexBuffer* vb, Surface* surface);

        Polygon* AddRectangle();

        Polygon* AddRectangle(Surface* surface);

        Polygon* AddCircle();

        Polygon* AddCircle(Surface* surface);

        Polygon* AddCircle(uint vertices);

        Polygon* AddCircle(uint vertices, Surface* surface);

        // Create and add polygon to be drawn.
        // points: list of points
        Polygon* AddPolygon(const vector<Point>& points);

        // Create and add polygon to be drawn.
        // points: list of points
        Polygon* AddPolygon(VertexBuffer* vb);

        // Create and add sprite to be drawn.
        // t: texture to be used by the sprite
        // surface: which surface
        Sprite* AddSprite(Texture* t, Surface* surface);

        // Create and add sprite to be drawn.
        // t: texture to be used by the sprite
        Sprite* AddSprite(Texture* t);

        // Create and add a unicolor rectangle to be drawn.
        // surface: which surface,nullptr for default
        Sprite* AddFillRectangle(Surface* surface);

        // Create and add a unicolor rectangle to be drawn.
        Sprite* AddFillRectangle();

        Text* AddText(const wchar_t* str);

        Text* AddText(const wchar_t* str, Font* font);

        Text* AddText(const wchar_t* str, Surface* surface);

        Text* AddText(const wchar_t* str, Font* font, Surface* surface);

        Animation* AddAnimation(Sprite* sprite, Surface* surface);

        Animation* AddAnimation(Sprite* sprite);

        Animation* AddAnimation(Texture* texture, Surface* surface);

        Animation* AddAnimation(Texture* texture);

        void Remove(Drawable* drawable) const;

        void Add(Drawable* drawable, Surface* surface) const;

        void Add(Drawable* drawable);

        TextureFilter GetDefaultTextureFilter() const;

        void SetDefaultTextureFilter(TextureFilter filter);

        Texture* GetPixel();

        Font* GetDefaultFont() const;

        // Destroy all surfaces (except the default one) and all object from the default surface.
        void Clear();

        // Resize the shared user buffer for PS
        // size: must be multiple of 16
        void ResizeExtraPSBuffer(uint size);
    };
}

#pragma region code
namespace viva
{
    DrawManager::DrawManager()
    {
        this->defaultFilter = TextureFilter::Point;
        this->defaultSurface = creator->CreateSurface();
        this->surfaces.push_back(this->defaultSurface);

        // create default font
        {
            vector<int> data =
            {
                604,1,2,1,6,1,111,1,47,1,8,2,1,2,6,1,2,1,5,5,5,2,18,3,11,1,4,1,11,1,51,1,6,4,7,2,6,5,17,1,
                8,2,1,2,5,1,2,1,5,1,4,1,4,1,2,1,17,3,10,1,6,1,10,1,9,1,40,1,6,1,4,1,4,2,1,1,5,1,5,1,
                16,1,8,1,2,1,6,1,2,1,5,1,9,1,2,1,8,3,7,1,11,1,6,1,7,7,6,1,40,1,6,1,4,1,7,1,5,1,5,1,
                16,1,8,1,2,1,4,8,3,1,10,2,8,1,10,1,10,1,8,1,9,1,9,1,39,1,7,1,4,1,7,1,11,1,16,1,18,1,2,1,
                6,4,8,3,5,1,10,1,10,1,8,1,8,1,1,1,8,1,39,1,7,1,4,1,7,1,10,1,17,1,18,1,2,1,10,1,4,3,8,2,
                20,1,8,1,7,1,3,1,4,7,13,7,15,1,8,1,4,1,7,1,8,2,18,1,16,8,8,1,7,2,5,1,1,1,2,1,17,1,8,1,
                19,1,38,1,8,1,4,1,7,1,7,1,39,1,2,1,10,1,6,1,2,1,4,1,2,2,18,1,8,1,19,1,8,2,27,1,9,1,4,1,
                7,1,6,1,20,3,17,1,2,1,5,1,4,1,6,1,2,1,4,1,3,1,18,1,8,1,19,1,8,1,19,2,7,1,9,1,4,1,7,1,
                5,1,5,1,15,3,16,1,2,1,6,5,8,2,6,5,18,1,6,1,20,1,7,2,19,2,6,1,11,4,5,7,2,7,34,1,2,1,8,1,
                41,1,6,1,28,1,28,1,82,1,42,1,4,1,29,1,1400,4,9,1,5,5,8,4,2,7,5,4,6,4,67,4,53,2,4,1,7,2,5,1,
                11,1,6,1,5,1,4,1,4,1,4,1,4,1,29,2,12,2,10,4,6,1,4,1,4,3,5,6,6,4,1,1,2,6,4,7,9,1,6,1,
                1,1,5,1,10,1,13,1,4,1,4,1,4,1,4,1,28,1,16,1,8,1,4,1,4,1,5,1,5,2,6,1,4,1,4,1,4,2,3,1,
                4,1,4,1,4,1,9,1,5,1,2,1,5,1,9,1,13,1,5,1,4,1,4,1,4,1,5,2,9,2,8,2,5,8,5,2,6,1,4,1,
                4,1,3,3,4,1,2,1,5,1,4,1,3,1,6,1,3,1,5,1,3,1,4,1,6,3,6,1,2,1,5,5,5,1,1,4,8,1,6,4,
                5,1,3,2,5,2,9,2,7,1,22,1,10,1,4,1,2,1,2,1,4,1,2,1,5,1,4,1,3,1,10,1,5,1,3,1,2,1,10,1,
                5,1,3,1,10,1,4,2,4,1,6,1,6,1,4,1,5,3,1,1,23,2,24,2,7,1,5,1,2,1,2,1,4,1,2,1,5,5,4,1,
                10,1,5,1,3,4,11,1,4,6,9,1,4,1,5,1,6,1,6,1,4,1,9,1,25,1,7,8,7,1,8,1,6,1,2,1,2,1,3,6,
                4,1,4,1,3,1,10,1,5,1,3,1,2,1,11,1,8,1,10,1,4,1,5,1,6,1,6,1,4,1,8,1,17,2,8,2,18,2,16,1,
                3,3,3,1,4,1,4,1,4,1,3,1,10,1,5,1,3,1,4,1,3,1,5,1,8,1,4,1,5,1,4,1,5,1,5,1,7,1,4,1,
                7,1,7,2,9,1,11,1,16,1,10,2,6,1,8,1,6,1,3,1,4,1,4,1,5,1,3,1,4,1,4,1,4,1,4,5,7,4,4,5,
                6,5,6,1,8,4,5,3,8,2,8,2,12,2,12,2,11,2,7,1,3,1,2,3,4,3,1,6,6,5,3,6,4,7,85,1,50,3,1764,7,
                5,4,1,1,2,3,2,3,2,7,5,6,2,3,1,4,2,5,4,3,3,3,1,3,3,3,4,4,5,6,5,4,4,6,6,3,1,1,3,7,
                3,3,2,3,1,3,4,7,1,4,1,3,3,3,3,1,4,1,4,1,4,2,3,1,4,1,6,1,11,1,5,1,3,1,6,1,7,2,3,2,
                3,2,4,1,4,1,4,1,5,1,4,1,3,1,4,1,4,1,4,1,4,1,3,2,3,1,2,1,2,1,4,1,4,1,3,1,6,1,2,1,
                5,1,3,1,5,1,4,1,4,1,3,1,10,1,4,1,6,1,11,1,5,1,2,1,7,1,7,1,1,1,1,1,1,1,3,1,1,1,3,1,
                3,1,6,1,4,1,4,1,2,1,6,1,3,1,4,1,4,1,4,1,3,1,2,1,2,1,4,1,4,1,4,1,4,1,3,1,2,1,2,1,
                4,1,3,1,5,1,2,1,5,1,10,1,4,1,6,1,11,1,5,1,1,1,8,1,7,1,1,1,1,1,1,1,3,1,1,1,3,1,3,1,
                6,1,4,1,4,1,2,1,6,1,3,1,4,1,4,1,8,1,2,1,2,1,4,1,4,1,4,1,4,1,3,1,2,1,2,1,5,1,1,1,
                6,4,5,1,10,6,6,1,11,1,5,4,7,1,7,1,1,1,1,1,1,1,3,1,2,1,2,1,3,1,6,1,4,1,4,1,2,1,6,1,
                3,5,6,4,7,1,7,1,4,1,5,1,2,1,4,1,1,1,1,1,1,1,6,1,7,1,2,1,5,1,3,5,2,1,4,1,6,1,6,1,
                4,1,5,1,3,1,6,1,4,1,2,1,2,1,2,1,3,1,3,1,1,1,3,1,6,1,4,5,3,1,6,1,3,1,2,1,11,1,6,1,
                7,1,4,1,5,1,2,1,4,1,1,1,1,1,1,1,5,1,1,1,6,1,8,1,6,1,3,1,4,1,6,1,6,1,4,1,5,1,3,1,
                6,1,4,1,2,1,5,1,3,1,3,1,1,1,3,1,6,1,4,1,7,1,6,1,3,1,3,1,5,1,4,1,6,1,7,1,4,1,5,1,
                2,1,4,1,1,1,1,1,1,1,4,1,3,1,5,1,9,1,5,1,3,1,4,1,6,1,6,1,4,1,5,1,4,1,5,1,4,1,2,1,
                5,1,3,1,4,2,4,1,4,1,5,1,8,1,4,1,4,1,4,1,4,2,3,1,6,1,7,1,4,1,6,2,5,1,1,1,1,1,1,1,
                3,1,5,1,3,5,7,5,3,3,2,3,2,7,4,4,5,3,3,2,2,8,1,3,3,3,1,3,3,2,5,4,5,5,6,4,4,3,3,2,
                3,1,1,3,5,5,6,4,7,2,6,1,3,1,3,3,3,3,115,5,1432,1,22,1,109,1,10,1,38,3,4,1,10,3,9,1,18,1,17,2,
                23,2,16,5,12,2,11,1,10,1,5,2,7,3,3,3,3,6,6,1,7,1,11,1,8,1,1,1,18,1,17,1,24,1,15,1,18,1,29,1,
                8,1,5,1,4,1,4,1,6,1,7,1,11,1,7,1,3,1,18,1,16,1,24,1,15,1,18,1,29,1,9,1,3,1,9,1,7,1,8,1,
                10,1,6,1,5,1,25,4,5,1,1,3,6,4,1,1,4,3,1,1,5,3,5,7,5,3,1,2,3,1,1,3,5,3,7,5,5,1,2,3,
                5,1,1,1,9,1,8,1,8,1,10,1,37,1,4,1,4,2,3,1,4,1,4,2,3,1,3,2,4,1,3,1,6,1,8,1,3,2,4,2,
                3,1,6,1,11,1,5,1,2,1,8,1,10,1,8,1,8,1,10,1,42,1,4,1,5,1,2,1,9,1,5,1,3,1,5,1,5,1,7,1,
                5,1,4,1,4,1,6,1,11,1,5,1,1,1,9,1,9,1,9,1,9,1,9,1,37,6,4,1,5,1,2,1,9,1,5,1,3,7,5,1,
                7,1,5,1,4,1,4,1,6,1,11,1,5,3,9,1,8,1,3,1,6,1,9,1,9,1,36,1,5,1,4,1,5,1,2,1,9,1,5,1,
                3,1,11,1,7,1,5,1,4,1,4,1,6,1,11,1,5,1,2,1,8,1,7,1,4,1,6,1,10,1,8,1,36,1,4,2,4,2,3,1,
                4,1,5,1,3,1,3,2,4,1,4,1,5,1,8,1,3,2,4,1,4,1,6,1,11,1,5,1,3,1,5,5,5,6,6,1,10,1,8,1,
                37,4,1,2,2,2,1,3,6,5,5,3,1,2,4,4,5,5,6,3,1,1,3,3,2,3,2,7,8,1,4,2,2,4,25,1,11,1,7,1,
                102,1,28,1,37,3,15,3,102,1,28,1,155,5,24,5,74,10,1072,3,159,1,29,1,77,1,72,1,8,1,9,1,19,1,77,1,71,1,9,1,
                10,1,18,1,5,2,1,1,2,1,4,2,1,3,6,3,5,2,1,3,6,3,1,2,3,2,2,3,3,4,1,1,2,7,4,2,3,2,3,3,
                2,3,1,3,5,2,1,3,2,3,1,3,3,3,2,7,6,1,9,1,10,1,18,1,6,2,1,2,1,1,4,2,3,1,4,1,3,1,5,2,
                3,1,4,1,3,2,5,3,5,1,4,2,4,1,9,1,4,1,4,1,4,1,3,1,3,1,3,1,2,1,4,1,3,1,5,1,3,1,4,1,
                7,1,9,1,10,1,6,2,10,1,6,1,2,1,2,1,4,1,4,1,3,1,5,1,4,1,5,1,2,1,5,1,5,1,7,1,10,1,9,1,
                4,1,5,1,2,1,4,1,3,1,3,1,3,1,2,1,5,1,3,1,8,1,8,1,9,1,10,1,5,1,2,1,2,1,6,1,6,1,2,1,
                2,1,4,1,4,1,3,1,5,1,4,1,5,1,2,1,5,1,5,1,8,5,5,1,9,1,4,1,5,1,2,1,5,1,1,1,1,1,1,1,
                5,2,6,1,3,1,7,1,8,1,10,1,11,1,8,2,7,1,6,1,2,1,2,1,4,1,4,1,3,1,5,1,4,1,5,1,2,1,5,1,
                5,1,13,1,4,1,9,1,4,1,5,1,2,1,5,1,1,1,1,1,1,1,4,1,2,1,6,1,1,1,7,1,10,1,9,1,10,1,18,1,
                6,1,2,1,2,1,4,1,4,1,4,1,3,1,5,2,3,1,4,1,3,2,5,1,7,1,5,1,4,1,4,1,4,1,3,2,6,2,6,1,
                1,1,1,1,1,1,3,1,4,1,5,1,1,1,6,1,4,1,6,1,9,1,10,1,15,7,2,3,1,2,1,2,2,3,2,3,4,3,6,1,
                1,3,6,3,1,1,4,5,4,6,6,4,6,3,1,2,5,2,7,1,3,1,3,3,2,3,5,1,6,7,6,1,9,1,10,1,56,1,14,1,
                76,1,19,1,9,1,10,1,56,1,14,1,75,1,21,1,8,1,9,1,56,3,12,3,71,5,435
            };

            // here's how above encoding works, each number represents how many consecutive pixels in a row are transparent or white
            // they simply alternate between white or transparent
            // e.g. 4,7,2 means 4 pixels transparent then 7 pixels white then 2 pixels transparent
            vector<Color> data2;
            Color current(0, 0, 0, 0);

            for (auto i : data)
            {
                for (int j = 0; j < i; j++)
                    data2.push_back(current);

                current = current.r == 0 ? Color(255, 255, 255, 255) : Color(0, 0, 0, 0);
            }

            // build font metrics fro default font
            std::string defaultMetrics = "common lineHeight=20\n";
            char buf[500];
            int id = 32;

            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 19; j++)
                {
                    sprintf(buf, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d\n",
                        id, j * 10, i * 19, 10, 19, 0, 0, 10);
                    defaultMetrics += buf;
                    id++;
                }
            }

            Texture* t = creator->CreateTexture(data2.data(), Size(190, 95));
            this->defaultFont = creator->CreateFontV(t, defaultMetrics.c_str(), true);
        }

        Color p[] = { Color(255,255,255,255) };
        this->whitePixel = creator->CreateTexture(p, Size(1, 1));

        vector<Point> square({ Point(0,0),Point(1,0) ,Point(1,-1) ,Point(0,-1) ,Point(0,0) });
        this->rectVertexBuffer = creator->CreateVertexBuffer(square);

        vector<Point> circle;
        for (int i = 0; i < 21; i++)
            circle.push_back(Point(sinf(2 * math::PI / 20.0f * i) / 2.0f, cosf(2 * math::PI / 20.0f * i) / 2.0f ));
        this->circleVertexBuffer = creator->CreateVertexBuffer(circle);
    }

    // Destroys DrawManager and all drawables.
    void DrawManager::_Destroy()
    {
        for (Surface* s : this->surfaces)
            s->Destroy();

        this->defaultFont->Destroy();
        this->whitePixel->Destroy();
        this->rectVertexBuffer->Destroy();
        this->circleVertexBuffer->Destroy();
        delete this;
    }

    // Draw all objects on their surfaces.
    void DrawManager::_DrawNodes()
    {
        for(int i=0;i<this->surfaces.size();i++)
            surfaces.at(i)->_DrawAll();
    }

    // Draw surfaces.
    void DrawManager::_DrawSurfaces()
    {
        for (int i = 0; i < this->surfaces.size(); i++)
            surfaces.at(i)->_DrawSurface();
    }

    Surface* DrawManager::AddSurface()
    {
        Surface* newSurface = creator->CreateSurface();
        this->surfaces.push_back(newSurface);
        return newSurface;
    }

    Surface* DrawManager::GetDefaultSurface()
    {
        return this->defaultSurface;
    }

    void DrawManager::AddSurface(Surface* s)
    {
        this->surfaces.push_back(s);
    }

    void DrawManager::RemoveSurface(Surface* s)
    {
        if (s == this->defaultSurface)
            return;

        auto it = std::find(this->surfaces.begin(), this->surfaces.end(), s);

        if (it != this->surfaces.end())
            this->surfaces.erase(it);
    }

    void DrawManager::MoveSurfaceUp(Surface* s)
    {
        if (this->surfaces[0] == s)
            return;

        auto it = std::find(this->surfaces.begin(), this->surfaces.end(), s);
        auto prev = --it;
        *it = *prev;
        *prev = s;
    }

    void DrawManager::MoveSurfaceDown(Surface* s)
    {
        if (this->surfaces.back() == s)
            return;

        auto it = std::find(this->surfaces.begin(), this->surfaces.end(), s);
        auto next = ++it;
        *it = *next;
        *next = s;
    }

    void DrawManager::MoveSurfaceTop(Surface* s)
    {
        if (this->surfaces[0] == s)
            return;

        auto it = std::find(this->surfaces.begin(), this->surfaces.end(), s);
        this->surfaces.erase(it);
        this->surfaces.insert(this->surfaces.begin(), s);
    }

    void DrawManager::MoveSurfaceBottom(Surface* s)
    {
        if (this->surfaces.back() == s)
            return;

        auto it = std::find(this->surfaces.begin(), this->surfaces.end(), s);
        this->surfaces.erase(it);
        this->surfaces.push_back(s);
    }

    // Create and add polygon to be drawn.
    // points: list of points
    // surface: which surface
    Polygon* DrawManager::AddPolygon(const vector<Point>& points, Surface* surface)
    {
        Polygon* p = creator->CreatePolygon(points);
        this->Add(p, surface);
        return p;
    }

    // Create and add polygon to be drawn.
    // points: list of points
    // surface: which surface
    Polygon* DrawManager::AddPolygon(VertexBuffer* vb, Surface* surface)
    {
        Polygon* p = creator->CreatePolygon(vb);
        this->Add(p, surface);
        return p;
    }

    Polygon* DrawManager::AddRectangle()
    {
        return this->AddRectangle(this->defaultSurface);
    }

    Polygon* DrawManager::AddRectangle(Surface* surface)
    {
        Polygon* p = creator->CreatePolygon(this->rectVertexBuffer);
        this->Add(p, surface);
        return p;
    }

    Polygon* DrawManager::AddCircle()
    {
        return this->AddCircle(this->defaultSurface);
    }

    Polygon* DrawManager::AddCircle(Surface* surface)
    {
        Polygon* p = creator->CreatePolygon(this->circleVertexBuffer);
        this->Add(p, surface);
        return p;
    }

    Polygon* DrawManager::AddCircle(uint vertices)
    {
        return this->AddCircle(vertices, this->defaultSurface);
    }

    Polygon* DrawManager::AddCircle(uint vertices, Surface* surface)
    {
        vector<Point> circle;
        for (uint i = 0; i < vertices + 1; i++)
            circle.push_back(Point(sinf(2.0f * math::PI / vertices * i), cosf(2.0f * math::PI / vertices * i)));

        Polygon* p = creator->CreatePolygon(circle);
        this->Add(p, surface);
        return p;
    }

    // Create and add polygon to be drawn.
    // points: list of points
    Polygon* DrawManager::AddPolygon(const vector<Point>& points)
    {
        return this->AddPolygon(points, this->defaultSurface);
    }

    // Create and add polygon to be drawn.
    // points: list of points
    Polygon* DrawManager::AddPolygon(VertexBuffer* vb)
    {
        return this->AddPolygon(vb, this->defaultSurface);
    }

    // Create and add sprite to be drawn.
    // t: texture to be used by the sprite
    // surface: which surface
    Sprite* DrawManager::AddSprite(Texture* t, Surface* surface)
    {
        Sprite* s = creator->CreateSprite(t);
        this->Add(s, surface);
        return s;
    }

    // Create and add sprite to be drawn.
    // t: texture to be used by the sprite
    Sprite* DrawManager::AddSprite(Texture* t)
    {
        return this->AddSprite(t, this->defaultSurface);
    }

    // Create and add a unicolor rectangle to be drawn.
    // surface: which surface,nullptr for default
    Sprite* DrawManager::AddFillRectangle(Surface* surface)
    {
        Sprite* s = this->AddSprite(this->whitePixel, surface);
        return s;
    }

    // Create and add a unicolor rectangle to be drawn.
    Sprite* DrawManager::AddFillRectangle()
    {
        return this->AddFillRectangle(this->defaultSurface);
    }

    Text* DrawManager::AddText(const wchar_t* str)
    {
        return this->AddText(str, this->defaultSurface);
    }

    Text* DrawManager::AddText(const wchar_t* str, Font* font)
    {
        return this->AddText(str, font, this->defaultSurface);
    }

    Text* DrawManager::AddText(const wchar_t* str, Surface* surface)
    {
        Text* t = creator->CreateText(str);
        this->Add(t, surface);

        return t;
    }

    Text* DrawManager::AddText(const wchar_t* str, Font* font, Surface* surface)
    {
        Text* t = creator->CreateText(str, font);
        this->Add(t, surface);

        return t;
    }

    Animation* DrawManager::AddAnimation(Sprite* sprite, Surface* surface)
    {
        Animation* ani = creator->CreateAnimation(sprite);
        this->Add(ani, surface);
        return ani;
    }

    Animation* DrawManager::AddAnimation(Sprite* sprite)
    {
        Animation* ani = creator->CreateAnimation(sprite);
        this->Add(ani, this->defaultSurface);
        return ani;
    }

    Animation* DrawManager::AddAnimation(Texture* texture, Surface* surface)
    {
        Animation* ani = creator->CreateAnimation(texture);
        this->Add(ani, surface);
        return ani;        
    }

    Animation* DrawManager::AddAnimation(Texture* texture)
    {
        Animation* ani = creator->CreateAnimation(texture);
        this->Add(ani, this->defaultSurface);
        return ani;
    }

    void DrawManager::Remove(Drawable* drawable) const
    {
        drawable->GetSurface()->Remove(drawable);
    }

    void DrawManager::Add(Drawable* drawable, Surface* surface) const
    {
        surface->Add(drawable);
    }

    void DrawManager::Add(Drawable* drawable)
    {
        this->defaultSurface->Add(drawable);
    }

    TextureFilter DrawManager::GetDefaultTextureFilter() const
    {
        return this->defaultFilter;
    }

    void DrawManager::SetDefaultTextureFilter(TextureFilter filter)
    {
        this->defaultFilter = filter;
    }

    Texture* DrawManager::GetPixel()
    {
        return this->whitePixel;
    }

    Font* DrawManager::GetDefaultFont() const
    {
        return this->defaultFont;
    }

    // Destroy all surfaces (except the default one) and all object from the default surface.
    void DrawManager::Clear()
    {
        for (Surface* surface : this->surfaces)
            if (surface != this->defaultSurface)
                surface->Destroy();

        this->defaultSurface->Clear();
        this->surfaces.clear();
        this->surfaces.push_back(this->defaultSurface);
    }

    void DrawManager::ResizeExtraPSBuffer(uint size)
    {
        d3d.constantBufferPSExtra->Release();
        d3d.constantBufferPSExtra = util::CreateConstantBuffer(size);
        d3d.context->PSSetConstantBuffers(1, 1, &d3d.constantBufferPSExtra);
    }
}
#pragma endregion

/*@// Window *********************************************************************************************************@*/
namespace viva
{
    // Window where everything is being drawn.
    class Window
    {
    private:
        RAWINPUTDEVICE Rid; //RAWINPUTDEVICE Rid[1]; // you can have more than one
        HWND handle;
        MSG msg;
        std::function<void()> worker;   // lib side worker
        std::function<void()> activity; // client size activity
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    public:
        // Ctor.
        Window(const char* title, const Size& size);

        HWND GetHandle() const;

        void _Destroy();

        void SetWindowTitle(const char* title);

        // start win32 message pump
        // gameloop: user's function that runs every frame
        // intloop: engine function that runs every frame
        CloseReason Run(const std::function<void()>& gameloop, const std::function<void()>& intloop);
    };
}

#pragma region code
namespace viva
{
    LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_SYSKEYDOWN:
        {
            if (wParam == VK_MENU)//ignore left alt stop
            {
            }
            else
                return DefWindowProc(hwnd, msg, wParam, lParam); // this makes ALT+F4 work
            break;
        }
        case WM_CLOSE:
        {
            ShowWindow(hwnd, false);

            if (wParam == 0)
                PostQuitMessage((int)CloseReason::WindowClose);
            else if (wParam == (int)CloseReason::EngineClose)
                PostQuitMessage((int)CloseReason::EngineClose);

            break;
        }
        case WM_COMMAND:
            break;
        case WM_MOUSEWHEEL:
        {
            if (mouse != nullptr)
                mouse->_SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));

            break;
        }
        case WM_INPUT:
        {
            UINT dwSize = 48; // 48 for 64bit build
            static BYTE lpb[48];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
                lpb, &dwSize, sizeof(RAWINPUTHEADER)); // this gets relative coords

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                int xPosRelative = raw->data.mouse.lLastX;
                int yPosRelative = raw->data.mouse.lLastY;

                if (mouse != nullptr)
                    mouse->_SetCursorDeltaRaw(xPosRelative, yPosRelative);
            }
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        return 0;
    }

    Window::Window(const char* title, const Size& size)
    {
        const char className[] = "myWindowClass";
        WNDCLASSEX wc;
        ZeroMemory(&msg, sizeof(msg));
        ZeroMemory(&wc, sizeof(wc));
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = Window::WndProc;
        wc.hInstance = GetModuleHandle(0);
        wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
        wc.lpszClassName = className;
        wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

        if (!RegisterClassEx(&wc))
        {
            throw viva::Error(__FUNCTION__, "Window Class failed to register");
        }

        RECT rect = { 0, 0, (int)size.width, (int)size.height };

        // this is to enforce the correct size of window client
        ::AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS, FALSE, 0);

        this->handle = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
            NULL, NULL, GetModuleHandle(0), NULL);

        USHORT HID_USAGE_PAGE_GENERIC = 1;
        USHORT HID_USAGE_GENERIC_MOUSE = 2;

        Rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
        Rid.usUsage = HID_USAGE_GENERIC_MOUSE;
        Rid.dwFlags = RIDEV_INPUTSINK;
        Rid.hwndTarget = handle;
        ::RegisterRawInputDevices(&Rid, 1, sizeof(RAWINPUTDEVICE));

        if (handle == NULL)
        {
            throw Error(__FUNCTION__, "Window Creation Failed!");
        }
    }

    HWND Window::GetHandle() const
    {
        return this->handle;
    }

    void Window::_Destroy()
    {
        ::DestroyWindow(this->handle);
        ::UnregisterClass("myWindowClass", GetModuleHandle(0));
        delete this;
    }

    void Window::SetWindowTitle(const char* title)
    {
        ::SetWindowText(this->handle, title);
    }

    // start win32 message pump
    // gameloop: user's function that runs every frame
    // intloop: engine function that runs every frame
    CloseReason Window::Run(const std::function<void()>& gameloop, const std::function<void()>& intloop)
    {
        ::ShowWindow(handle, SW_SHOW);
        ::UpdateWindow(handle);
        this->activity = gameloop;
        this->worker = intloop;

        while (true)
        {
            if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                if (msg.message == WM_QUIT)
                {
                    CloseReason code = (CloseReason)msg.wParam;
                    ::ZeroMemory(&msg, sizeof(msg));
                    return code;
                }
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                this->worker();
                this->activity();
            }
        }
    }
}
#pragma endregion

namespace viva
{
    struct VivaParams
    {
        const char* title;
        Size size;
        Size unit;
    };

    // Main viva object. Viva starts and ends here.
    class Viva
    {
    public:
        // Initialize viva.
        Viva(VivaParams& params);

        // Destructor.
        ~Viva();
    };
}

#pragma region code
namespace viva
{
    Viva::Viva(VivaParams& params)
    {
        if (params.unit.width == 0)
            params.unit = { 32.0f,32.0f };

        window = new Window(params.title, params.size);
        creator = new Creator();
        engine = new Engine(params.size);
        camera = new Camera(params.unit);
        drawManager = new DrawManager();
        keyboard = new input::Keyboard();
        mouse = new input::Mouse();
        time = new Time();
        routineManager = new RoutineManager();
        networkManager = new net::NetworkManager();
        uiManager = new ui::UIManager();
    }

    // Destructor.
    Viva::~Viva()
    {
        // in reverse order
        uiManager->_Destroy();
        networkManager->_Destroy();
        routineManager->_Destroy();
        time->_Destroy();
        mouse->_Destroy();
        keyboard->_Destroy();
        drawManager->_Destroy();
        camera->_Destroy();
        engine->_Destroy();
        creator->_Destroy();
        window->_Destroy();
    }
}
#pragma endregion

/*@// NetworkError ***********************************************************************************************@*/
namespace viva
{
    namespace net
    {
        struct NetworkError
        {
            std::string message;
            int code;
        };
    }
}

/*@// Base *******************************************************************************************************@*/
namespace viva
{
    namespace net
    {
        class Socket
        {
        protected:
            int index;
            size_t id;
            std::queue<NetworkError> errors;
            std::mutex errorQueueMutex;
            std::function<void(const NetworkError& error)> onErrorHandler;
            Routine* activityRoutine;
            SOCKET handle;
            sockaddr_in address;
            static bool wsInitialized;
            static WSAData wsadata;
            static std::string GetLastWinsockErrorMessage(DWORD errorCode);
        public:
            void _AddError(const NetworkError& error);

            size_t GetId() const;

            void OnError(const std::function<void(const NetworkError& error)>& handler);

            SOCKET _GetSocket() const;

            sockaddr_in _GetSockAddr() const;

            virtual void _Activity() = 0;

            virtual void Destroy() = 0;

            int _GetIndex() const;

            void _SetIndex(int index);
        };
    }
}

#pragma region code
namespace viva
{
    namespace net
    {
        bool Socket::wsInitialized = false;
        WSAData Socket::wsadata;

        std::string Socket::GetLastWinsockErrorMessage(DWORD errorCode)
        {
            char str[300];
            SecureZeroMemory(str, sizeof(char) * 300);
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode,
                MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), str, 300, 0);
            return std::string(str);
        }

        void Socket::_AddError(const NetworkError& error)
        {
            this->errorQueueMutex.lock();
            this->errors.push(error);
            this->errorQueueMutex.unlock();
        }

        size_t Socket::GetId() const
        {
            return this->id;
        }

        void Socket::OnError(const std::function<void(const NetworkError& error)>& handler)
        {
            this->onErrorHandler = handler;
        }

        SOCKET Socket::_GetSocket() const
        {
            return this->handle;
        }

        sockaddr_in Socket::_GetSockAddr() const
        {
            return this->address;
        }

        int Socket::_GetIndex() const
        {
            return this->index;
        }

        void Socket::_SetIndex(int index)
        {
            this->index = index;
        }
    }
}
#pragma endregion

        /*@// Client *****************************************************************************************************@*/
namespace viva
{
    namespace net
    {
        class Client : public net::Socket
        {
        protected:
            std::string ip;
            unsigned short port;
            std::function<void()> onConnectHandler;
            std::function<void(const vector<byte>&)> onMsgHandler;
            bool isConnected;
            bool returnReceive;
            Routine* timeOutHandler;
            std::future<int> timeOut;
            std::future<bool> receiveThread;
            std::mutex msgQueueMutex;
            std::vector<byte> msg;
        public:
            Client(const char* _ip, unsigned short _port);

            Client(SOCKET socket, sockaddr_in address, const char* ip, unsigned short port);

            void OnConnect(const std::function<void()>& handler);

            void OnMsg(const std::function<void(const vector<byte>&)>& handler);

            void _Activity() override;

            void _PushMsgBytes(byte* arr, int len);

            void _ProcessMsg();

            void _SetConnected(bool val);

            const char* GetIp() const;

            void Send(vector<byte>& msg);

            void Send(byte* msg, unsigned short len);

            void Connect(double timeoutSeconds);

            void Disonnect();

            void Destroy() override;

            static bool ReceiveThread(Client* client);

            static int ConnectThread(Client* client);

            bool GetReturnReceive() const;
        };
    }
}

#pragma region code
namespace viva
{
    namespace net
    {
        Client::Client(SOCKET socket, sockaddr_in address, const char* ip, unsigned short port)
            : ip(ip), port(port), isConnected(false), returnReceive(false)
        {
            this->handle = socket;
            this->address = address;
            this->isConnected = true;
        }

        Client::Client(const char* ip, unsigned short port)
            : ip(ip), port(port), isConnected(false), returnReceive(false)
        {
            // WSAStartup
            if (!Socket::wsInitialized)
            {
                int res = ::WSAStartup(MAKEWORD(2, 2), &wsadata);
                if (res != 0)
                {
                    std::string msg = GetLastWinsockErrorMessage(res);
                    throw viva::Error("WSAStartup", msg.c_str());
                }

                Socket::wsInitialized = true;
            }

            // socket()
            handle = socket(AF_INET, SOCK_STREAM, NULL);
            if (handle == INVALID_SOCKET)
            {
                std::string msg = GetLastWinsockErrorMessage(WSAGetLastError());
                throw viva::Error("socket", msg.c_str());
            }
            id = (size_t)handle;

            // sockaddr_in
            SecureZeroMemory(&address, sizeof(address));
            address.sin_family = AF_INET;
            inet_pton(AF_INET, ip, &(address.sin_addr));
            address.sin_port = htons(port);
        }

        void Client::OnConnect(const std::function<void()>& handler)
        {
            this->onConnectHandler = handler;
        }

        void Client::OnMsg(const std::function<void(const vector<byte>&)>& handler)
        {
            this->onMsgHandler = handler;
        }

        void Client::_Activity()
        {
            this->_ProcessMsg();
        }

        void Client::_PushMsgBytes(byte* arr, int len)
        {
            this->msgQueueMutex.lock();
            size_t size = this->msg.size();
            this->msg.resize(msg.size() + len);
            memcpy(this->msg.data() + size, arr, len);
            this->msgQueueMutex.unlock();
        }

        void Client::_ProcessMsg()
        {
            if (this->msg.size() > 0)
            {
                this->msgQueueMutex.lock();
                unsigned short size;
                memcpy(&size, this->msg.data(), sizeof(unsigned short)); // TODO endianess problem

                if (this->msg.size() < size + 2)
                {
                    this->msgQueueMutex.unlock();
                    return;
                }

                vector<byte> completeMsg(this->msg.begin() + 2, this->msg.begin() + 2 + size);
                this->msg.erase(this->msg.begin(), this->msg.begin() + 2 + size);
                this->msgQueueMutex.unlock();

                if (this->onMsgHandler)
                    this->onMsgHandler(completeMsg);
            }
        }

        void Client::_SetConnected(bool val)
        {
            this->isConnected = val;
        }

        const char* Client::GetIp() const
        {
            return this->ip.c_str();
        }

        void Client::Send(vector<byte>& msg)
        {
            this->Send(msg.data(), (unsigned short)msg.size());
        }

        int Client::ConnectThread(Client* client)
        {
            char buf[5] = { 0,0,0,0,0 };

            if (::connect(client->_GetSocket(), (sockaddr*)&(client->_GetSockAddr()), sizeof(sockaddr_in)) == SOCKET_ERROR)
                return WSAGetLastError();

            // welcome protocol
            int received = 0;
            while (received < 5)
                received += ::recv(client->_GetSocket(), (char*)buf, 5 - received, 0);

            if (::memcmp(buf, "Hello", 5) != 0)
                return 10060; // error 10060 is timed out

            client->_SetConnected(true);

            return 0;
        }

        bool Client::ReceiveThread(Client* client)
        {
            byte tempBuffer[256];

            while (!client->GetReturnReceive())
            {
                int len = ::recv(client->_GetSocket(), (char*)tempBuffer, 256, NULL);

                if (len == SOCKET_ERROR)
                {
                    auto code = ::WSAGetLastError();
                    std::string msg = GetLastWinsockErrorMessage(code);
                    NetworkError err = { msg, code };
                    client->_AddError(err);
                }
                /*else if (len == 0)
                {
                    break;
                }*/

                client->_PushMsgBytes(tempBuffer, len);
            }

            return false;
        }

        void Client::Connect(double timeoutSeconds)
        {
            if (isConnected)
                throw Error(__FUNCTION__, "Client is already running");

            this->msg.clear();

            this->timeOut = std::async(Client::ConnectThread, this);

            // TODO implement this without timeout ?
            this->timeOutHandler = routineManager->AddRoutine([&]()
            {
                auto state = this->timeOut.wait_for(std::chrono::milliseconds(0));

                if (state != std::future_status::ready)
                {
                    ::closesocket(handle);
                    handle = ::socket(AF_INET, SOCK_STREAM, NULL);
                    id = (long long)handle;
                    ::WSASetLastError(10060);
                    std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                    NetworkError err = { msg, 10060 };
                    this->_AddError(err);
                }
                else if (timeOut.get() != 0)
                {
                    ::closesocket(handle);
                    handle = ::socket(AF_INET, SOCK_STREAM, NULL);
                    id = (long long)handle;
                    ::WSASetLastError(10060);
                    std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                    NetworkError err = { msg, 10060 };
                    this->_AddError(err);
                }
                else
                {
                    this->onConnectHandler();
                    this->receiveThread = std::async(Client::ReceiveThread, this);
                }

                return 0;
            }, 0, timeoutSeconds, 0, 0);
        }

        bool Client::GetReturnReceive() const
        {
            return this->returnReceive;
        }

        void Client::Send(byte* msg, unsigned short len)
        {
            vector<byte> buf;
            buf.resize(2 + len);
            ::memcpy(buf.data(), &len, 2);
            ::memcpy(buf.data() + 2, msg, len);
            int sent = 0;

            while (sent < buf.size())
            {
                int temp = ::send(handle, (char*)buf.data() + sent, (int)buf.size() - sent, 0);

                if (temp == SOCKET_ERROR)
                {
                    std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                    throw viva::Error("Send", msg.c_str());
                }

                sent += temp;
            }
        }

        void Client::Disonnect()
        {
            // TODO
            throw Error(__FUNCTION__, "Not implemented");
        }

        void Client::Destroy()
        {
            networkManager->_Remove(this);

            // TODO
            throw Error(__FUNCTION__, "Not implemented");
            // other things to do:
            // stop threads
            // close sockets

            delete this;
        }
    }
}
#pragma endregion

/*@// Server *****************************************************************************************************@*/
namespace viva
{
    namespace net
    {
        class Server : public net::Socket
        {
        protected:
            unsigned short port;
            std::function<void(Client* c)> onConnectHandler;
            std::function<void(Client* c)> onDisconnectHandler;
            std::mutex errorQueueMutex;
            std::mutex clientQueueMutex;
            std::queue<Client*> clients; // first clients are coming to queue
            vector<Client*> ackedClients; // main thread grabs clients from queue, calls onconnect callback and moves from queue to vector
            bool isRunning;
            bool returnAccept;
            std::future<bool> acceptThread;

        public:
            Server(unsigned short port);

            void OnConnect(const std::function<void(Client* c)>& handler);

            void OnDisconnect(const std::function<void(Client* c)>& handler);

            const vector<Client*>& GetClients() const;

            void _AddClient(Client* client);

            void _Activity() override;

            bool GetReturnAccept() const;

            void Start(int backlog);

            void Stop();

            void Destroy() override;

            static bool AcceptThread(Server* server);
        };
    }
}

#pragma region code
namespace viva
{
    namespace net
    {
        Server::Server(unsigned short port)
            : port(port), isRunning(false), returnAccept(false)
        {
            // WSAStartup
            if (!Socket::wsInitialized)
            {
                int res = ::WSAStartup(MAKEWORD(2, 2), &wsadata);
                if (res != 0)
                {
                    std::string msg = GetLastWinsockErrorMessage(res);
                    throw viva::Error("WSAStartup", msg.c_str());
                }

                Socket::wsInitialized = true;
            }

            // socket()
            ::SecureZeroMemory(&address, sizeof(address));
            this->handle = ::socket(AF_INET, SOCK_STREAM, NULL);
            if (this->handle == INVALID_SOCKET)
            {
                std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                throw viva::Error("socket", msg.c_str());
            }

            // sockaddr
            address.sin_port = ::htons(port);
            address.sin_addr.s_addr = ::htonl(INADDR_ANY);
            address.sin_family = AF_INET;
            sockaddr* paddress = (sockaddr*)&address;

            // bind()
            if (::bind((SOCKET)handle, paddress, (int)sizeof(sockaddr)) == SOCKET_ERROR)
            {
                std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                throw viva::Error("bind", msg.c_str());
            }
        }

        void Server::OnConnect(const std::function<void(Client* c)>& handler)
        {
            this->onConnectHandler = handler;
        }

        void Server::OnDisconnect(const std::function<void(Client* c)>& handler)
        {
            this->onDisconnectHandler = handler;
        }

        const vector<Client*>& Server::GetClients() const
        {
            return this->ackedClients;
        }

        void Server::_AddClient(Client* client)
        {
            this->clientQueueMutex.lock();
            this->clients.push(client);
            this->clientQueueMutex.unlock();
        }

        void Server::_Activity()
        {
            if (this->clients.size() > 0)
            {
                this->clientQueueMutex.lock();
                Client* client = this->clients.front();
                this->clients.pop();
                this->clientQueueMutex.unlock();

                this->ackedClients.push_back(client);
                this->onConnectHandler(client);
            }
        }

        bool Server::GetReturnAccept() const
        {
            return this->returnAccept;
        }

        bool Server::AcceptThread(Server* server)
        {
            int size = sizeof(sockaddr_in);
            sockaddr_in address;
            SOCKET acceptedSocket;
            char hello[] = "Hello";

            while (!server->GetReturnAccept())
            {
                acceptedSocket = ::accept(server->_GetSocket(), (sockaddr*)(&address), &size);

                if (acceptedSocket == INVALID_SOCKET)
                {
                    auto code = ::WSAGetLastError();
                    std::string msg = GetLastWinsockErrorMessage(code);
                    NetworkError err = { msg, code };
                    server->_AddError(err);
                }
                else
                {
                    int sent = 0;
                    while (sent < 5)
                    {
                        int temp = ::send(acceptedSocket, (const char*)hello + sent, 5 - sent, 0);

                        if (temp == SOCKET_ERROR)
                            return false;

                        sent += temp;
                    }

                    Client* client = new Client(acceptedSocket, address, "", 0);
                    server->_AddClient(client);
                }
            }

            return false;
        }

        void Server::Start(int backlog)
        {
            if (this->isRunning)
                throw viva::Error(__FUNCTION__, "Server already running");

            // listen()
            if (::listen(handle, backlog) == SOCKET_ERROR)
            {
                std::string msg = GetLastWinsockErrorMessage(::WSAGetLastError());
                throw viva::Error("listen", msg.c_str());
            }

            this->isRunning = true;

            this->acceptThread = std::async(Server::AcceptThread, this);
        }

        void Server::Stop()
        {
            // TODO
            throw Error(__FUNCTION__, "Not implemented");
        }

        void Server::Destroy()
        {
            networkManager->_Remove(this);

            // TODO
            throw Error(__FUNCTION__, "Not implemented");
            // other things to do:
            // stop threads
            // close sockets

            delete this;
        }
    }
}
#pragma endregion

/*@ Network Manager **********************************************************************************************@*/
namespace viva
{
    namespace net
    {
        class NetworkManager
        {
        private:
            vector<Socket*> sockets;
        public:
            void _Activity();

            void _Clear();

            void _Destroy();

            void _Add(Socket* s);

            void _Remove(Socket* s);
        };
    }
}

#pragma region code
namespace viva
{
    namespace net
    {
        void NetworkManager::_Add(Socket* b)
        {
            b->_SetIndex(this->sockets.size());
            this->sockets.push_back(b);
        }

        void NetworkManager::_Remove(Socket* b)
        {
            this->sockets.erase(this->sockets.begin() + b->_GetIndex());
        }

        void NetworkManager::_Activity()
        {
            for (int i = 0; i < this->sockets.size(); i++)
                this->sockets[i]->_Activity();
        }

        void NetworkManager::_Clear()
        {
            for (int i = 0; i < this->sockets.size(); i++)
                this->sockets[i]->Destroy();

            this->sockets.clear();
        }

        void NetworkManager::_Destroy()
        {
            this->_Clear();
            delete this;
        }
    }
}
#pragma endregion

/*@// Mouse ******************************************************************************************************@*/
namespace viva
{
    namespace input
    {
        const int STATE_SIZE = 5;

        class Mouse
        {
        private:
            bool cursorVisibility;
            bool rawCursor;
            Point lastCursorPos;
            bool buffer1[STATE_SIZE];
            bool buffer2[STATE_SIZE];
            bool* curState;
            bool* prevState;
            int mouseWheel;
            Point cursorDelta;
            Point cursorDeltaRaw;
            bool useRaw;
        public:
            Mouse();

            // Get difference between mouse wheel positions from frame to frame.
            int GetWheelDelta() const;

            // Get difference between cursor positions from frame to frame.
            const Point& GetCursorDelta() const;

            // Get cursor position in client coordinates (in pixels from top left corner).
            const Point& GetCursorClientPos() const;

            // Get cursor position in screen coordinates (in pixels from top left corner of the screen).
            const Point& GetCursorPos() const;

            void _SetMouseWheel(int wheel);

            void _SetCursorDeltaRaw(int x, int y);

            // Should cursor delta be read from os or from harware directly.
            // val: yes or no ?
            void UseRawInput(bool val);

            // Show/hide cursor.
            // visible: show or hide
            void ShowCursor(bool visible);

            // Is cursor visible (returns state set by ShowCursor()).
            bool IsCursorVisible() const;

            // Is key down in this frame.
            // key: key to check
            bool IsButtonDown(MouseKey key) const;

            // Is key down in this frame but was up in previous.
            // key: key to check
            bool IsButtonPressed(MouseKey key) const;

            // Is key up in this frame but was down in previous.
            // key: key to check
            bool IsButtonReleased(MouseKey key) const;

            // Sets key state to up.
            void ResetKey(MouseKey key);

            void _Destroy();

            void _Activity();
        };
    }
}

#pragma region code
namespace viva
{
    namespace input
    {
        Mouse::Mouse()
            : lastCursorPos({ 0,0 }),
            mouseWheel(0),
            cursorDelta({ 0,0 }),
            cursorDeltaRaw({ 0,0 }),
            useRaw(false),
            curState(this->buffer1),
            prevState(this->buffer2)
        {
            POINT p;
            ::GetCursorPos(&p);
            this->lastCursorPos.x = (float)p.x;
            this->lastCursorPos.y = (float)p.y;
            memset(this->curState, 0, STATE_SIZE);
            memset(this->prevState, 0, STATE_SIZE);
        }

        void Mouse::_SetMouseWheel(int wheel)
        {
            this->mouseWheel = wheel;
        }

        void Mouse::_SetCursorDeltaRaw(int x, int y)
        {
            this->cursorDeltaRaw.x = (float)x;
            this->cursorDeltaRaw.y = (float)y;
        }

        int Mouse::GetWheelDelta() const
        {
            return this->mouseWheel;
        }

        void Mouse::UseRawInput(bool val)
        {
            this->rawCursor = val;
        }

        const Point& Mouse::GetCursorDelta() const
        {
            return this->useRaw ? this->cursorDeltaRaw : this->cursorDelta;
        }

        const Point& Mouse::GetCursorClientPos() const
        {
            throw Error(__FUNCTION__, "Not implemented");
            return this->lastCursorPos;
        }

        const Point& Mouse::GetCursorPos() const
        {
            return this->lastCursorPos;
        }

        void Mouse::ShowCursor(bool visible)
        {
            ::ShowCursor(visible);
        }

        bool Mouse::IsCursorVisible() const
        {
            return this->cursorVisibility;
        }

        bool Mouse::IsButtonDown(MouseKey key) const
        {
            return this->curState[(int)key];
        }

        bool Mouse::IsButtonPressed(MouseKey key) const
        {
            return this->curState[(int)key] && !prevState[(int)key] && engine->GetFrame() != 0;
        }

        bool Mouse::IsButtonReleased(MouseKey key) const
        {
            return !this->curState[(int)key] && this->prevState[(int)key];
        }

        void Mouse::_Activity()
        {
            // get cursor pos and delta from os
            POINT p;
            ::GetCursorPos(&p);
            this->cursorDelta.x = p.x - this->lastCursorPos.x;
            this->cursorDelta.y = p.y - this->lastCursorPos.y;
            this->lastCursorPos.x = (float)p.x;
            this->lastCursorPos.y = (float)p.y;

            // swap states
            auto temp = this->prevState;
            this->prevState = this->curState;
            this->curState = temp;

            // update current
            this->curState[VK_LBUTTON] = (::GetAsyncKeyState(VK_LBUTTON) & 0x8000) && true;
            this->curState[VK_RBUTTON] = (::GetAsyncKeyState(VK_RBUTTON) & 0x8000) && true;
            this->curState[VK_MBUTTON] = (::GetAsyncKeyState(VK_MBUTTON) & 0x8000) && true;
        }

        void Mouse::ResetKey(MouseKey key)
        {
            this->curState[(int)key] = false;
            this->prevState[(int)key] = false;
        }

        void Mouse::_Destroy()
        {
            delete this;
        }
    }
}
#pragma endregion

        /*@// Keyboard ***************************************************************************************************@*/
namespace viva
{
    namespace input
    {
        class Keyboard
        {
        protected:
            static const int STATE_SIZE = 256;
            bool buf1[STATE_SIZE];
            bool buf2[STATE_SIZE];
            // flags to indicate which keys are chars
            // used by GetChar
            char chars[STATE_SIZE];
            bool* curState;
            bool* prevState;
            bool capslockActive;
            int getChar;
        public:
            Keyboard();

            // Is key down in this frame.
            // key: key to check
            bool IsKeyDown(KeyboardKey key) const;

            // Is key down in this frame but was up in previous.
            // key: key to check
            bool IsKeyPressed(KeyboardKey key) const;

            // Is key up in this frame but was down in previous.
            // key: key to check
            bool IsKeyReleased(KeyboardKey key) const;

            // Returns key typed including all ASCII chars between 32 and 127 and \n \t \b
            // If not applicable char was typed, the function returns 0.
            // enableShift: should I include shift for capitalization
            // enableCapslock: should I enable capslock for capitalization
            int GetChar(bool enableShift = true, bool enableCapslock = true) const;

            //Is capslock active.
            bool IsCapslockActive() const;

            //virtual char GetKey(int offset) const = 0;

            // Sets key state to up.
            void ResetKey(KeyboardKey key);

            // Destroys keyboard object.
            void _Destroy();

            void _Activity();
        };
    }
}

#pragma region code
namespace viva
{
    namespace input
    {
        Keyboard::Keyboard() : capslockActive(false), curState(buf1), prevState(buf2)
        {
            memset(this->buf1, 0, Keyboard::STATE_SIZE);
            memset(this->buf2, 0, Keyboard::STATE_SIZE);

            byte charCodes[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'Q', 'W', 'E', 'R', 'T',
                'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
                'Z', 'X', 'C', 'V', 'B', 'N', 'M', 0xc0, 0xbd, 0xbb, 0xdc, 0xdb,
                0xdd, 0xba, 0xde, 0xbc, 0xbe, 0xbf,
                ' ', 0x0d, '\t', '\b' };

            for (int i = 0; i < Keyboard::STATE_SIZE; i++)
                this->chars[i] = 0;

            for (int i = 0; i < sizeof(charCodes); i++)
                this->chars[charCodes[i]] = charCodes[i];
        }

        void Keyboard::_Activity()
        {
            this->getChar = 0;
            this->capslockActive = ::GetKeyState((int)KeyboardKey::CapsLock) & 1;

            // swap states
            auto temp = this->prevState;
            this->prevState = this->curState;
            this->curState = temp;

            // get button states
            for (int i = 0; i < Keyboard::STATE_SIZE; i++)
            {
                this->curState[i] = (::GetAsyncKeyState(i) & 0x8000) && true;

                if (this->IsKeyPressed((KeyboardKey)i) && this->chars[i] != 0)
                    this->getChar = i;
            }
        }

        bool Keyboard::IsKeyDown(KeyboardKey key) const
        {
            return this->curState[(int)key];
        }

        bool Keyboard::IsKeyPressed(KeyboardKey key) const
        {
            return this->curState[(int)key] && !this->prevState[(int)key] && engine->GetFrame() != 0;
        }

        bool Keyboard::IsKeyReleased(KeyboardKey key) const
        {
            return !this->curState[(int)key] && this->prevState[(int)key];
        }

        int Keyboard::GetChar(bool enableShift, bool enableCapslock) const
        {
            if (this->getChar == 0)
                return 0;

            byte output[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'q', 'w', 'e', 'r', 't',
                'y', 'u', 'i', 'o', 'p', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
                'z', 'x', 'c', 'v', 'b', 'n', 'm', '`', '-', '=', '\\', '[', ']', ';', '\'', ',', '.', '/',
                ' ', '\n', '\t', '\b' };
            byte output2[] = { ')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'Q', 'W', 'E', 'R', 'T',
                'Y', 'U', 'I', 'O', 'P', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
                'Z', 'X', 'C', 'V', 'B', 'N', 'M', '~', '_', '+', '|', '{', '}', ':', '\"', '<', '>', '?',
                ' ', '\n', '\t', '\b' };
            // from combination of capslock and shit, figure out what is the case
            char mod = (enableShift && this->IsKeyDown(KeyboardKey::Shift)) + (enableCapslock && this->capslockActive);

            if (mod == 1)
                return output2[this->chars[this->getChar]];
            else
                return output[this->chars[this->getChar]];

            return 0;
        }

        bool Keyboard::IsCapslockActive() const
        {
            return this->capslockActive;
        }

        /*char Win32Keyboard::GetKey(int offset) const override
        {
        }*/

        void Keyboard::ResetKey(KeyboardKey key)
        {
            this->curState[(int)key] = false;
            this->prevState[(int)key] = false;
        }

        void Keyboard::_Destroy()
        {
            delete this;
        }
    }
}
#pragma endregion

        /*@// Gamepad ****************************************************************************************************@*/
namespace viva
{
    namespace input
    {
        class Gamepad
        {
            //how many gamepads listen to
            //keeps this at minimum for efficiency
            //max count is XUSER_MAX_COUNT (currently 4)
            //virtual void SetGamepadsCount(UINT count) { acceptGamepads = count; }
            //virtual UINT GetGamepadsCount() const { return acceptGamepads; }
            //virtual bool IsAnyButtonDown(UINT gamepad) const;
            //virtual bool IsButtonDown(UINT gamepad, Buttons button) const;
            //virtual bool IsButtonPressed(UINT gamepad, Buttons button) const;
            //virtual bool IsButtonReleased(UINT gamepad, Buttons button) const;
            //virtual BYTE GetLeftTrigger(UINT gamepad) const;
            //virtual BYTE GetRightTrigger(UINT gamepad) const;
            //virtual SHORT GetLeftStickX(UINT gamepad) const;
            //virtual SHORT GetLeftStickY(UINT gamepad) const;
            //virtual SHORT GetRightStickX(UINT gamepad) const;
            //virtual SHORT GetRightStickY(UINT gamepad) const;
            //virtual BYTE GetLeftTriggerDelta(UINT gamepad) const;
            //virtual BYTE GetRightTriggerDelta(UINT gamepad) const;
            //virtual SHORT GetLeftStickXDelta(UINT gamepad) const;
            //virtual SHORT GetLeftStickYDelta(UINT gamepad) const;
            //virtual SHORT GetRightStickXDelta(UINT gamepad) const;
            //virtual SHORT GetRightStickYDelta(UINT gamepad) const;
            //virtual void SetMotorSpeed(UINT gamepad, USHORT speedLeft, USHORT speedRight);

            ////technically: was gamepad connected during last read
            ////generally: is that gamepad connected
            //virtual bool IsGamepadActive(UINT gamepad) const;

            ////was a new gamepad detected between last and current read
            //virtual bool GamepadConnected() const;

            ////was a gamepad lost between last and current read
            //virtual bool GamepadDisconnected() const;
            //virtual void Destroy();
        };
    }
}

#pragma region code
namespace viva
{
    namespace input
    {
    }
}
#pragma endregion

/*@ Control @*/
namespace viva
{
    namespace ui
    {
        struct Style
        {
            Color backgroundColor;
            // overrides backgroundColor
            Texture* backgroundImage;
            Font* font;
            // for text
            Size padding;
            Point position;
            Size size;
            int tabIndex;
            Color textColor;
            int textSize;
            float zindex;
        };

        struct Props
        {
            std::wstring text;
            bool visible;
            bool enableMouseEvents;
            bool focus;
        };

        struct Events
        {
            std::function<void(input::MouseKey)> click;
            std::function<void()> focus;
            std::function<void()> focuslost;
            std::function<void(input::MouseKey)> mousedown;
            std::function<void()> mouseenter;
            std::function<void()> mouseleave;
            std::function<void()> mousemove;
            std::function<void(input::MouseKey)> mouseup;
            std::function<void(float)> mousewheel;
        };

        enum class ControlState : int
        {
            Normal = 0,
            Hover = 1,
            Focus = 2
        };

        class Control :  public Destroyable
        {
        protected:
            Sprite* sprite;
            Text* text;
            Props props;
            Style styles[3];
            Events events;
            Control* parent;
            vector<Control*> children;
            ControlState state;            
        public:
            Control(const char* style);

            void Show(bool show);

            void SetText(const wchar_t* text);

            void EnableMouseEvents(bool enable);

            void SetStyle(const char* style);

            void Update();

            void _AddChild(Control* c);

            void _RemoveChild(Control* c);

            void SetFocus();

            void OnClick(const std::function<void(input::MouseKey)>& handler);
                        
            void OnMouseDown(const std::function<void(input::MouseKey)>& handler);
            
            void OnMouseEnter(const std::function<void()>& handler);
            
            void OnMouseLeave(const std::function<void()>& handler);
            
            void OnMouseMove(const std::function<void()>& handler);
            
            void OnMouseUp(const std::function<void(input::MouseKey)>& handler);
            
            void OnMouseWheel(const std::function<void(float)>& handler);

            void OnFocus(const std::function<void()>& handler);

            void OnLostFocus(const std::function<void()>& handler);

            void Destroy() override;
        };
    }
}

#pragma region code
namespace viva
{
    namespace ui
    {
        Control::Control(const char* style)
        {
            this->Update();
        }

        void Control::Show(bool show)
        {
            this->props.visible = show;
            this->text->SetVisible(show);
            this->sprite->SetVisible(show);

            for (Control* c : this->children)
                c->Show(show);
        }

        void Control::SetText(const wchar_t* text)
        {
            this->text->SetText(text);
        }

        void Control::EnableMouseEvents(bool enable)
        {
            this->props.enableMouseEvents = enable;
        }

        void Control::SetStyle(const char* style)
        {
        }

        void Control::Update()
        {
        }

        void Control::SetFocus()
        {

        }

        void Control::_AddChild(Control* c)
        {
            if (c->parent == this)
                return;

            if (c->parent != nullptr)
                c->parent->_RemoveChild(c);

            this->children.push_back(c);
        }

        void Control::_RemoveChild(Control* c)
        {
            auto it = std::find(this->children.begin(), this->children.end(), c);

            if (it != this->children.end())
                this->children.erase(it);
        }

        void Control::OnClick(const std::function<void(input::MouseKey)>& handler)
        {
            this->events.click = handler;
        }

        void Control::OnMouseDown(const std::function<void(input::MouseKey)>& handler)
        {
            this->events.mousedown = handler;
        }

        void Control::OnMouseEnter(const std::function<void()>& handler)
        {
            this->events.mouseenter = handler;
        }

        void Control::OnMouseLeave(const std::function<void()>& handler)
        {
            this->events.mouseleave = handler;
        }

        void Control::OnMouseMove(const std::function<void()>& handler)
        {
            this->events.mousemove = handler;
        }

        void Control::OnMouseUp(const std::function<void(input::MouseKey)>& handler)
        {
            this->events.mouseup = handler;
        }

        void Control::OnMouseWheel(const std::function<void(float)>& handler)
        {
            this->events.mousewheel = handler;
        }

        void Control::OnFocus(const std::function<void()>& handler)
        {
            this->events.focus = handler;
        }

        void Control::OnLostFocus(const std::function<void()>& handler)
        {
            this->events.focuslost = handler;
        }

        void Control::Destroy()
        {
            for (Control* c : this->children)
                c->Destroy();
            
            // in case parent is not destroyed
            // remove this from parent's list to prevent data corruption
            if (this->parent != nullptr)
                this->parent->_RemoveChild(this);
            else
                uiManager->_RemoveControl(this);

            if (this->sprite != nullptr)
                this->sprite->Destroy();

            if (this->text != nullptr)
                this->text->Destroy();

            delete this;
        }
    }
}
#pragma endregion

/*@ UI Manager @*/
namespace viva
{
    namespace ui
    {
        class UIManager
        {
        private:
            vector<Control*> roots;
        public:
            void _Activity();

            Control* AddControl(const char* style);

            Control* AddControl(const char* style, Control* parent);

            void _RemoveControl(Control* c);

            void _Destroy();
        };
    }
}

#pragma region code
namespace viva
{
    namespace ui
    {
        void UIManager::_Activity()
        {
        }

        Control* UIManager::AddControl(const char* style)
        {
            Control* c = new Control(style);
            this->roots.push_back(c);
            return c;
        }

        Control* UIManager::AddControl(const char* style, Control* parent)
        {
            Control* c = new Control(style);
            parent->_AddChild(c);
            return c;
        }

        void UIManager::_RemoveControl(Control* c)
        {
            auto it = std::find(this->roots.begin(), this->roots.end(), c);

            if (it != this->roots.end())
                this->roots.erase(it);
        }

        void UIManager::_Destroy()
        {
            for (Control* c : this->roots)
                c->Destroy();
        }
    }
}
#pragma endregion

/*@// G L O B A L    V A R I A B L E S ***********************************************************************************@*/
namespace viva
{
    Engine* engine;
    Camera* camera;
    Creator* creator;
    DrawManager* drawManager;
    Window* window;
    ResourceManager* resourceManager;
    input::Mouse* mouse;
    input::Keyboard* keyboard;
    RoutineManager* routineManager;
    Time* time;
    net::NetworkManager* networkManager;
    ui::UIManager* uiManager;
    D3D11 d3d;
}
