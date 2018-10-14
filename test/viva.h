#pragma once
#include <vector>
#include <string>
#include <functional>

namespace viva
{
    namespace math
    {
        struct alignas(16) vector
        {
            union
            {
                struct { float x, y, z, w; };
            };

            vector(float x, float y, float z, float w);
            vector(float x, float y, float z);
            vector(float x, float y);
            vector();

            void xy(float x, float y);
            void xyz(float x, float y, float z);
            void add_xy(float x, float y);
            void add_xyz(float x, float y, float z);
            void mul_xy(float x, float z);
            void mul_xyz(float x, float y, float z);

            vector operator + (const vector& a);
            vector operator - (const vector& a);
            vector operator - ();
            vector operator * (float a);
            vector& operator += (const vector& a);
            vector& operator -= (const vector& a);
        };
    }
}

namespace viva
{
    using std::vector;
    typedef unsigned char byte;
    typedef unsigned int uint;
    typedef void* HWND;

    class Animation;
    class Engine;
    class Surface;
    class PixelShader;
    class Camera;
    class Creator;
    class DrawManager;
    class Window;
    class RoutineManager;
    class Time;
    class Sprite;
    class Text;
    struct Routine;

    namespace input
    {
        class Mouse;
        class Keyboard;
    }

    namespace net
    {
        class Server;
        class Client;
    }

    typedef math::vector Vector;

    extern Engine* engine;
    extern Camera* camera;
    extern Creator* creator;
    extern DrawManager* drawManager;
    extern Window* window;
    extern input::Mouse* mouse;
    extern input::Keyboard* keyboard;
    extern RoutineManager* routineManager;
    extern Time* time;

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

    // Tranformable and destroyable.
    class Destroyable
    {
    public:
        // Remove from all managers and memory.
        virtual void Destroy() = 0;
    };

    /*@ Rect ********************************************************************************************************@*/
        // xyz
    struct Rect
    {
        float left, top, right, bottom;
        // xyz 
        Rect();
        // xyz 
        Rect(float _left, float _top, float _right, float _bottom);
    };

    /*@ Action *******************************************************************************************************@*/
        // xyz 
    struct Action
    {
        vector<Rect> uvTable;
        double speed;
        int length;

        Action(const vector<Rect>& _uvTable, double _speed);
    };

    /*@/ Point ********************************************************************************************************@*/
        // xyz 
    struct Point
    {
        float x, y;
        // xyz 
        Point();
        // xyz 
        Point(float x, float y);
    };

    /*@// Color ********************************************************************************************************@*/
        // xyz 
    struct Color
    {
        byte r, g, b, a;

        // color rgba(255,255,255,255)
        Color();
        // each value should be 0 to 1
        Color(byte _r, byte _g, byte _b, byte _a);
    };

    /*@// Size *********************************************************************************************************@*/
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

    /*@// Vertex *******************************************************************************************************@*/
        // xyz 
    struct Vertex
    {
        float x, y, z, r, g, b, u, v;
        // xyz 
        Vertex();
        // xyz 
        Vertex(float _x, float _y, float _z, float _r, float _g, float _b, float _u, float _v);
    };

    class Error : public std::runtime_error
    {
    public:
        const char* function;

