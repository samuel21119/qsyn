/****************************************************************************
  PackageName  [ qcir/oracle ]
  Synopsis     [ read input in truth table format and convert to XAG via abc ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#pragma once

#include <base/abc/abc.h>

#include <istream>

namespace qsyn::qcir {

[[nodiscard]] Abc_Ntk_t* truth_table_to_ntk(std::istream& input, bool hex = false);
[[nodiscard]] Abc_Ntk_t* read_to_ntk(std::string file_name);

// from abc.rc
[[nodiscard]] Abc_Ntk_t* abc_resyn(Abc_Ntk_t* p_ntk, bool consider_xor = false);
[[nodiscard]] Abc_Ntk_t* abc_resyn2(Abc_Ntk_t* p_ntk, bool consider_xor = false);
[[nodiscard]] Abc_Ntk_t* abc_resyn2a(Abc_Ntk_t* p_ntk, bool consider_xor = false);
[[nodiscard]] Abc_Ntk_t* abc_resyn3(Abc_Ntk_t* p_ntk, bool consider_xor = false);

// basic aig optimization
[[nodiscard]] Abc_Ntk_t* abc_balance(Abc_Ntk_t* p_ntk, bool consider_xor = false);
[[nodiscard]] Abc_Ntk_t* abc_rewrite(Abc_Ntk_t* p_ntk, bool use_zeros = false);
[[nodiscard]] Abc_Ntk_t* abc_refactor(Abc_Ntk_t* p_ntk, bool use_zeros = false);
[[nodiscard]] Abc_Ntk_t* abc_resub(Abc_Ntk_t* p_ntk, bool use_zeros = false, int n_cuts_max = 8);

}  // namespace qsyn::qcir
