#include "Ceramic.hpp"
#include <vector>
#define CLAY_IMPLEMENTATION
#include "../../clay.h"

using CeramicLayout = CeramicElement::CeramicLayout;

// Returns true if the currently open element is hovered over
bool CeramicHovered() {
	return Clay_Hovered();
}

CeramicMouseState CeramicGetMouseState() {
	CeramicMouseState mouseState;

	switch (Clay_GetCurrentContext()->pointerInfo.state) {
	case CLAY_POINTER_DATA_PRESSED_THIS_FRAME: mouseState.down = mouseState.newlyDown = true; break;
	case CLAY_POINTER_DATA_PRESSED: mouseState.down = true; break;
	case CLAY_POINTER_DATA_RELEASED: mouseState.up = true; break;
	case CLAY_POINTER_DATA_RELEASED_THIS_FRAME: mouseState.up = true; mouseState.newlyReleased = true; break;
	}

	return mouseState;
}

std::vector<std::string> cachedStrings;

// Erases the cached strings from  last frame
void CeramicClearStrings() {
	cachedStrings.clear();
	Clay_ResetMeasureTextCache();
}
// Saves a string for next frame
const std::string& CeramicPushString(const char* string) {
	return cachedStrings.emplace_back(string);
}
// Saves a string for next frame
const std::string& CeramicPushString(const std::string& string) {
	return cachedStrings.emplace_back(string);
}

CeramicLayout& CeramicLayout::sizing(const Clay_Sizing& sizeingStruct) {
	config->sizing = sizeingStruct;
	return *this;
}
static void ceramicAxis(CeramicSizeing size, Clay_SizingAxis& axis) {
	axis.size.minMax = { .min = 0, .max = 0 };
	switch (size) {
	case CeramicSizeing::Fit:
		axis.type = CLAY__SIZING_TYPE_FIT;
		break;
	case CeramicSizeing::Grow:
		axis.type = CLAY__SIZING_TYPE_GROW;
		break;
	default:
		printf("Invalid sizing passed with width\n");
		break;
	}
}
static void ceramicSize(float value, CeramicSizeing sizing, Clay_SizingAxis& axis) {
	if (sizing == CeramicSizeing::Percent) {
		axis.type = CLAY__SIZING_TYPE_PERCENT;
		axis.size.percent = value;
	}
	else if (sizing == CeramicSizeing::Fixed) {
		axis.type = CLAY__SIZING_TYPE_FIXED;
		axis.size.minMax = { .min = value, .max = value };
	}
	else throw(-1);
}
CeramicLayout& CeramicLayout::width(float value, CeramicSizeing sizing) {
	ceramicSize(value, sizing, config->sizing.width);
	return *this;
}
CeramicLayout& CeramicLayout::width(float min, float max) {
	config->sizing.width.type = CLAY__SIZING_TYPE_FIXED;
	config->sizing.width.size.minMax = { .min = min, .max = max };
	return *this;
}
CeramicLayout& CeramicLayout::width(CeramicSizeing size) {
	auto& axis = config->sizing.width;
	ceramicAxis(size, axis);
	return *this;
}
CeramicLayout& CeramicLayout::height(float value, CeramicSizeing sizing) {
	ceramicSize(value, sizing, config->sizing.height);
	return *this;
}
CeramicLayout& CeramicLayout::height(float min, float max) {
	config->sizing.height.type = CLAY__SIZING_TYPE_FIXED;
	config->sizing.height.size.minMax = { .min = min, .max = max };
	return *this;
}
CeramicLayout& CeramicLayout::height(CeramicSizeing size) {
	auto& axis = config->sizing.height;
	ceramicAxis(size, axis);
	return *this;
}

CeramicLayout& CeramicLayout::grow() {
	width(CeramicSizeing::Grow);
	height(CeramicSizeing::Grow);
	return *this;
}
CeramicLayout& CeramicLayout::sizeFixed(float _width, float _height) {
	width(_width, CeramicSizeing::Fixed);
	height(_height, CeramicSizeing::Fixed);
	return *this;
}
CeramicLayout& CeramicLayout::sizePercent(float _width, float _height) {
	width(_width, CeramicSizeing::Percent);
	height(_height, CeramicSizeing::Percent);
	return *this;
}

