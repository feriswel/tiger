/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef SPLITCHPARSEDCOLLOID_H
#define SPLITCHPARSEDCOLLOID_H

#include "SplitCHCResColloid.h"
#include "JvarMapInterface.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class SplitCHParsedColloid;

template <>
InputParameters validParams<SplitCHParsedColloid>();

/**
 * CHParsed uses the Free Energy function and derivatives
 * provided by a DerivativeParsedMaterial.
 * This is the split operator variant.
 * \see CHParsed
 */
class SplitCHParsedColloid : public SplitCHCResColloid
{
public:
  SplitCHParsedColloid(const InputParameters & parameters);

  virtual void initialSetup();

protected:
  virtual Real computeDFDC(PFFunctionType type);
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

private:
  const unsigned int _nvar;
  const MaterialProperty<Real> & _dFdc;
  const MaterialProperty<Real> & _d2Fdc2;

  std::vector<const MaterialProperty<Real> *> _d2Fdcdarg;
};

#endif // SPLITCHPARSED_H
