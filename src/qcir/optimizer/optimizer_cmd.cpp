/****************************************************************************
  PackageName  [ qcir/optimizer ]
  Synopsis     [ Define optimizer package commands ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <spdlog/spdlog.h>

#include <string>

#include "../qcir.hpp"
#include "../qcir_mgr.hpp"
#include "./optimizer.hpp"
#include "cli/cli.hpp"
#include "util/data_structure_manager_common_cmd.hpp"

using namespace dvlab::argparse;
using dvlab::CmdExecResult;
using dvlab::Command;

extern bool stop_requested();

namespace qsyn::qcir {

//----------------------------------------------------------------------
//    Optimize
//----------------------------------------------------------------------
Command qcir_optimize_cmd(QCirMgr& qcir_mgr) {
    return {"optimize",
            [](ArgumentParser& parser) {
                parser.description("optimize QCir");

                parser.add_argument<bool>("-p", "--physical")
                    .default_value(false)
                    .action(store_true)
                    .help("optimize physical circuit, i.e preserve the swap path");
                parser.add_argument<bool>("-c", "--copy")
                    .default_value(false)
                    .action(store_true)
                    .help("copy a circuit to perform optimization");
                parser.add_argument<bool>("-s", "--statistics")
                    .default_value(false)
                    .action(store_true)
                    .help("count the number of rules operated in optimizer.");
                parser.add_argument<bool>("-t", "--tech")
                    .default_value(false)
                    .action(store_true)
                    .help("Only perform optimizations preserving gate sets and qubit connectivities.");
            },
            [&](ArgumentParser const& parser) {
                if (!dvlab::utils::mgr_has_data(qcir_mgr)) return CmdExecResult::error;
                Optimizer optimizer;
                std::optional<QCir> result;
                std::string procedure_str{};
                if (parser.get<bool>("--tech") || !qcir_mgr.get()->get_gate_set().empty()) {
                    result        = optimizer.trivial_optimization(*qcir_mgr.get());
                    procedure_str = "Tech Optimize";
                } else {
                    result        = optimizer.basic_optimization(*qcir_mgr.get(), {.doSwap             = !parser.get<bool>("--physical"),
                                                                                   .separateCorrection = false,
                                                                                   .maxIter            = 1000,
                                                                                   .printStatistics    = parser.get<bool>("--statistics")});
                    procedure_str = "Optimize";
                }
                if (result == std::nullopt) {
                    spdlog::error("Fail to optimize circuit.");
                    return CmdExecResult::error;
                }

                if (parser.get<bool>("--copy")) {
                    qcir_mgr.add(qcir_mgr.get_next_id(), std::make_unique<QCir>(std::move(*result)));
                } else {
                    qcir_mgr.set(std::make_unique<QCir>(std::move(*result)));
                }

                if (stop_requested()) {
                    procedure_str += "[INT]";
                }

                qcir_mgr.get()->add_procedure(procedure_str);

                return CmdExecResult::done;
            }};
}

}  // namespace qsyn::qcir