CeramicLayout& CeramicLayout::padding(const Clay_Padding& paddingStruct) {
	config->padding = paddingStruct;
	return *this;
}
CeramicLayout& CeramicLayout::padding(uint16_t all) {
	padding(all, all, all, all);
	return *this;
}
CeramicLayout& CeramicLayout::padding(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom){
	config->padding = { .left = left, .right = right, .top = top, .bottom = bottom };
	return *this;
}
CeramicLayout& CeramicLayout::childGap(uint16_t gap) {
	config->childGap = gap;
	return *this;
}
CeramicLayout& CeramicLayout::padGap(uint16_t _padding, uint16_t _childGap) {
	padding(_padding);
	childGap(_childGap);
	return *this;
}
CeramicLayout& CeramicLayout::childAlignment(const Clay_ChildAlignment& childAlignmentStruct) {
	config->childAlignment = childAlignmentStruct;
	return *this;
}
CeramicLayout& CeramicLayout::childAlignment(CeramicAlignX xAlign, CeramicAlignY yAlign) {
	static Clay_LayoutAlignmentX xAligns[] = { CLAY_ALIGN_X_LEFT, CLAY_ALIGN_X_RIGHT, CLAY_ALIGN_X_CENTER };
	static Clay_LayoutAlignmentY yAligns[] = { CLAY_ALIGN_Y_TOP, CLAY_ALIGN_Y_BOTTOM, CLAY_ALIGN_Y_CENTER };
	config->childAlignment.x = xAligns[int(xAlign)];
	config->childAlignment.y = yAligns[int(yAlign)];
	return *this;
}
CeramicLayout& CeramicLayout::layoutDirection(Clay_LayoutDirection direction) {
	config->layoutDirection = direction;
	return *this;
}
CeramicLayout& CeramicLayout::layoutDirection(CeramicDirection direction) {
	config->layoutDirection = direction == CeramicDirection::LeftToRight ? CLAY_LEFT_TO_RIGHT : CLAY_TOP_TO_BOTTOM;
	return *this;
}
CeramicElement& CeramicLayout::finishLayout() {
	return *element;
}

CeramicLayout::CeramicLayout(CeramicElement* element) :
	element(element),
	config(&element->element.layout)
{}




Clay_ElementDeclaration& CeramicElement::getRaw() {
	return element;
}
const Clay_ElementDeclaration& CeramicElement::getRaw()const {
	return element; // But const this time hehehe
}
CeramicElement CeramicElement::copy(const std::string& _name)const {
	CeramicElement e;
	e.element = element;
	e.name = _name;
	Clay_String s;
	s.chars = e.name.c_str();
	s.length = e.name.size();
	e.element.id = CLAY_SID(s);
	e.isOpen = false;
	return e;
}
CeramicElement CeramicElement::copy(const std::string& _name, int id)const {
	CeramicElement e;
	e.element = element;
	e.name = _name;
	Clay_String s;
	s.chars = e.name.c_str();
	s.length = e.name.size();
	e.element.id = CLAY_SIDI(s, id);
	e.isOpen = false;
	return e;
}

void CeramicElement::open() {
	if (isOpen)throw(-1);
	Clay__OpenElement();
	Clay__ConfigureOpenElement((Clay__Clay_ElementDeclarationWrapper{ element }).wrapped);
	isOpen = true;
}
void CeramicElement::close() {
	if (!isOpen) throw(-1);
	Clay__CloseElement();
	isOpen = false;
}
void CeramicElement::use() {
	open();
	close();
}

CeramicElement& CeramicElement::getScrollData(CeramicScrollData& data) {
	data = Clay_GetScrollContainerData(element.id);
	return *this;
}
// This'll only work if you've set the element ID already
CeramicElement& CeramicElement::getHovered(bool& isHovered) {
	isHovered = Clay_PointerOver(element.id);
	return *this;
}

