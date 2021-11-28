#include "Memory.h"

using namespace ThorsAnvil::Anvil::Fire;

FrameView::FrameView()
    : b(nullptr)
    , e(nullptr)
{}

FrameView::FrameView(Frame& frame, std::size_t start, std::size_t size)
    : b(&*frame.begin() + start)
    , e(&*frame.begin() + start + size)
{}

MemoryLocation::MemoryLocation()
    : value(Dead{})
{}
