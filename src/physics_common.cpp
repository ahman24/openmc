#include "openmc/physics_common.h"

#include "openmc/random_lcg.h"
#include "openmc/settings.h"

namespace openmc {

//==============================================================================
// RUSSIAN_ROULETTE
//==============================================================================

void russian_roulette(Particle& p, double weight_survive)
{
  // Particle weight too high: split into multiple particles
  if (p.wgt() > weight_survive) {

    // Calculate number of splitted particles
    int np = ceil(p.wgt() / weight_survive);

    // Push new splitted particles into secondary bank
    p.wgt() /= np;
    for (int i = 0; i < np - 1; ++i) {
      p.create_secondary(p.wgt(), p.u(), p.E(), ParticleType::neutron);
    }
  }
  // Particle weight too low: execute RR
  else if (weight_survive * prn(p.current_seed()) < p.wgt()) {
    p.wgt() = weight_survive;
  } else {
    p.wgt() = 0.;
  }
}

} // namespace openmc
