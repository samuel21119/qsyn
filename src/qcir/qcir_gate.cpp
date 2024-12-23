/****************************************************************************
  PackageName  [ qcir ]
  Synopsis     [ Define class QCirGate member functions ]
  Author       [ Design Verification Lab ]
  Copyright    [ Copyright(c) 2023 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include "./qcir_gate.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <set>

#include "./basic_gate_type.hpp"

namespace qsyn::qcir {

size_t SINGLE_DELAY   = 1;
size_t DOUBLE_DELAY   = 2;
size_t SWAP_DELAY     = 6;
size_t MULTIPLE_DELAY = 5;

QCirGate::QCirGate(size_t id, Operation const& op, QubitIdList qubits)
    : _id(id),
      _operation{op},
      _qubits{std::move(qubits)} {
    DVLAB_ASSERT(qubit_id_is_unique(_qubits), "Qubits must be unique!");
}

void QCirGate::set_operation(Operation const& op) {
    if (op.get_num_qubits() != _qubits.size()) {
        spdlog::error("Operation {} cannot be set with {} qubits!", op.get_type(), _qubits.size());
        return;
    }

    _operation = op;
}

/**
 * @brief Get delay of gate
 *
 * @return size_t
 */
size_t QCirGate::get_delay() const {
    if (get_operation() == SwapGate{})
        return SWAP_DELAY;
    if (_qubits.size() == 1)
        return SINGLE_DELAY;
    if (_qubits.size() == 2)
        return DOUBLE_DELAY;
    return MULTIPLE_DELAY;
}

std::optional<size_t> QCirGate::get_pin_by_qubit(QubitIdType qubit) const {
    auto it = std::find(_qubits.begin(), _qubits.end(), qubit);
    if (it == _qubits.end()) return std::nullopt;
    return std::distance(_qubits.begin(), it);
}

void QCirGate::set_qubits(QubitIdList qubits) {
    if (qubits.size() != _qubits.size()) {
        spdlog::error("Qubits cannot be set with different size!");
        return;
    }
    if (!qubit_id_is_unique(qubits)) {
        spdlog::error("Qubits cannot be set with duplicate qubits!");
        return;
    }

    _qubits = std::move(qubits);
}

bool QCirGate::qubit_id_is_unique(QubitIdList const& qubits) {
    return std::set(qubits.begin(), qubits.end()).size() == qubits.size();
}

}  // namespace qsyn::qcir
