#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "renderers/SFML2/clay_renderer_SFML2.cpp"

const Clay_Color COLOR_LIGHT = (Clay_Color) {224, 215, 210, 255};
const Clay_Color COLOR_RED = (Clay_Color) {168, 66, 28, 255};
const Clay_Color COLOR_ORANGE = (Clay_Color) {225, 138, 50, 255};

void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    printf("%s\n", errorData.errorText.chars);
    switch(errorData.errorType) {
        // etc
    }
}

void handleWindowEvents(sf::RenderWindow& window){
    sf::Event event;
    while(window.pollEvent(event)){
        switch(event.type){
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

Clay_ElementDeclaration sidebarItemConfig = (Clay_ElementDeclaration) {
    .layout = {
        .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(50) }
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
Clay_RenderCommandArray makeLayout(){
    Clay_BeginLayout();

    CLAY({ .id = CLAY_ID("OuterContainer"), .layout = { .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}, .padding = CLAY_PADDING_ALL(16), .childGap = 16 }, .backgroundColor = {250,250,255,255} }) {
        CLAY({
            .id = CLAY_ID("SideBar"),
            .layout = { .layoutDirection = CLAY_TOP_TO_BOTTOM, .sizing = { .width = CLAY_SIZING_FIXED(300), .height = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16 },
            .backgroundColor = COLOR_LIGHT
        }) {
            CLAY({ .id = CLAY_ID("ProfilePictureOuter"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0) }, .padding = CLAY_PADDING_ALL(16), .childGap = 16, .childAlignment = { .y = CLAY_ALIGN_Y_CENTER } }, .backgroundColor = COLOR_RED }) {
                CLAY({ .id = CLAY_ID("ProfilePicture"), .layout = { .sizing = { .width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60) }}, .image = { .imageData = &profilePicture, .sourceDimensions = {60, 60} } }) {}
                CLAY_TEXT(CLAY_STRING("Clay - UI Library (Now with SFML!)"), CLAY_TEXT_CONFIG({ .fontSize = 12, .textColor = {255, 255, 255, 255} }));
            }

            // Standard C code like loops etc work inside components
            for (int i = 0; i < 5; i++) {
                SidebarItemComponent();
            }
        }
        CLAY({ .id = CLAY_ID("MainContent"), .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } }, .backgroundColor = COLOR_LIGHT }) {}
    }

    return Clay_EndLayout();
}

int main(){
    if(!fake){
        fake = true;
        main();
        fake = false;
    }
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

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena, (Clay_Dimensions) { startWidth, startHeight }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(SFML_MeasureText, (void*)&renderer);

    while(window.isOpen()){
        handleWindowEvents(window);
        Clay_SetLayoutDimensions((Clay_Dimensions) { float(window.getView().getSize().x), float(window.getView().getSize().y) });

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K))
            Clay_SetDebugModeEnabled(true);
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::L))
            Clay_SetDebugModeEnabled(false);

        auto commands = makeLayout();

        window.clear();
        Clay_SFML_Render(&renderer, commands);
        window.display();
        
        if(fake)return 0;
    }

    // while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
    //     sf::sleep(sf::milliseconds(500));
    // }

    return 0;
}