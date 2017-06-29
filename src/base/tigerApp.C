#include "tigerApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

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
