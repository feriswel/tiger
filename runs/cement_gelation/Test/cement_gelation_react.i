[Mesh]
  type = GeneratedMesh
  dim = 2 #######CHANGE FOR 3D
  nx = 128
  ny = 128
  #nz = 64 #######UNCOMMENT FOR 3D
  xmax = 100
  ymax = 100
  #zmax = 50 #######UNCOMMENT FOR 3D
  block_id = 0
  block_name = 'mesh_domain'
[]

[Variables]
  #Phase-field variables
  [./phi]
  [../]
  [./w]
  [../]
[]

[AuxVariables]
  [./local_energy]
    order = CONSTANT
    family = MONOMIAL
  [../]

  #Scalar variable
  [./c]
    family = SCALAR
    order = FIRST
    initial_condition = 1
  [../]
  [./phi_mean]
    order = FIRST
    family = SCALAR
    initial_condition = 0.001
  [../]
[]

[ICs]
  [./IC_phi]
    type = RandomIC
    variable = phi
    min = 0.1
    max = 0.9
  [../]
[]

[Kernels]
  [./phi_dot]
    # time derivative operator is applied to "v",
    # and then contributes to the residual of "variable".
    type = CoupledTimeDerivative
    variable = w
    v = phi
  [../]

  #my kernel for the reaction rate
  [./ACbulk]
    type = AllenCahnRes
    variable = w
    f_name = fbulk
    mob_name = R0 #reaction rate coefficient
  [../]
  [./phi_res]
    type = SplitCHParsedColloid
    variable = phi
    f_name = fbulk
    kappa_name = kappa_phi
    w = w
  [../]

#  CONSERVED LANGEVIN NOISE TERM
#  [./conserved_langevin]
#    type = ConservedLangevinNoise
#    amplitude = 0.05
#    multiplier = R0
#    variable = phi
#    noise = normal_noise
#  [../]
[]

[AuxScalarKernels]
  #scalar kernels

  [./reac_diff_res]
    type = CalcConcAux
    variable = c
    phi_mean = phi_mean
    reaction_coef = 1.0
    diffusion_coef = 1.0
    gamma = 2.0
  [../]

  [./calc_phi_mean]
    type = ElementAverageAux
    variable = phi_mean
    user_object = phi_mean_uo
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


[UserObjects]
  #Langevin noise term
  [./normal_noise]
    type = ConservedNormalNoise
  [../]

  #mean packing density
  [./phi_mean_uo]
    type = ElementAverageValue
    variable = phi
    execute_on = 'initial timestep_begin'
    output = none
  [../]
[]

[BCs]
  [./Periodic]
    [./all]
      auto_direction = 'x y' #######ADD Z FOR 3D
    [../]
  [../]
[]

[Materials]

  [./mob_reac]
    type = DerivativeParsedMaterial
    args = phi
    f_name = M
    constant_names = 'phi_cr L W gamma nu'
    constant_expressions = '0.25 1.0 1e-03 0.2 0.88'
    function = 'phi_cr^-gamma * L^( - gamma / nu) * ( -( phi - phi_cr) * L^( 1 / nu) * if(( phi - phi_cr) < 0 , 1, 0) + W * exp(-abs( ( phi - phi_cr) * L^( 1 / nu) )/2/W))^(gamma)'
    derivative_order = 1
    outputs = exodus
  [../]
  [./gradient_energy_coef]
    type = DerivativeParsedMaterial
    args = phi
    f_name = kappa_phi
    constant_names = 'kappa_0'
    constant_expressions = '2.0'
    function = 'kappa_0 / (phi * (1.0 - phi))'
    derivative_order = 1
    outputs = exodus
  [../]
  [./mob_diff]
    type = DerivativeParsedMaterial
    args = phi
    f_name = R0
    function = '1.0'
    derivative_order = 1
    outputs = exodus
  [../]
  [./free_energy]
    type = DerivativeParsedMaterial
    f_name = fbulk
    args = phi
    constant_names = 'Omega'
    constant_expressions = '4.0'
    function = ' phi * log(phi) + ( 1 - phi) * log( 1 - phi) + Omega * phi * ( 1 - phi)'
    enable_jit = true
    outputs = exodus
  [../]
[]

[Postprocessors]
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
  petsc_options_value = 'asm lu'

  l_max_its = 15
  l_tol = 1e-3
  nl_max_its = 15
  nl_rel_tol = 1e-7
  end_time = 30.0
  dt = 0.01

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
  print_linear_residuals = true
[]
