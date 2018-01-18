#ifndef DIFFUSIVEFLUXODE_H
#define DIFFUSIVEFLUXODE_H

#include "ODETimeKernel.h"

// Forward Declaration
class DiffusiveFluxODE;

template <>
InputParameters validParams<DiffusiveFluxODE>();

class DiffusiveFluxODE : public ODETimeKernel
{
public:
  DiffusiveFluxODE(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  VariableValue & _mean_pack;
  const Real _self_diffusion;
  const Real _overlap_coef;
};

#endif // ODETIMEDERIVATIVE_H
