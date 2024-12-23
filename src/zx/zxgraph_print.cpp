/****************************************************************************
  PackageName  [ zx ]
  Synopsis     [ Define class ZXGraph Print functions ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <spdlog/spdlog.h>

#include <cstddef>
#include <gsl/narrow>
#include <map>
#include <ranges>
#include <string>

#include "fmt/core.h"
#include "spdlog/common.h"
#include "util/text_format.hpp"
#include "zx/zxgraph.hpp"

namespace qsyn {

namespace zx {

/**
 * @brief Print information of ZXGraph
 *
 */
void ZXGraph::print_graph(spdlog::level::level_enum lvl) const {
    if (!spdlog::should_log(lvl)) return;
    spdlog::log(lvl, "Graph ({} inputs, {} outputs, {} vertices, {} edges)", get_num_inputs(), get_num_outputs(), get_num_vertices(), get_num_edges());
}

/**
 * @brief Print Inputs of ZXGraph
 *
 */
void ZXGraph::print_inputs() const {
    fmt::println("Input:  ({})", fmt::join(_inputs | std::views::transform([](ZXVertex* v) { return v->get_id(); }), ", "));
    fmt::println("Total #Inputs: {}", get_num_inputs());
}

/**
 * @brief Print Outputs of ZXGraph
 *
 */
void ZXGraph::print_outputs() const {
    fmt::println("Output: ({})", fmt::join(_outputs | std::views::transform([](ZXVertex* v) { return v->get_id(); }), ", "));
    fmt::println("Total #Outputs: {}", get_num_outputs());
}

/**
 * @brief Print Inputs and Outputs of ZXGraph
 *
 */
void ZXGraph::print_io() const {
    fmt::println("Input:  ({})", fmt::join(_inputs | std::views::transform([](ZXVertex* v) { return v->get_id(); }), ", "));
    fmt::println("Output: ({})", fmt::join(_outputs | std::views::transform([](ZXVertex* v) { return v->get_id(); }), ", "));
    fmt::println("Total #(I,O): ({}, {})", get_num_inputs(), get_num_outputs());
}

/**
 * @brief Print Vertices of ZXGraph
 *
 */
void ZXGraph::print_vertices(spdlog::level::level_enum lvl) const {
    if (!spdlog::should_log(lvl)) return;
    spdlog::log(lvl, "");
    std::ranges::for_each(_vertices, [&lvl](ZXVertex* v) { v->print_vertex(lvl); });
    spdlog::log(lvl, "Total #Vertices: {}", get_num_vertices());
    spdlog::log(lvl, "");
}

/**
 * @brief Print Vertices of ZXGraph in `cand`.
 *
 * @param cand
 */
void ZXGraph::print_vertices(std::vector<size_t> cand) const {
    std::unordered_map<size_t, ZXVertex*> id2_vmap = create_id_to_vertex_map();

    fmt::println("");
    for (size_t i = 0; i < cand.size(); i++) {
        if (is_v_id(cand[i])) id2_vmap[cand[i]]->print_vertex();
    }
    fmt::println("");
}

/**
 * @brief Print Vertices of ZXGraph in `cand` by qubit.
 *
 * @param cand
 */
void ZXGraph::print_vertices_by_rows(spdlog::level::level_enum lvl, std::vector<float> const& cand) const {
    if (!spdlog::should_log(lvl)) return;
    std::map<float, std::vector<ZXVertex*>> q2_vmap;
    for (auto const& v : _vertices) {
        if (!q2_vmap.contains(v->get_row())) {
            q2_vmap.emplace(v->get_row(), std::vector<ZXVertex*>(1, v));
        } else {
            q2_vmap[v->get_row()].emplace_back(v);
        }
    }
    if (cand.empty()) {
        for (auto const& [key, vec] : q2_vmap) {
            spdlog::log(lvl, "");
            for (auto const& v : vec) v->print_vertex(lvl);
            spdlog::log(lvl, "");
        }
    } else {
        for (auto const& v : cand) {
            if (q2_vmap.contains(v)) {
                spdlog::log(lvl, "");
                for (auto const& v : q2_vmap[v]) v->print_vertex(lvl);
            }
            spdlog::log(lvl, "");
        }
    }
}

/**
 * @brief Print Edges of ZXGraph
 *
 */
void ZXGraph::print_edges() const {
    for_each_edge([](EdgePair const& epair) {
        fmt::println("{:<12} Type: {}", fmt::format("({}, {})", epair.first.first->get_id(), epair.first.second->get_id()), epair.second);
    });
    fmt::println("Total #Edges: {}", get_num_edges());
}

}  // namespace zx

}  // namespace qsyn
