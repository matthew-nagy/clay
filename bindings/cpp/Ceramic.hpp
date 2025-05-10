#include "../../clay.h"
#include <string>

enum class CeramicDirection{
    LeftToRight,
    TopToBottom
};
enum class CeramicAlignX{
    Left = 0, Right, Center
};
enum class CeramicAlignY{
    Top = 0, Bottom, Center
};
enum class CeramicSizeing {
    // (default) Wraps tightly to the size of the element's contents.
    Fit,
    // Expands along this axis to fill available space in the parent element, sharing it with other GROW elements.
    Grow,
    // Expects 0-1 range. Clamps the axis size to a percent of the parent container's axis size minus padding and child gaps.
    Percent,
    // Clamps the axis size to an exact size in pixels.
    Fixed,
};
enum class CeramicLimit {
    Min, Max
};

// Returns true if the currently open element is hovered over
bool CeramicHovered();

struct CeramicMouseState {
    bool down = false, newlyDown = false;
    bool up = false, newlyReleased = false;
};
CeramicMouseState CeramicGetMouseState();

// Erases the cached strings from  last frame
void CeramicClearStrings();
// Saves a string for next frame
const std::string& CeramicPushString(const char* string);
// Saves a string for next frame
const std::string& CeramicPushString(const std::string& string);

using CeramicScrollData = Clay_ScrollContainerData;

struct CeramicElement final{
    struct CeramicLayout{

        CeramicLayout& sizing(const Clay_Sizing& sizeingStruct);
        CeramicLayout& width(float value, CeramicSizeing sizing = CeramicSizeing::Percent);
        CeramicLayout& width(float min, float max);
        CeramicLayout& width(CeramicSizeing size);
        CeramicLayout& height(float values, CeramicSizeing sizing = CeramicSizeing::Percent);
        CeramicLayout& height(float min, float max);
        CeramicLayout& height(CeramicSizeing size);

        CeramicLayout& grow();
        CeramicLayout& sizeFixed(float _width, float _height);
        CeramicLayout& sizePercent(float _width, float _height);

        CeramicLayout& padding(const Clay_Padding& paddingStruct);
        CeramicLayout& padding(uint16_t all);
        CeramicLayout& padding(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom);
        CeramicLayout& childGap(uint16_t gap);
        //Padding and child gap in one
        CeramicLayout& padGap(uint16_t _padding, uint16_t _childGap);
        CeramicLayout& childAlignment(const Clay_ChildAlignment& childAlignmentStruct);
        CeramicLayout& childAlignment(CeramicAlignX xAlign, CeramicAlignY yAlign);
        CeramicLayout& layoutDirection(Clay_LayoutDirection direction);
        CeramicLayout& layoutDirection(CeramicDirection direction);
        CeramicElement& finishLayout();

        CeramicLayout(CeramicElement* element);
    private:
        CeramicElement* element;
        Clay_LayoutConfig* config;
    };

    Clay_ElementDeclaration& getRaw();
    const Clay_ElementDeclaration& getRaw()const;
    CeramicElement copy(const std::string& name)const;
    CeramicElement copy(const std::string& name, int id)const;

    CeramicElement();
    CeramicElement(const char* name);
    CeramicElement(const std::string& name);
    CeramicElement(const std::string& name, int id);
    virtual ~CeramicElement();

    virtual void open();
    virtual void close();
    void use();

    // This'll only work if you've set the element ID already
    CeramicElement& getScrollData(CeramicScrollData& data);
    // This'll only work if you've set the element ID already
    CeramicElement& getHovered(bool& isHovered);

    CeramicLayout layoutStart();
    CeramicElement& padGap(uint16_t _padding, uint16_t _childGap);
    CeramicElement& grow();
    CeramicElement& sizeFixed(float _width, float _height);
    CeramicElement& sizePercent(float _width, float _height);

    CeramicElement& backgroundColor(const Clay_Color& color);
    CeramicElement& backgroundColor(float r, float g, float b, float a = 255.0f);
    CeramicElement& cornerRadius(const Clay_CornerRadius& radius);
    CeramicElement& cornerRadius(float topLeft, float topRight, float bottomLeft, float bottomRight);
    CeramicElement& image(const Clay_ImageElementConfig& config);
    CeramicElement& image(void* imageData, float width, float height);
    struct _Floating {
        CeramicElement& raw(const Clay_FloatingElementConfig& config);
        CeramicElement& mousePassthrough();
        CeramicElement& mouseCapture();
        CeramicElement& setAttachMode(float x, float y, Clay_FloatingAttachPointType fromChild, Clay_FloatingAttachPointType toParent);
        
        CeramicElement& disableFloating();
        CeramicElement& attachToParent();
        CeramicElement& attachToRoot();
        CeramicElement& attachToID(uint32_t parentID);

        CeramicElement& expand(float x, float y);
        CeramicElement& zIndex(int16_t z);

        _Floating(Clay_ElementDeclaration& element, CeramicElement* e);
    private:
        Clay_ElementDeclaration& element;
        CeramicElement* e;
    }floating;
    CeramicElement& custom(const Clay_CustomElementConfig& config);
    CeramicElement& custom(void* custom);
    CeramicElement& scroll(const Clay_ScrollElementConfig& config);
    CeramicElement& scroll(bool scrollsHorizontal, bool scrollsVertical);
    CeramicElement& border(const Clay_BorderElementConfig& config);
    CeramicElement& borderColour(Clay_Color color);
    CeramicElement& borderColour(float r, float g, float b, float a = 255.0f);
    CeramicElement& borderThickness(float all);
    CeramicElement& borderThickness(float left, float right, float top, float bottom);
    CeramicElement& borderChildren(float size);
private:

    Clay_ElementDeclaration element;
    std::string name;
    bool isOpen = false;
};

class CeramicText {
public:

    CeramicText& color(Clay_Color color);
    CeramicText& color(float r, float g, float b, float a = 255.0f);
    CeramicText& font(uint16_t id);
    CeramicText& fontSize(uint16_t size);
    CeramicText& spacing(uint16_t letterSpacing, uint16_t lineHeight);
    CeramicText& align(CeramicAlignX alignment);
    CeramicText& setWrapMode(Clay_TextElementConfigWrapMode wrapMode);
    CeramicText& hash(bool shouldHash = true);
    CeramicText();

    void use(const char* text);
    void use(const std::string& text);

    virtual ~CeramicText() = default;

private:
    Clay_TextElementConfig config;
};