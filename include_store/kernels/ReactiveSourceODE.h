#ifndef REACTIVESOURCEODE_H
#define REACTIVESOURCEODE_H

#include "ODETimeKernel.h"

// Forward Declaration
class ReactiveSourceODE;

template <>
InputParameters validParams<ReactiveSourceODE>();

class ReactiveSourceODE : public ODETimeKernel
{
public:
  ReactiveSourceODE(const InputParameters & parameters);

protected:
  virtual Real computeResidual() override;
  // virtual Real computeQpJacobian() override;

  VariableValue & _mean_pack;
  VariableValue & _mean_pack_old;
  const Real _conversion_constant;
};

#endif // ODETIMEDERIVATIVE_H