CeramicElement& CeramicElement::padGap(uint16_t _padding, uint16_t _childGap) {
	CeramicLayout(this).padGap(_padding, _childGap);
	return *this;
}
CeramicElement& CeramicElement::grow() {
	CeramicLayout(this).grow();
	return *this;
}
CeramicElement& CeramicElement::sizeFixed(float _width, float _height) {
	CeramicLayout(this).sizeFixed(_width, _height);
	return *this;
}
CeramicElement& CeramicElement::sizePercent(float _width, float _height) {
	CeramicLayout(this).sizePercent(_width, _height);
	return *this;
}

CeramicElement::CeramicElement():
	element({}),
	floating(element, this)
{}
CeramicElement::CeramicElement(const char* _name) :
	name(_name),
	element({}),
	floating(element, this)
{
	Clay_String s;
	s.chars = _name;
	s.length = strlen(_name);

	element.id = CLAY_SID(s);
}
CeramicElement::CeramicElement(const std::string& name) :
	name(name),
	element({}),
	floating(element, this)
{
	Clay_String s;
	s.chars = this->name.c_str();
	s.length = this->name.size();
	element.id = CLAY_SID(s);
}
CeramicElement::CeramicElement(const std::string& name, int id) :
	name(name),
	element({}),
	floating(element, this)
{
	Clay_String s;
	s.chars = this->name.c_str();
	s.length = this->name.size();
	element.id = CLAY_SIDI(s, id);
}
CeramicElement::~CeramicElement() {
	if (isOpen)throw(-1);
}

CeramicLayout CeramicElement::layoutStart() {
	return CeramicLayout(this);
}
CeramicElement& CeramicElement::backgroundColor(const Clay_Color& color) {
	element.backgroundColor = color;
	return *this;
}
CeramicElement& CeramicElement::backgroundColor(float r, float g, float b, float a) {
	element.backgroundColor = { .r = r, .g = g, .b = b, .a = a };
	return *this;
}
CeramicElement& CeramicElement::cornerRadius(const Clay_CornerRadius& radius) {
	element.cornerRadius = radius;
	return *this;
}
CeramicElement& CeramicElement::cornerRadius(float topLeft, float topRight, float bottomLeft, float bottomRight) {
	element.cornerRadius = { .topLeft = topLeft, .topRight = topRight, .bottomLeft = bottomLeft, .bottomRight = bottomRight };
	return *this;
}
CeramicElement& CeramicElement::image(const Clay_ImageElementConfig& config) {
	element.image = config;
	return *this;
}
CeramicElement& CeramicElement::image(void* imageData, float width, float height) {
	element.image = { .imageData = imageData, .sourceDimensions = {.width = width, .height = height} };
	return *this;
}


CeramicElement& CeramicElement::_Floating::raw(const Clay_FloatingElementConfig& config) {
	element.floating = config;
	return *e;
}
CeramicElement& CeramicElement::_Floating::mousePassthrough() {
	element.floating.pointerCaptureMode = Clay_PointerCaptureMode::CLAY_POINTER_CAPTURE_MODE_PASSTHROUGH;
	return *e;
}
CeramicElement& CeramicElement::_Floating::mouseCapture() {
	element.floating.pointerCaptureMode = Clay_PointerCaptureMode::CLAY_POINTER_CAPTURE_MODE_CAPTURE;
	return *e;
}
CeramicElement& CeramicElement::_Floating::setAttachMode(float x, float y, Clay_FloatingAttachPointType fromChild, Clay_FloatingAttachPointType toParent) {
	element.floating.offset = { .x = x, .y = y };
	element.floating.attachPoints = { .element = fromChild, .parent = toParent };
	return *e;
}

CeramicElement& CeramicElement::_Floating::disableFloating() {
	element.floating.attachTo = Clay_FloatingAttachToElement::CLAY_ATTACH_TO_NONE;
	return *e;
}
CeramicElement& CeramicElement::_Floating::attachToParent() {
	element.floating.attachTo = Clay_FloatingAttachToElement::CLAY_ATTACH_TO_PARENT;
	return *e;
}
CeramicElement& CeramicElement::_Floating::attachToRoot() {
	element.floating.attachTo = Clay_FloatingAttachToElement::CLAY_ATTACH_TO_ROOT;
	return *e;
}
CeramicElement& CeramicElement::_Floating::attachToID(uint32_t parentID) {
	element.floating.attachTo = Clay_FloatingAttachToElement::CLAY_ATTACH_TO_PARENT;
	element.floating.parentId = parentID;
	return *e;
}

