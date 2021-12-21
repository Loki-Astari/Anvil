#ifndef THORSANVIL_ANVIL_ICE_UTILITY_VIEW_H
#define THORSANVIL_ANVIL_ICE_UTILITY_VIEW_H

namespace ThorsAnvil::Anvil::Ice
{

    // View
    enum ViewType {Forward, Reverse};

    template<ViewType V, typename I>
    struct View
    {
        I beginRange;
        I endRange;
        using ForwardI = I;
        using ReverseI = std::reverse_iterator<I>;
        auto begin() {if constexpr (V == Forward) {return beginRange;} else {return ReverseI(endRange);}}
        auto end()   {if constexpr (V == Forward) {return endRange;} else {return ReverseI(beginRange);}}
    };

    // Helper function to make view without need to know type.
    template<ViewType V, typename I>
    View<V, I> make_View(I begin, I end) {return View<V, I>{begin, end};}

    // Helper function to make view without need to know type.
    template<ViewType V, typename C>
    std::enable_if_t<std::is_const_v<C>, View<V, typename C::const_iterator>>
    make_View(C& c) {return View<V, typename C::const_iterator>{std::cbegin(c), std::cend(c)};}

    template<ViewType V, typename C>
    std::enable_if_t<!std::is_const_v<C>, View<V, typename C::iterator>>
    make_View(C& c) {return View<V, typename C::iterator>{std::begin(c), std::end(c)};}

    template<typename T>
    struct ReversView
    {
        T& range;
        ReversView(T& range)
            : range(range)
        {}

        auto begin() {return range.rbegin();}
        auto end()   {return range.rend();}
    };

}

#endif
