#ifndef TIGERAPP_H
#define TIGERAPP_H

#include "MooseApp.h"

class tigerApp;

template <>
InputParameters validParams<tigerApp>();

class tigerApp : public MooseApp
{
public:
  tigerApp(InputParameters parameters);
  virtual ~tigerApp();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* TIGERAPP_H */
