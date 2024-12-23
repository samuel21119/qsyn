/****************************************************************************
  PackageName  [ zx ]
  Synopsis     [ Define basic data or var for graph package ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#pragma once

#include <fmt/core.h>
#include <fmt/format.h>

#include <functional>
#include <iosfwd>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "qsyn/qsyn_type.hpp"
#include "util/ordered_hashmap.hpp"
#include "util/ordered_hashset.hpp"
#include "util/phase.hpp"
#include "util/text_format.hpp"

namespace qsyn::zx {

class ZXVertex;
class ZXGraph;

enum class VertexType : std::uint8_t {
    boundary,
    z,
    x,
    h_box
};

enum class EdgeType : std::uint8_t {
    simple,
    hadamard
};

//------------------------------------------------------------------------
//  Define types
//------------------------------------------------------------------------

using ZXVertexList = dvlab::utils::ordered_hashset<ZXVertex*>;
using EdgePair     = std::pair<std::pair<ZXVertex*, ZXVertex*>, EdgeType>;
using NeighborPair = std::pair<ZXVertex*, EdgeType>;

// two boundary vertices from different ZXGraph and the edge type between them
using ZXCut = std::tuple<ZXVertex*, ZXVertex*, EdgeType>;

struct NeighborPairHash {
    size_t operator()(NeighborPair const& k) const {
        return (
            (std::hash<ZXVertex*>()(k.first) ^
             (std::hash<EdgeType>()(k.second) << 1)) >>
            1);
    }
};
struct EdgePairHash {
    size_t operator()(EdgePair const& k) const {
        return (
                   (std::hash<ZXVertex*>()(k.first.first) ^
                    (std::hash<ZXVertex*>()(k.first.second) << 1)) >>
                   1) ^
               (std::hash<EdgeType>()(k.second) << 1);
    }
};
using Neighbors = dvlab::utils::ordered_hashset<NeighborPair, NeighborPairHash>;

struct ZXCutHash {
    size_t operator()(ZXCut const& cut) const {
        auto [v1, v2, edgeType] = cut;
        // the order of v1 and v2 does not matter
        if (v1 > v2) std::swap(v1, v2);
        size_t result = std::hash<ZXVertex*>()(v1) ^ std::hash<ZXVertex*>()(v2);
        result ^= std::hash<EdgeType>()(edgeType) << 1;
        return result;
    }
};

struct ZXCutEqual {
    bool operator()(ZXCut const& lhs, ZXCut const& rhs) const {
        auto [v1, v2, edgeType]  = lhs;
        auto [v3, v4, edgeType2] = rhs;
        // the order of v1 and v2 does not matter
        if (v1 > v2) std::swap(v1, v2);
        if (v3 > v4) std::swap(v3, v4);
        return v1 == v3 && v2 == v4 && edgeType == edgeType2;
    }
};

using ZXCutSet = dvlab::utils::ordered_hashset<ZXCut, ZXCutHash, ZXCutEqual>;

//------------------------------------------------------------------------
//   Define hashes
//------------------------------------------------------------------------

}  // namespace qsyn::zx

std::ostream& operator<<(std::ostream& stream, qsyn::zx::VertexType const& vt);
std::ostream& operator<<(std::ostream& stream, qsyn::zx::EdgeType const& et);

template <>
struct fmt::formatter<qsyn::zx::VertexType> {
    using VertexType = qsyn::zx::VertexType;
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(qsyn::zx::VertexType const& vt, format_context& ctx) const -> format_context::iterator {
        using namespace dvlab;
        switch (vt) {
            case VertexType::x:
                return fmt::format_to(ctx.out(), "{}", fmt_ext::styled_if_ansi_supported("X", fmt::fg(fmt::terminal_color::red) | fmt::emphasis::bold));
            case VertexType::z:
                return fmt::format_to(ctx.out(), "{}", fmt_ext::styled_if_ansi_supported("Z", fmt::fg(fmt::terminal_color::green) | fmt::emphasis::bold));
            case VertexType::h_box:
                return fmt::format_to(ctx.out(), "{}", fmt_ext::styled_if_ansi_supported("H", fmt::fg(fmt::terminal_color::yellow) | fmt::emphasis::bold));
            case VertexType::boundary:
            default:
                return fmt::format_to(ctx.out(), "●");
        }
    }
};

template <>
struct fmt::formatter<qsyn::zx::EdgeType> {
    using EdgeType = qsyn::zx::EdgeType;
    constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator {
        return ctx.begin();
    }
    auto format(EdgeType const& et, format_context& ctx) const -> format_context::iterator {
        using namespace dvlab;
        switch (et) {
            case EdgeType::hadamard:
                return fmt::format_to(ctx.out(), "{}", fmt_ext::styled_if_ansi_supported("H", fmt::fg(fmt::terminal_color::blue) | fmt::emphasis::bold));
            case EdgeType::simple:
            default:
                return fmt::format_to(ctx.out(), "-");
        }
    }
};
