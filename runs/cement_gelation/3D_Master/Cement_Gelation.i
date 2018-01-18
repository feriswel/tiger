#
# Example problem showing how to use the DerivativeParsedMaterial with SplitCHParsed.
# The free energy is identical to that from SplitCHMath, f_bulk = 1/4*(1-c)^2*(1+c)^2.
#

[Mesh]
  type = GeneratedMesh
  dim = 3 #######CHANGE FOR 3D
  nx = 128
  ny = 128
  nz = 128 #######UNCOMMENT FOR 3D
  xmax = 50
  ymax = 50
  zmax = 50 #######UNCOMMENT FOR 3D
[]

[Variables]
  [./c]
  [../]
  [./w]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[ICs]
  [./cIC]
    type = RandomIC
    variable = c
    min = 0.01
    max = 0.06
  [../]
[]

[Kernels]
  [./c_dot]
    type = CoupledTimeDerivative
    variable = w
    v = c
  [../]
  [./c_res]
    type = SplitCHParsed
    variable = c
    f_name = fbulk
    kappa_name = kappa_c
    w = w
  [../]
  [./w_res]
    type = SplitCHWRes
    variable = w
    mob_name = M
  [../]
  [./nucleation]
    type = ReactiveSource
    variable = w
    v = c
    mob_name = R0
    gamma = 0.2
    v_max = 0.74
  [../]

  #CONSERVED LANGEVIN NOISE TERM
  [./conserved_langevin]
    type = ConservedLangevinNoise
    amplitude = 0.05
    multiplier = M
    variable = c
    noise = normal_noise
  [../]
[]

[UserObjects]
  [./normal_noise]
    type = ConservedNormalNoise
  [../]
[]

[AuxKernels]
  [./local_energy]
    type = TotalFreeEnergy
    variable = local_energy
    f_name = fbulk
    interfacial_vars = c
    kappa_names = kappa_c
    execute_on = timestep_end
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y z' #######ADD Z FOR 3D
    [../]
  [../]
[]

[Materials]
  [./mat]
    type = GenericConstantMaterial
    prop_names  = 'kappa_c'
    prop_values = '2.0'
  [../]
  [./reac_coef]
    #Reaction rate according to classical nucleation theory in an open volume at a
    #concentration c
    type = GenericConstantMaterial
    prop_names  = 'R0'
    prop_values = '0.005'
  [../]
  [./mob]
    type = DerivativeParsedMaterial
    args = c
    f_name = M
    constant_names = 'c_cr L W gamma nu D0'
    constant_expressions = '0.25 1.0 1e-03 0.2 0.88 c_cr^-gamma'
    function = 'D0 * L^( - gamma / nu) * ( -( c - c_cr) * L^( 1 / nu) * if((c-c_cr) < 0 , 1, 0) + W * exp(-abs( (c-c_cr) * L^( 1 / nu) )/2/W))^(gamma)'
    derivative_order = 1
    outputs = exodus
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    f_name = fbulk
    args = c
    constant_names = 'Omega c_max'
    constant_expressions = '5.0 0.74'
    function = 'c*log(c) + (c_max-c)*log(c_max-c) + Omega*c*(c_max-c)'
    enable_jit = true
    outputs = exodus
  [../]
[]

[Postprocessors]
  [./total_free_energy]
    type = ElementIntegralVariablePostprocessor
    variable = local_energy
  [../]
  [./mean_packing]
    type = ElementAverageValue
    variable = c
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

  l_max_its = 15
  l_tol = 1e-3
  nl_max_its = 15
  nl_rel_tol = 1e-7
  end_time = 500.0

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
  csv = true
  print_perf_log = true
[]
