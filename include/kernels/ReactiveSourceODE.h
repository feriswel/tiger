#ifndef REACTIVESOURCEODE_H
#define REACTIVESOURCEODE_H

#include "ODEKernel.h"

// Forward Declaration
class ReactiveSourceODE;

template <>
InputParameters validParams<ReactiveSourceODE>();

class ReactiveSourceODE : public ODEKernel
{
public:
  ReactiveSourceODE(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const VariableValue & _mean_pack;
  const VariableValue & _mean_pack_old;
  const Real _conversion_constant;
};

#endif // ODETIMEDERIVATIVE_H