CeramicElement& CeramicElement::_Floating::expand(float x, float y) {
	element.floating.expand = { .width = x, .height = y };
	return *e;
}
CeramicElement& CeramicElement::_Floating::zIndex(int16_t z) {
	element.floating.zIndex = z;
	return *e;
}
CeramicElement::_Floating::_Floating(Clay_ElementDeclaration& element, CeramicElement* e):
	element(element),
	e(e)
{ }


CeramicElement& CeramicElement::custom(const Clay_CustomElementConfig& config) {
	element.custom = config;
	return *this;
}
CeramicElement& CeramicElement::custom(void* custom) {
	element.custom.customData = custom;
	return *this;
}


CeramicElement& CeramicElement::scroll(const Clay_ScrollElementConfig& config) {
	element.scroll = config;
	return *this;
}
CeramicElement& CeramicElement::scroll(bool scrollsHorizontal, bool scrollsVertical) {
	element.scroll = { .horizontal = scrollsHorizontal, .vertical = scrollsVertical };
	return *this;
}

CeramicElement& CeramicElement::border(const Clay_BorderElementConfig& config) {
	element.border = config;
	return *this;
}
CeramicElement& CeramicElement::borderColour(Clay_Color color) {
	element.border.color = color;
	return *this;
}
CeramicElement& CeramicElement::borderColour(float r, float g, float b, float a) {
	element.border.color = { .r = r, .g = g, .b = b, .a = a };
	return *this;
}
CeramicElement& CeramicElement::borderThickness(float all) {
	element.border.width.left = element.border.width.right =
		element.border.width.top = element.border.width.bottom =
		all;
	return *this;
}
CeramicElement& CeramicElement::borderThickness(float left, float right, float top, float bottom) {
	element.border.width.left = left;
	element.border.width.right = right;
	element.border.width.top = top;
	element.border.width.bottom = bottom;
	return *this;
}
CeramicElement& CeramicElement::borderChildren(float size) {
	element.border.width.betweenChildren = size;
	return *this;
}


CeramicText& CeramicText::color(Clay_Color color) {
	config.textColor = color;
	return *this;
}
CeramicText& CeramicText::color(float r, float g, float b, float a) {
	config.textColor = { .r = r, .g = g, .b = b, .a = a };
	return *this;
}
CeramicText& CeramicText::font(uint16_t id) {
	config.fontId = id;
	return *this;
}
CeramicText& CeramicText::fontSize(uint16_t size) {
	config.fontSize = size;
	return *this;
}
CeramicText& CeramicText::spacing(uint16_t letterSpacing, uint16_t lineHeight) {
	config.letterSpacing = letterSpacing;
	config.lineHeight = lineHeight;
	return *this;
}
CeramicText& CeramicText::align(CeramicAlignX alignment) {
	config.textAlignment = alignment == CeramicAlignX::Left ? Clay_TextAlignment::CLAY_TEXT_ALIGN_LEFT :
		(
			alignment == CeramicAlignX::Center ? 
			Clay_TextAlignment::CLAY_TEXT_ALIGN_CENTER : Clay_TextAlignment::CLAY_TEXT_ALIGN_RIGHT

		);
	return *this;
}
CeramicText& CeramicText::setWrapMode(Clay_TextElementConfigWrapMode wrapMode) {
	config.wrapMode = wrapMode;
	return *this;
}
CeramicText& CeramicText::hash(bool shouldHash) {
	config.hashStringContents = shouldHash;
	return *this;
}

void CeramicText::use(const char* text) {
	Clay_String s;
	s.chars = text;
	s.length = strlen(text);
	CLAY_TEXT(s, &config);
}
void CeramicText::use(const std::string& text) {
	Clay_String s;
	s.chars = text.c_str();
	s.length = text.size();
	CLAY_TEXT(s, &config);
}

CeramicText::CeramicText():
	config({})
{ 
	config.fontId = 0;
}
