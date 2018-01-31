#include "tigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

// Custom Includes:
#include "ReactiveSource.h"
#include "AllenCahnColloid.h"
#include "AllenCahnRes.h"
#include "ACInterfaceColloid.h"
#include "SplitCHCResColloid.h"
#include "SplitCHParsedColloid.h"
#include "SplitCHBaseColloid.h"
#include "ReactiveSourceODE.h"
#include "ElementAverageAux.h"
#include "ElementAverage.h"
#include "ElementAverageValueConst.h"
#include "DiffusiveFluxODE.h"
#include "MechStrainEnergyAux.h"
#include "CalcConcAux.h"

template <>
InputParameters
validParams<tigerApp>()
{
  InputParameters params = validParams<MooseApp>();
  return params;
}

tigerApp::tigerApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  tigerApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  tigerApp::associateSyntax(_syntax, _action_factory);
}

tigerApp::~tigerApp() {}

// External entry point for dynamic application loading
extern "C" void
tigerApp__registerApps()
{
  tigerApp::registerApps();
}
void
tigerApp::registerApps()
{
  registerApp(tigerApp);
}

// External entry point for dynamic object registration
extern "C" void
tigerApp__registerObjects(Factory & factory)
{
  tigerApp::registerObjects(factory);
}

void
tigerApp::registerObjects(Factory & factory)
{
  registerKernel(ReactiveSource);
  registerKernel(ACInterfaceColloid);
  registerKernel(AllenCahnColloid);
  registerKernel(AllenCahnRes);
  registerKernel(SplitCHCResColloid);
  registerKernel(SplitCHParsedColloid);
  registerKernel(SplitCHBaseColloid);
  registerScalarKernel(ReactiveSourceODE);
  registerScalarKernel(DiffusiveFluxODE);
  registerAuxKernel(MechStrainEnergyAux);
  registerAuxKernel(ElementAverageAux);
  registerAuxKernel(CalcConcAux);
  registerScalarKernel(ElementAverage);
  registerPostprocessor(ElementAverageValueConst);
}

// External entry point for dynamic syntax association
extern "C" void
tigerApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  tigerApp::associateSyntax(syntax, action_factory);
}
void
tigerApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
}