        // Ctor.
        // fun: should be a function that throws that exception
        // msg: custom message, like what happened ?
        Error(const char* fun, const char* msg);
    };

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
    }

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
                freeObjects.push_back(pool.at(i));
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

    class VertexBuffer : public Destroyable
    {
    public:
        void Destroy();

        int GetVertexCount() const;
    };

    // 8L Twin Turbo W16
    class Engine
    {
    public:
        // Start engine (game loop).
        // gameloop: user's function called every frame
        CloseReason Run(const std::function<void()>& gameloop = []() {});

        // Get background color. Background color is the color being drawn if there's nothing there.
        const Color& GetBackgroundColor() const;

        // Set background color. Background color is the color being drawn if there's nothing there.
        // color: the color
        void SetBackgroundColor(const Color& color);

        // Get viewport/client size. Client is the drawing area in window.
        const Size& GetClientSize() const;

        void GetScreenshot(vector<Color>& dst) const;

        // Stop engine. Makes Run method return.
        void Exit();

        // Open console.
        void OpenConsole();

        // Close console.
        void CloseConsole();
    };

    class Time
    {
    public:
        // Time elapsed in seconds since the beginning of the game.
        double GetGameTime() const;

        // Time elapsed since last frame.
        double GetFrameTime() const;

        // Frames per second.
        double GetFps() const;
    };

    // Transform object responsible for position, rotation, scale etc. of its owner.
    class Transform
    {
    public:
        Transform* SetCoordMode(TransformMode m);

        TransformMode GetMode() const;

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
    };
    
    class Drawable : public Destroyable
    {
    public:
        virtual Surface* GetSurface() const = 0;

        virtual bool IsVisible() const = 0;

        virtual Drawable* SetVisible(bool val) = 0;

        const Color& GetColor() const;

        Drawable* SetColor(byte r, byte g, byte b, byte a);

        Drawable* SetColor(const Color& c);

        Drawable* SetR(byte r);

        Drawable* SetG(byte g);

        Drawable* SetB(byte b);

        Drawable* SetA(byte a);

        void SetExtraBufferPSdata(void* data);
    };

    class Surface
    {
    public:
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

    class Polygon : public Drawable
    {
    public:
        // Get transform of the object.
        Transform* T();

        // Get transform of the object.
        Transform* GetTransform();

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

        void Destroy() override;
    };

    class Texture : public Destroyable
    {
    public:
        const Size& GetSize() const;

        void Destroy();
    };

    struct CharacterMetrics
    {
        int id;
        Rect uv;
        Size size;
        Point offset;
        float advance;
    };

    struct FontMetrics
    {
        float lineHeight;
    };

    // Bitmap font. Stores coordinates for where letters are on texture
    class Font : public Destroyable
    {
    public:
        // Create bitmap font from texture. And calc primitive metrics
        // tex: texture to use
        // glyphs: exact uv coordinate for glyphs. It should contain at least ascii 0-126
        Font(Texture* tex, const char* fontMetrics, bool);

        // Create bitmap font from texture.
        Font(Texture* tex, const char* fontMetricsFile);

        // Gets uv coordinate for char 'code'.
        const CharacterMetrics& GetChar(uint code) const;

        const FontMetrics& GetFontMetrics() const;

        Texture* GetTexture() const;

        void Destroy();
    };

    class RoutineManager
    {
    public:

        // Add a new routine.
        //func: code to execute by event (function pointer or closure)
        //      func must return an int. This is to make it easier to remove routines from inside. 
        //      If func returns 0, routine is destroyed.
        //delay: how long to wait before start running
        //lifeTime: destroy event after that time, 0 to never destroy
        //tick: run event once every tick seconds, 0 to run every frame
        Routine* AddRoutine(const std::function<int()>& func, int id, double delay, double lifeTime, double tick);

        Routine* AddRoutine(const std::function<int()>& func);

        // Find routine.
        // Note, this method performs linear search to find by name.
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

        void RemoveRoutine(Routine* r);
    };

    class PixelShader : public Destroyable
    {
    public:
        void Destroy();
    };

    class Camera
    {
    public:
        // Returns screen coordinates in pixels of the given world coordinates
        Point WorldToScreen(const Vector& pos) const;

        Vector ScreenToWorld(const Point& pos) const;

        Size Pixel2World(const Size& in) const;

        Size World2Pixel(const Size& in) const;

        const Size& GetFrustumSize() const;

        Transform* GetLookAt();
    };

    class Sprite : public Drawable
    {
    public:
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

        void Destroy() override;
    };

    class Animation : public Drawable
    {
    public:
        // Get transform of the object.
        Transform* T();

        // Get transform of the object.
        Transform* GetTransform();
        
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

        void SetAction(int index);

        Action* GetAction() const;

        void AddAction(double speed, int columns, int rows, int first, int last);

        void AddAction(double speed, const vector<Rect>& uvTable);

        void AddAction(double speed, const Size& texSizePx, const vector<Rect>& uvTablePx);

        void AddOnFrameChangeHandler(const std::function<void(int)>& fun);

        void AddOnActionLoopedHandler(const std::function<void(int)>& fun);

        void ClearOnFrameChangeHandlers();

        void ClearAddOnActionLoopedHandlers();

        Surface* GetSurface() const override;

        bool IsVisible() const override;

        Drawable* SetVisible(bool val) override;

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

    // Factory for all objects.
    class Creator
    {
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

        Text* CreateText(const wchar_t* text);

        Text* CreateText(const wchar_t* text, Font* font);

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
    };

    class Text : public Sprite
    {
    public:

        Text* SetText(const wchar_t* str);

        const wchar_t* GetText() const;
    };

    class DrawManager
    {
    public:
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

        Text* AddText(const wchar_t* text);

        Text* AddText(const wchar_t* text, Font* font);

        Text* AddText(const wchar_t* text, Surface* surface);

        Text* AddText(const wchar_t* text, Font* font, Surface* surface);

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

        Surface* AddSurface();

        Surface* GetDefaultSurface();

        void AddSurface(Surface* s);

        void RemoveSurface(Surface* s);

        void MoveSurfaceUp(Surface* s);

        void MoveSurfaceDown(Surface* s);

        void MoveSurfaceTop(Surface* s);

        void MoveSurfaceBottom(Surface* s);

        void ResizeExtraPSBuffer(uint size);

        // Destroy all surfaces (except the default one) and all object from the default surface.
        void Clear();
    };

    class Window
    {
    public:
        HWND GetHandle() const;

        void SetWindowTitle(const char* title);
    };

    struct VivaParams
    {
        const char* title;
        Size size;
        Size unit;
    };

    class Viva
    {
    public:
        // Initialize viva.
        // title: window title
        // size: size of the viewport
        // path: the default path,
        Viva(VivaParams& params);

        // Destructor.
        ~Viva();
    };

    namespace net
    {
        struct NetworkError
        {
            std::string message;
            int code;
        };

        class Client
        {
        public:
            void OnConnect(const std::function<void()>& handler);

            void OnMsg(const std::function<void(const vector<byte>&)>& handler);

            const char* GetIp() const;

            void Send(vector<byte>& msg);

            void Send(byte* msg, unsigned short len);

            void Connect(double timeoutSeconds);

            void Disonnect();

            void Destroy();

            bool GetReturnReceive() const;
        };

        class Server
        {
        public:
            void OnConnect(const std::function<void(Client* c)>& handler);

            void OnDisconnect(const std::function<void(Client* c)>& handler);

            const vector<Client*>& GetClients() const;

            bool GetReturnAccept() const;

            void Start(int backlog);

            void Stop();

            void Destroy();
        };
    }

    namespace input
    {
        class Mouse
        {
        public:
            // Get difference between mouse wheel positions from frame to frame.
            int GetWheelDelta() const;

            // Get difference between cursor positions from frame to frame.
            const Point& GetCursorDelta() const;

            // Get cursor position in client coordinates (in pixels from top left corner).
            const Point& GetCursorClientPos() const;

            // Get cursor position in screen coordinates (in pixels from top left corner of the screen).
            const Point& GetCursorPos() const;

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
        };

        class Keyboard
        {
        public:
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
            char GetChar(bool enableShift = true, bool enableCapslock = true) const;

            //Is capslock active.
            bool IsCapslockActive() const;

            // 
            //virtual char GetKey(int offset) const = 0;

            // Sets key state to up.
            void ResetKey(KeyboardKey key);
        };
    }
}