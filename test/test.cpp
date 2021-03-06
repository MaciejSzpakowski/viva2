#include "viva.h"
#pragma comment(lib,"../x64/Debug/viva2.lib")
#include <cstdio>
#include <cmath>
#include <functional>
#include <vector>
#include <chrono>

int currentTest = 0;
std::vector<std::function<void()>> tests;
std::vector<viva::Destroyable*> resources;

void AddText(const wchar_t* str)
{
    viva::Text* text1 = viva::drawManager->AddText(str);
    text1->T()->SetCoordMode(viva::TransformMode::Screen);
    text1->SetColor(0, 0, 0, 255);
    text1->T()->Pos().xy(0, 22);
}

void testCoords()
{
    const wchar_t* msg = L" !\"#$%&'()*+,-./0123456789:;<=>?@\n"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`\n"
        "abcdefghijklmnopqrstuvwxyz{|}~\n\n"
        "Coordinates Test:\n"
        "violet axes with origin in the middle of the world and green unit squares\n"
        "black circle diameter=1 at (-10,10), red square width=1 at (10, -1)\n"
        "red triangle with points [(7,7),(9,7),(8,9)]"
        ;
    AddText(msg);
    
    {
        // circle
        auto c = viva::drawManager->AddCircle();
        c->T()->Pos().xy(-10, 10);
        c->SetColor(0, 0, 0, 255);
    }

    // square
    auto sq = viva::drawManager->AddRectangle();
    sq->T()->Pos().xy(10, -1);
    sq->SetColor(255, 0, 0, 255);

    // triangle
    viva::vector<viva::Point> v({ { 7,7 },{ 9,7 },{ 8,9 },{ 7,7 } });
    viva::Polygon* tri = viva::drawManager->AddPolygon(v);
    tri->SetColor(255, 0, 0, 255);

    // x axis
    viva::vector<viva::Point> v2({ { -100,0 } ,{ 100,0 } });
    viva::Polygon* p1 = viva::drawManager->AddPolygon(v2);
    p1->SetColor(100, 0, 255, 255);

    // y axis
    viva::vector<viva::Point> v1({ { -0,-100 } ,{ 0,100 } });
    viva::Polygon* p2 = viva::drawManager->AddPolygon(v1);
    p2->SetColor(100, 0, 255, 255);

    // unit squares
    for (int i = -20; i < 20; i++)
    {
        for (int j = -30; j < 30; j++)
        {
            auto s = viva::drawManager->AddRectangle();
            s->T()->Pos().xy((float)j, (float)i);
            s->SetColor(0, 255, 100, 255);
        }
    }
}

void testPolygons()
{
    const wchar_t* msg = L"Polygons and colors\n"
                      "Red star on the left, blue rectangle in the middle, \n"
                      "rainbow circle on the right";
    AddText(msg);

    viva::vector<viva::Point> v;
    for (int i = 0; i < 10; i += 2)
    {
        v.push_back({ sinf(0.31415f*i * 2), cosf(0.31415f*i * 2) });
        v.push_back({ sinf(0.31415f*(i + 1) * 2) * 2, cosf(0.31415f*(i + 1) * 2) * 2 });
    }
    v.push_back(*v.begin());
    viva::Polygon* p1 = viva::drawManager->AddPolygon(v);
    p1->T()->Pos().x = -17;
    p1->T()->Scale().xy(2, 2);
    p1->SetColor(255, 0, 0, 255);

    viva::Polygon* r1 = viva::drawManager->AddRectangle();
    r1->T()->Scale().xy(10, 3);
    r1->SetColor(0, 0, 255, 255);
    
    viva::Polygon* c1 = viva::drawManager->AddCircle(20);
    c1->T()->Scale().xy(4, 4);
    c1->T()->Pos().x = 16;

    viva::routineManager->AddRoutine([c1]
    {
        c1->SetColor({
            (viva::byte)(sin(viva::time->GetGameTime()) * 255),
            (viva::byte)(cos(viva::time->GetGameTime()*0.6) * 255),
            (viva::byte)(sin(viva::time->GetGameTime() *0.5) * 255), 255 });
        return 1;
    });
}

void testSprites()
{
    const wchar_t* msg = L"Texture and sprites\n"
        "On the left, brick 200x200 in pixels (take a screenshot and check)\n"
        "On the right, red-green checkerboard 200x200\n"
        "In the middle top, leaf in pink square (should be proportional)\n"
        "In the middle bottom, quarter leaf in pink square\n"
        "Press 1 to hide everything";
    AddText(msg);

    viva::Texture* texBrick = viva::creator->CreateTexture("brick.jpg");
    resources.push_back(texBrick);
    viva::Sprite* s1 = viva::drawManager->AddSprite(texBrick);
    s1->T()->SetPixelScale({ 200, 200 });
    s1->T()->Pos().x = -14;

    std::vector<viva::Color> pixels;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            pixels.push_back(viva::Color( 255 - 255 * ((i + j) % 2), 255 * ((i + j) % 2), 0, 255 ));

    viva::Texture* t1 = viva::creator->CreateTexture(pixels.data(), { 8, 8 });
    resources.push_back(t1);
    viva::Sprite* s2 = viva::drawManager->AddSprite(t1);
    s2->T()->SetPixelScale({ 200, 200 });
    s2->T()->Pos().x = 14;

    viva::Texture* texLeaf = viva::creator->CreateTexture("leaf.png");
    resources.push_back(texLeaf);
    viva::Sprite* s3 = viva::drawManager->AddSprite(texLeaf);
    s3->SetScale2TextureSize();
    s3->T()->Scale().mul_xy(0.5f, 0.5f);
    s3->T()->Pos().y = 10;

    viva::Polygon* p1 = viva::drawManager->AddRectangle();
    p1->T()->Scale() = s3->T()->Scale();
    p1->T()->Pos().y = 10;
    p1->SetColor(255, 0, 255, 255);

    viva::Sprite* s4 = viva::drawManager->AddSprite(texLeaf);
    s4->SetUV({ 0, 0, 0.5f, 0.5f });
    s4->SetScale2TextureSize();
    s4->T()->Scale().mul_xy(0.5f, 0.5f);
    s4->T()->Pos().y = -10;

    viva::Polygon* p2 = viva::drawManager->AddRectangle();
    p2->T()->Scale() = s4->T()->Scale();
    p2->T()->Pos().y = -10;
    p2->SetColor(255, 0, 255, 255);
}

void testAnimation()
{
    const wchar_t* msg = L"Animation\n"
        "On the left, cat running to the right\n"
        "On the right, cat running slower backwards and flipped hori.\n";
    AddText(msg);

    const int ACTION_RUN = 0;

    viva::Texture* aniTex = viva::creator->CreateTexture("ani.png");
    resources.push_back(aniTex);
    viva::Animation* ani = viva::drawManager->AddAnimation(aniTex);
    ani->AddAction(20, 2, 4, 0, 7);
    ani->SetAction(ACTION_RUN);
    ani->T()->SetPixelScale(512, 256);
    ani->T()->Scale().mul_xy(0.5f,0.5f);
    ani->T()->Pos().xy(-10, 8);

    viva::Animation* ani2 = viva::drawManager->AddAnimation(aniTex);
    ani2->AddAction(-7, 2, 4, 0, 7);
    ani2->SetAction(ACTION_RUN);
    ani2->T()->SetPixelScale(512, 256);
    ani2->T()->Scale().mul_xy(0.5f, 0.5f);
    ani2->T()->Pos().xy(10, 8);
    ani2->SetFlipHorizontally(true);

    viva::Animation* ani3 = viva::drawManager->AddAnimation(aniTex);
    ani3->AddAction(20, 2, 4, 0, 7);
    ani3->SetAction(ACTION_RUN);
    ani3->T()->SetPixelScale(512, 256);
    ani3->T()->Scale().mul_xy(0.5f, 0.5f);
    ani3->T()->Pos().xy(-10, -8);
    ani3->SetFlipVertically(true);

    viva::Animation* ani4 = viva::drawManager->AddAnimation(aniTex);
    ani4->AddAction(-7, 2, 4, 0, 7);
    ani4->SetAction(ACTION_RUN);
    ani4->T()->SetPixelScale(512, 256);
    ani4->T()->Scale().mul_xy(0.5f, 0.5f);
    ani4->T()->Pos().xy(10, -8);
    ani4->SetFlipHorizontally(true);
    ani4->SetFlipVertically(true);
}

void testCreateSprites()
{
    std::wstring msg = L"Reuse vs recreate textures (size 128x128)\n";
    std::vector<viva::Color> v;
    for (int i = 0; i < 128 * 128; i++)
        v.push_back({ 255, 0, 0, 255 });

    auto time1 = std::chrono::steady_clock::now();
    viva::Texture* t1;

    //// TODO: memory leaks

    for (int i = 0; i < 10000; i++)
    {
        t1 = viva::creator->CreateTexture(v.data(), { 128, 128 });
        resources.push_back(t1);
        viva::drawManager->AddSprite(t1);
    }

    auto time2 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    msg += L"Creating 10000 textures and sprites for that textures ";
    msg += std::to_wstring(ms);
    msg += L" ms\n";

    // this deletes FPS text which crashes program
    //viva::drawManager->Clear();

    time1 = std::chrono::steady_clock::now();
    t1 = viva::creator->CreateTexture(v.data(), { 128, 128 });
    resources.push_back(t1);

    for (int i = 0; i < 10000; i++)
    {
        viva::drawManager->AddSprite(t1);
    }

    time2 = std::chrono::steady_clock::now();
    ms = std::chrono::duration_cast<std::chrono::milliseconds>(time2 - time1).count();
    msg += L"Creating 10000 sprites using cached texture ";
    msg += std::to_wstring(ms);
    msg += L" ms";

    AddText(msg.c_str());
}

void testTransform()
{
    const wchar_t* msg = L"Transformation\n"
        "left leaf, spin ccw\n"
        "moddle leaf, alternate spinning\n"
        "left brick, spin cw around its corner\n"
        "middle brick, grow/shrink and stretch on x axis\n"
        "right brick, bubble up/ fall down";
    AddText(msg);

    viva::Texture* leafTex = viva::creator->CreateTexture("leaf.png");
    resources.push_back(leafTex);
    viva::Texture* brickTex = viva::creator->CreateTexture("brick.jpg");
    resources.push_back(brickTex);

    {
        viva::Sprite* s = viva::drawManager->AddSprite(leafTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.3f, 0.3f);
        s->T()->Pos().xy(-15, 4);
        s->T()->RotVel() = 2;
    }
    {
        viva::Sprite* s = viva::drawManager->AddSprite(brickTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.3f, 0.3f);
        s->T()->Pos().xy(-11, 4);
        s->T()->RotVel() = -3;
        s->T()->Origin().xy(-0.5f, 0.5f);
    }
    {
        viva::Sprite* s = viva::drawManager->AddSprite(brickTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.1f, 0.1f);
        s->T()->Pos().xy(0, 4);
        s->T()->ScaleAcc().xy(5, 5);

        viva::routineManager->AddRoutine([=]()
        {
            if (s->T()->ScaleVel().x > 3.0f)
                s->T()->ScaleAcc().xy(-5, -5);
            if (s->T()->ScaleVel().x < -3.0f)
                s->T()->ScaleAcc().xy(5, 5);

            return 1;
        });
    }
    {
        viva::Sprite* s = viva::drawManager->AddSprite(brickTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.1f, 0.1f);
        s->T()->Pos().xy(10, 4);
        s->T()->Acc().y = 10;

        viva::routineManager->AddRoutine([=]()
        {
            if (s->T()->Pos().y > 10)
            {
                s->T()->Vel().y = 0;
                s->T()->Acc().y = -10;
            }
            else if (s->T()->Pos().y < -10)
            {
                s->T()->Vel().y = 0;
                s->T()->Acc().y = 10;
            }

            return 1;
        });
    }
    {
        viva::Sprite* s = viva::drawManager->AddSprite(leafTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.3f, 0.3f);
        s->T()->Pos().xy(0, -4);
        s->T()->RotAcc() = 1;

        viva::routineManager->AddRoutine([=]()
        {
            if (s->T()->RotVel() > 10)
                s->T()->RotAcc() = -1;
            if (s->T()->RotVel() < -10)
                s->T()->RotAcc() = 1;

            return 1;
        });
    }
    {
        viva::Sprite* s = viva::drawManager->AddSprite(leafTex);
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.3f, 0.3f);
        s->T()->Pos().xy(0, -4);
        s->T()->RotAcc() = 1;

        viva::routineManager->AddRoutine([=]()
        {
            if (s->T()->RotVel() > 10)
                s->T()->RotAcc() = -1;
            if (s->T()->RotVel() < -10)
                s->T()->RotAcc() = 1;

            return 1;
        });
    }
}

void testPerformance1()
{
    const wchar_t* msg = L"Performance test";
    AddText(msg);

    for (int i = 0; i < 2000; i++)
    {
        auto c = viva::drawManager->AddCircle(10);
        c->SetColor(0, 0, 0, 255);
        c->T()->Pos().xy((float)viva::math::RandomDouble() * 30 - 15, (float)viva::math::RandomDouble() * 25 - 12);
        //c->TransformVertices(true);
        c->T()->RotVel() = (float)viva::math::RandomDouble() * 3 - 1.5f;
    }
}

void testSurfaceAndPostShader()
{
    const wchar_t* msg = L"Test10: Render targets and pixel shaders\n"
        "This text is in the top render target not affected by pixel shader\n";
    AddText(msg);

    static float time1 = (float)viva::time->GetGameTime();
    viva::routineManager->AddRoutine([=]()
    {
        time1 = (float)viva::time->GetGameTime();
        return 1;
    });

    viva::Texture* brickTex = viva::creator->CreateTexture("brick.jpg");
    resources.push_back(brickTex);
    viva::Texture* leafTex = viva::creator->CreateTexture("leaf.png");
    resources.push_back(leafTex);

    viva::Surface* surface = viva::drawManager->AddSurface();

    {
        // NOTES
        auto& frustum = viva::camera->GetFrustumSize();
        viva::Sprite* s = viva::drawManager->AddSprite(brickTex, surface);
        s->T()->Pos().add_xy(-frustum.width / 2.0f, frustum.height / 2.0f);
        s->T()->Scale().xy(frustum.width, frustum.height);
        s->T()->Pos().z = 0.1f;
    }

    {
        viva::Sprite* s = viva::drawManager->AddSprite(leafTex, surface);
        s->T()->Pos().x = -10;
        s->T()->Pos().z = 0;
        s->T()->RotVel() = 1;
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.5f, 0.5f);
    }

    {
        viva::Sprite* s = viva::drawManager->AddSprite(leafTex, surface);
        s->T()->Pos().x = 10;
        s->T()->Pos().z = 0;
        s->T()->RotVel() = -1;
        s->SetScale2TextureSize();
        s->T()->Scale().mul_xy(0.5f, 0.5f);
    }

    // background for text
    {
        viva::Sprite* s = viva::drawManager->AddFillRectangle();
        s->T()->SetCoordMode(viva::TransformMode::Screen);
        s->T()->Scale().xy(1000, 70);
    }
    
    surface->SetExtraBufferPSdata(&time1);

    const char ps2[] = R"(
			cbuffer cbBufferPS
			{
				float4 color;
			};

        			cbuffer cbExtra
			{
				float var[4];
			};

        			Texture2D ObjTexture;
			SamplerState ObjSamplerState;

        			struct VS_OUTPUT
			{
				float4 Pos : SV_POSITION;
				float3 Col : COLOR;
				float2 TexCoord : TEXCOORD;
			};

        			float4 main(VS_OUTPUT input) : SV_TARGET
			{
				float2 coord = input.TexCoord;
				coord[0] += sin(var[0]+coord[1]*2) * 0.4;
				coord[1] += sin(var[0]+coord[0]*2) * 0.4;
				float4 result = ObjTexture.Sample(ObjSamplerState, coord);
	
				clip(result.a - 0.001f);
				return result * color;
			}
		)";

    {
        viva::PixelShader* ps = viva::creator->CreatePixelShader(ps2);
        resources.push_back(ps);
        surface->SetPixelShader(ps);
    }
}

void testThruput()
{
    const wchar_t* msg = L"Test11: Memory test\n"
        "Open task manager and check if memory is leaking\n";
    AddText(msg);

    const char* psStr = R"(
        cbuffer cbBufferPS : register(b0) { float4 color; };
        Texture2D ObjTexture;
        SamplerState ObjSamplerState;
        struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };
        float4 main(VS_OUTPUT input) :SV_TARGET
        {
            return float4(input.Pos[0]/1920.0f,input.Pos[1]/1080.0f,input.Pos[2],1);
        };
    )";

    viva::PixelShader* ps = viva::creator->CreatePixelShader(psStr);
    resources.push_back(ps);

    viva::routineManager->AddRoutine([=]
    {
        viva::Sprite* s = viva::drawManager->AddFillRectangle();
        s->SetPixelShader(ps);
        s->SetColor({ (viva::byte)viva::math::RandomInt(0,255), 
            (viva::byte)viva::math::RandomInt(0,255), 
            (viva::byte)viva::math::RandomInt(0,255), 255 });
        s->T()->Vel().xy((float)viva::math::RandomDouble() * 10 - 5, (float)viva::math::RandomDouble() * 10 - 5);
        float scale = (float)viva::math::RandomDouble() + 0.2f;
        s->T()->Scale().xy(scale, scale);
        s->T()->RotVel() = (float)viva::math::RandomDouble() * 3 - 1.5f;

        auto r = viva::routineManager->AddRoutine([s]
        {
            s->Destroy();
            return 0;
        }, 0, 5, 0, 0.02f);

        if (r == nullptr)
            s->Destroy();

        return 1;

    });
}

void testServerClient()
{
    viva::engine->OpenConsole();

    printf("testing net\n");

    viva::net::Server* server = viva::creator->CreateServer(10000);
    viva::net::Client* client = viva::creator->CreateClient("127.0.0.1", 10000);

    server->OnConnect([](viva::net::Client* client)
    {
        printf("Server accepted\n");
    });

    client->OnConnect([]()
    {
        printf("Client connected\n");
    });

    client->OnMsg([](const std::vector<viva::byte>& msg)
    {
        printf("Got msg: %s\n", msg.data());
    });

    server->Start(100);

    viva::routineManager->AddRoutine([=]()
    {
        if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::KeyC))
        {
            printf("Conecting...\n");
            client->Connect(3);
        }
        if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::KeyS))
        {
            char buf[100];
            printf("Sending:\n");
            fgets(buf, 100, stdin);
            server->GetClients()[0]->Send((viva::byte*)buf, strlen(buf));
        }

        return 1;
    });
}

void testChildParent()
{
    const wchar_t* msg = L"Single parent, absolute vs relative position,\n"
        "pixel perfect scale for a child\n"
        "1 to select red, 2 to select green";
    AddText(msg);

    std::vector<viva::Sprite*> sprites;

    for (int i = 0; i < 10; i++)
    {
        viva::Sprite* s1 = viva::drawManager->AddFillRectangle();
        s1->SetColor({ (viva::byte)(220 - i * 20), (viva::byte)(220 - i * 20), (viva::byte)(220 - i * 20), 255 });
        s1->T()->Pos().xy(-2, 2);
        s1->T()->Scale().xy(3 - i * 0.2f, 3 - i * 0.2f);

        if (i > 0)
            s1->T()->SetParent(sprites.back()->T());

        sprites.push_back(s1);
    }

    /*viva::Sprite* s1 = viva::drawManager->AddFillRectangle();
    s1->SetColor({ 255, 0, 0, 255 });
    viva::Sprite* s2 = viva::drawManager->AddFillRectangle();
    s2->T()->Scale().xy(0.6f, 0.6f);
    s2->SetColor({ 0, 255, 0, 255 });
    s2->T()->SetParent(s1->T());
    viva::Sprite* s3 = viva::drawManager->AddFillRectangle();
    s3->T()->Scale().xy(0.3f, 0.3f);
    s3->SetColor({ 0, 0, 255, 255 });
    s3->T()->SetParent(s2->T());

    printf("%p\n", s1);
    printf("%p\n", s2);
    printf("%p\n", s3);*/

    viva::routineManager->AddRoutine([=]
    {
        static viva::Transform* activeObj = sprites[0]->T();
        
        if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key1))
            activeObj = sprites[0]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key2))
            activeObj = sprites[1]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key3))
            activeObj = sprites[2]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key4))
            activeObj = sprites[3]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key5))
            activeObj = sprites[4]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key6))
            activeObj = sprites[5]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key7))
            activeObj = sprites[6]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key8))
            activeObj = sprites[7]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key9))
            activeObj = sprites[8]->T();
        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::Key0))
            activeObj = sprites[9]->T();

        for (int i = 0; i < 10; i++)
        {
            sprites[i]->T()->Vel().xy(0, 0);
            sprites[i]->T()->RotVel() = 0;
        }

        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyA))
            activeObj->Vel().x = -2;
        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyD))
            activeObj->Vel().x = 2;
        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyW))
            activeObj->Vel().y = 2;
        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyS))
            activeObj->Vel().y = -2;
        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyQ))
            activeObj->RotVel() = 1;
        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::KeyE))
            activeObj->RotVel() = -1;

        return 1;
    });
}

float testExtraBufferVar[] = { 1,1,1,1 };
void testExtraBuffer()
{
    const wchar_t* msg = L"Test extra buffer";
    AddText(msg);

    viva::Texture* t = viva::creator->CreateTexture("brick.jpg");
    resources.push_back(t);
    viva::Sprite* s = viva::drawManager->AddSprite(t);
    s->SetColor({ 100,100,255,255 });
    s->T()->Scale().mul_xy(12, 12);
    s->SetExtraBufferPSdata(testExtraBufferVar);

    const char* psStr = R"(
    cbuffer cbBufferPS : register(b0) { float4 color; };
    cbuffer cbBufferPS : register(b1) { float4 var; };
    Texture2D ObjTexture;
    SamplerState ObjSamplerState;
    struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };
    float4 main(VS_OUTPUT input) :SV_TARGET
    {
    float4 result = ObjTexture.Sample(ObjSamplerState,input.TexCoord);
    clip(result.a - 0.001f);
    return result * color * sin(var[0]);
    };
    )";

    viva::PixelShader* ps = viva::creator->CreatePixelShader(psStr);
    resources.push_back(ps);

    s->SetPixelShader(ps);

    viva::routineManager->AddRoutine([]()
    {
        testExtraBufferVar[0] += 0.1f;
        return 1;
    }, 0, 0, 0, 1);
}

void testUi()
{

}

bool wrapper(std::function<void()> test)
{
    viva::VivaParams params;
    memset(&params, 0, sizeof(params));
    params.size = { 1920,1080 };
    params.title = "Tests";
    viva::Viva v(params);
    viva::engine->SetBackgroundColor({ 255, 255, 255, 255 });
    viva::engine->OpenConsole();
    bool quit = false;

    viva::Text* fpsText = viva::drawManager->AddText(L"");
    fpsText->T()->SetCoordMode(viva::TransformMode::Screen);
    fpsText->SetColor(0, 0, 0, 255);

    // routine
    viva::routineManager->AddRoutine([]()
    {
        viva::camera->GetLookAt()->Vel().xy(0, 0);

        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::Left))
        {
            viva::camera->GetLookAt()->Vel().x = -1.f;
        }
        else if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::Right))
        {
            viva::camera->GetLookAt()->Vel().x = 1.f;
        }

        if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::Up))
        {
            viva::camera->GetLookAt()->Vel().y = 1.f;
        }
        else if (viva::keyboard->IsKeyDown(viva::input::KeyboardKey::Down))
        {
            viva::camera->GetLookAt()->Vel().y = -1.f;
        }

        if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::PageDown) && currentTest > 0)
        {
            currentTest--;
            viva::engine->Exit();
        }

        else if (viva::keyboard->IsKeyPressed(viva::input::KeyboardKey::PageUp) && currentTest < tests.size() - 1)
        {
            currentTest++;
            viva::engine->Exit();
        }

        return 1;
    });

    viva::routineManager->AddRoutine([fpsText]() 
    {
        double fps = viva::time->GetFps();
        fpsText->SetText(std::to_wstring((int)fps).c_str());

        return 1;
    },0, 0, 0, 0.2f);

    test();

    if (viva::engine->Run() == viva::CloseReason::WindowClose)
        quit = true;

    for (auto r : resources)
        r->Destroy();
    resources.clear();

    return quit;
}

void mandelbrot()
{
    viva::VivaParams params;
    memset(&params, 0, sizeof(params));
    params.size = { 1000,1000 };
    params.title = "Tests";
    viva::Viva v(params);
    viva::engine->SetBackgroundColor({ 255, 255, 255, 255 });
    viva::engine->OpenConsole();
    viva::drawManager->ResizeExtraPSBuffer(32);

    //
    // buffers must have different names
    //
    const char* psStr = R"(
        cbuffer a : register(b0) {float4 color;};
        cbuffer b : register(b1) {double4 var;};
        Texture2D ObjTexture;
        SamplerState ObjSamplerState;
        struct VS_OUTPUT { float4 Pos:SV_POSITION; float3 Col:COLOR; float2 TexCoord:TEXCOORD; };
        
        struct ctx
        {
            float screenSize;
            int maxIterations;
        };
        
        double2 uvToComplex(double x, double y, ctx data)
        {
        	double real = x * var[2] + var[0];
        	double imaginary = y * var[2] + var[1];
        	return double2(real, imaginary);
        }
        
        int mandelbrot(double2 complexNum, int maxIterations)
        {
        	double2 val = complexNum;
        	int it = 0;
        	
        	while(it<maxIterations && val[0]*val[0] + val[1]*val[1] < 4)
        	{
                val = double2(val[0]*val[0]-val[1]*val[1]+complexNum[0],2*val[0]*val[1]+complexNum[1]);
        		it++;
        	}
        	
        	return it;
        }
        
        double4 calcPixel(double x, double y, ctx data)
        {
        	double2 num = uvToComplex(x,y, data);
        	int it = mandelbrot(num, data.maxIterations);
            double maxIt = data.maxIterations;
            double it2 = 1.0f - (it % 256) / 255.0f;
            return float4(it2,it2,it2,1.0f);
        }
        
        float4 main(VS_OUTPUT input):SV_TARGET
        {
            ctx data;
            data.screenSize = 1000;
            data.maxIterations = 2815;
            
        	float4 result = calcPixel(input.TexCoord[0],input.TexCoord[1], data);
        	return result;
        }
        )";

    double data[] = { -1,-1,2,1 };
    viva::PixelShader* ps = viva::creator->CreatePixelShader(psStr);
    viva::drawManager->GetDefaultSurface()->SetPixelShader(ps);
    viva::drawManager->GetDefaultSurface()->SetExtraBufferPSdata(data);

    viva::routineManager->AddRoutine([&]()
    {
        auto cur = viva::mouse->GetCursorDelta();

        if (viva::mouse->IsButtonDown(viva::input::MouseKey::Left))
        {
            data[0] -= cur.x / 500.0 * data[2];
            data[1] += cur.y / 500.0 * data[2];
        }
        else if (viva::mouse->IsButtonDown(viva::input::MouseKey::Middle))
        {
            data[2] *= 1 - (cur.x / 500.0);
            printf("%.32f\n", data[2]);
        }

        return 1;
    });

    viva::engine->Run();
}

int main()
{
    //mandelbrot();

    tests.push_back(testPerformance1);
    tests.push_back(testThruput);
    tests.push_back(testExtraBuffer);
    tests.push_back(testChildParent);
    tests.push_back(testCoords);
    tests.push_back(testPolygons);
    tests.push_back(testSprites);
    tests.push_back(testAnimation);
    tests.push_back(testTransform);
    tests.push_back(testCreateSprites);
    tests.push_back(testSurfaceAndPostShader);
    tests.push_back(testServerClient);

    while (!wrapper(tests[currentTest]));

    return 0;
}