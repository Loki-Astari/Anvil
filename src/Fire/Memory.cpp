#include "Memory.h"

using namespace ThorsAnvil::Anvil::Fire;

Frame::Frame(std::size_t size)
    : frame(size)
{}

FrameView::FrameView()
    : b(nullptr)
    , e(nullptr)
{}

FrameView::FrameView(Frame& frame, std::size_t start, std::size_t size)
    : b(&*frame.frame.begin() + start)
    , e(&*frame.frame.begin() + start + size)
{}

MemoryLocation::MemoryLocation()
    : value(Dead{})
{}
