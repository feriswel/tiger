#
# Example problem showing how to use the DerivativeParsedMaterial with SplitCHParsed.
# The free energy is identical to that from SplitCHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
#

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 128
  ny = 128
  xmax = 100
  ymax = 100
[]

[Variables]
  [./phi]
  [../]
  [./c]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  [./phiIC]
    type = RandomIC
    variable = phi
    min = 0.05
    max = 0.1
  [../]
  [./cIC]
    type = RandomIC
    variable = c
    min = 1.0
    max = 1.1
  [../]
[]

[Kernels]
  #nanoparticle field
  [./phi_dot]
    type = TimeDerivative
    variable = phi
  [../]
  [./phi_res]
    type = AllenCahn
    variable = phi
    args = c
    f_name = fbulk
    mob_name = k0
  [../]
  [./phi_int]
    type = ACInterface
    variable = phi
    kappa_name = kappa_phi
    mob_name = k0
  [../]
  [./noise_source]
    type = LangevinNoise
    variable = phi
    amplitude = 5e-1
  [../]
  #ion concentration field
  [./c_dot]
    type = PoroTimeDerivative
    variable = c
  [../]
  [./c_diff]
    type = PoroDiffusion
    variable = c
  [../]
  [./c_sink]
    type = ReactiveSource
    variable = c
    value = 0.1
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = phi
    kappa_names = kappa_phi
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y'
    [../]
  [../]
[]

[Materials]
  [./mat]
    type = GenericConstantMaterial
    prop_names  = 'k0 kappa_phi ceq'
    prop_values = '1.0 1.0 1.0'
  [../]
  [./porosity]
    type = ParsedMaterial
    f_name = porosity
    args = 'phi'
    function = '1-phi'
  [../]
  [./diffusivity]
    type = ParsedMaterial
    f_name = diffusivity
    args = 'phi'
    constant_names = 'D0 alpha'
    constant_expressions = '1e5 10'
    function = 'exp(- alpha * phi)'
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    f_name = fbulk
    args = 'phi c'
    constant_names = 'Omega Ep m'
    constant_expressions = '2.5 -0.2 3.0'
    function = 'phi*log(phi) / (1 + m * sqrt(phi)) + (1-phi)*log(1-phi)
      + Omega*phi*(1-phi) + Ep*log(c)*phi'
    enable_jit = true
    outputs = exodus
  [../]
[]

[Postprocessors]
  [./top]
    type = SideIntegralVariablePostprocessor
    variable = phi
    boundary = top
  [../]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  scheme = bdf2

  petsc_options_iname = '-pc_type -sub_pc_type'
  petsc_options_value = 'asm      lu'

  l_max_its = 30
  l_tol = 1e-4
  nl_max_its = 20
  nl_rel_tol = 1e-9
  end_time = 100.0

  [./TimeStepper]
    # Turn on time stepping
    type = IterationAdaptiveDT
    dt = 0.25
    cutback_factor = 0.8
    growth_factor = 1.5
    optimal_iterations = 7
  [../]
[]

[Outputs]
  exodus = true
  print_perf_log = true
[]
