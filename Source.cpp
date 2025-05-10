#define CLAY_IMPLEMENTATION
#include "bindings/cpp/Ceramic.hpp"
//#include "renderers/SFML2/clay_renderer_SFML2.cpp"
#include <stdio.h>
#include "renderers/SFML2/clay_renderer_SFML2.hpp"

const Clay_Color COLOR_LIGHT{224, 215, 210, 255};
const Clay_Color COLOR_RED{ 168, 66, 28, 255 };
const Clay_Color COLOR_ORANGE{ 225, 138, 50, 255 };

const Clay_Color COLOR_DARK_COOL{ 52, 54, 54, 255 };
const Clay_Color COLOR_PURPLE_DARK{ 98, 92, 162, 255 };
const Clay_Color COLOR_PURPLE_MEDIUM{ 140, 127, 193, 255 };
const Clay_Color COLOR_PURPLE_LIGHT{ 198, 168, 226, 255 };
const Clay_Color COLOR_LIGHT_COOL{ 225, 236, 228, 255 };


void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s\n", errorData.errorText.chars);
    switch (errorData.errorType) {
        // etc
    }
}

void handleWindowEvents(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window.close();
            break;
        case sf::Event::Resized:
            window.setView(window.getDefaultView());
            Clay_ResetMeasureTextCache();
            break;
        }
    }
}


// Re-useable components are just normal functions
void SidebarItemComponentCeramic() {
    CeramicElement()
        .backgroundColor(COLOR_PURPLE_DARK)
        .layoutStart()
        .width(CeramicSizeing::Grow)
        .height(50, CeramicSizeing::Fixed)
        .finishLayout()
        .use();
}
Clay_ElementDeclaration sidebarItemConfig = {
    .layout = {
        .sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
    },
    .backgroundColor = COLOR_ORANGE
};

// Re-useable components are just normal functions
void SidebarItemComponent() {
    CLAY(sidebarItemConfig) {
        // children go here...
    }
}

sf::Texture profilePicture;
Clay_RenderCommandArray makeLayout(bool ceramic) {
    Clay_BeginLayout();

    if (ceramic) {
        auto& outer = CeramicElement("OuterContainer")
            .grow()
            .padGap(16, 16)
            .backgroundColor(COLOR_DARK_COOL);
        outer.open();
        {
            auto& sidebar = CeramicElement("SideBar")
                .layoutStart()
                .width(300, CeramicSizeing::Fixed)
                .height(CeramicSizeing::Grow)
                .padGap(16, 16)
                .layoutDirection(CeramicDirection::TopToBottom)
                .finishLayout()
                .borderColour(COLOR_PURPLE_DARK)
                .borderThickness(5)
                .backgroundColor(COLOR_DARK_COOL);
            sidebar.open();
            {
                auto outer = CeramicElement("ProfilePictureOuter")
                    .layoutStart()
                    .width(CeramicSizeing::Grow)
                    .padGap(16, 16)
                    .childAlignment(CeramicAlignX::Center, CeramicAlignY::Center)
                    .finishLayout()
                    .backgroundColor(COLOR_PURPLE_MEDIUM);
                outer.open();
                {
                    CeramicElement("ProfilePicture")
                        .sizeFixed(60, 60)
                        .image(&profilePicture, 60, 60)
                        .use();
                    CeramicText()
                        .color(COLOR_LIGHT_COOL)
                        .fontSize(25)
                        .use("Clay - UI Library (In Ceramic!)");
                }
                outer.close();

                for (int i = 0; i < 5; i++)
                    SidebarItemComponentCeramic();
            }
            sidebar.close();
            CeramicElement("MainContent")
                .grow()
                .borderColour(COLOR_PURPLE_DARK)
                .borderThickness(5)
                .backgroundColor(COLOR_DARK_COOL)
                .use();
        }
        outer.close();
    }
    else {
        CLAY({ .id = CLAY_ID("OuterContainer"), .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = {250,250,255,255} }) {
            CLAY({
                 .id = CLAY_ID("SideBar"),
                 .layout = {.sizing = {.width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .layoutDirection = CLAY_TOP_TO_BOTTOM },
                 .backgroundColor = COLOR_LIGHT
                }) {
                CLAY({ .id = CLAY_ID("ProfilePictureOuter"), .layout = {.sizing = {.width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .childAlignment = {.y = CLAY_ALIGN_Y_CENTER } }, .backgroundColor = COLOR_RED }) {
                    CLAY({ .id = CLAY_ID("ProfilePicture"), .layout = {.sizing = {.width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) }}, .image = {.imageData = &profilePicture, .sourceDimensions = {60, 60} } }) {}
                    CLAY_TEXT(CLAY_STRING("Clay - UI Library (Now with SFML!)"), CLAY_TEXT_CONFIG({ .textColor = {255, 255, 255, 255}, .fontSize = 25 }));
                }

                // Standard C code like loops etc work inside components
                for (int i = 0; i < 5; i++) {
                    SidebarItemComponent();
                }
            }
            CLAY({ .id = CLAY_ID("MainContent"), .layout = {.sizing = {.width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } }, .backgroundColor = COLOR_LIGHT }) {}
        }
    }

    return Clay_EndLayout();
}

int main() {
    profilePicture.loadFromFile("D:/SFML-2.6.2/examples/assets/logo.png");

    sf::RenderTexture* tex = new sf::RenderTexture;
    tex->create(20, 20);
    tex->clear();
    tex->display();

    const float startWidth = 1000;
    const float startHeight = 800;
    sf::RenderWindow window(sf::VideoMode(startWidth, startHeight), "Clay");
    SFML_Renderer renderer;
    renderer.target = &window;
    sf::Font& font = renderer.fonts.emplace_back();
    font.loadFromFile("times new roman.ttf");
    font.setSmooth(false);

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena, { startWidth, startHeight }, { HandleClayErrors });
    Clay_SetMeasureTextFunction(SFML_MeasureText, (void*)&renderer);
    bool useCeramic = false;

    while (window.isOpen()) {
        handleWindowEvents(window);
        Clay_SetLayoutDimensions({ float(window.getView().getSize().x), float(window.getView().getSize().y) });

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            Clay_SetDebugModeEnabled(true);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            Clay_SetDebugModeEnabled(false);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            useCeramic = true;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
            useCeramic = false;

        auto commands = makeLayout(useCeramic);

        window.clear();
        Clay_SFML_Render(&renderer, commands);
        window.display();
    }

    // while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    //     sf::sleep(sf::milliseconds(500));
    // }

    return 0;
}