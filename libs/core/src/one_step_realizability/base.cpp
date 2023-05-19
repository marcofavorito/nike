//
// Created by marcofavorito on 18/05/23.
//


#include <memory>
#include <nike/one_step_realizability/base.hpp>
#include <nike/one_step_realizability/bdd.hpp>
#include "nike/one_step_realizability/smt.hpp"


namespace nike{
  namespace core{
    std::unique_ptr<OneStepRealizabilityChecker> get_default_realizability_checker() {
      return std::make_unique<BddOneStepRealizabilityChecker>();
    }
  }
}