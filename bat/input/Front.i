[Mesh]
  type = FileMesh
file = Mesh/Tube_Ref1.e
[]



[UserObjects]
  [./DensityWater]
    type = RichardsDensityConstBulk
    dens0 = 1
    bulk_mod = 1
  [../]
  [./DensityGas]
    type = RichardsDensityConstBulk
    dens0 = 0.5
    bulk_mod = 0.5
  [../]
  [./RelPermWater]
    type = RichardsRelPermPower
    simm = 0.0
    n = 2
  [../]
  [./RelPermGas]
    type = Q2PRelPermPowerGas
    simm = 0.0
    n = 3
  [../]
[]

[Variables]
  [./pp]
  [../]
  [./sat]
  [../]
[]


[AuxVariables]
[./porosity]
[../]
  [./velocity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./velocity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]

[]
[BCs]
#  [./left_water]
#    type = Q2PPiecewiseLinearSink
#    boundary = 1
#    pressures = '0 1'
#    bare_fluxes = '0 1.'
#    multiplying_fcn = 0.1
#    variable = sat
#    other_var = pp
#    var_is_porepressure = false
#    use_mobility = false
#    use_relperm = false
#    fluid_density = DensityWater
#    fluid_viscosity = 0.8
#    fluid_relperm = RelPermWater
#  [../]
#  [./right_water]
#    type = Q2PPiecewiseLinearSink
#    boundary = 3
#    pressures = '0 1'
#    bare_fluxes = '1 1'
#    variable = sat
#    other_var = pp
#    var_is_porepressure = false
#    use_mobility = true
#    use_relperm = true
#    fluid_density = DensityWater
#    fluid_viscosity = 0.8
#    fluid_relperm = RelPermWater
#  [../]
#  [./right_gas]
#    type = Q2PPiecewiseLinearSink
#    boundary = 3
#    pressures = '0 1'
#    bare_fluxes = '1 1'
#    variable = pp
#    other_var = sat
#    var_is_porepressure = true
#    use_mobility = true
#    use_relperm = true
#    fluid_density = DensityGas
#    fluid_viscosity = 0.5
#    fluid_relperm = RelPermGas
#  [../]
[./press_right]
type = DirichletBC
boundary = 1
variable = pp
value = 10000.0
[../]
[./sat_left]
type = DirichletBC
boundary = 1
variable = sat
value = 0.01
[../]
[]


[Q2P]
  porepressure = pp
  saturation = sat
  water_density = DensityWater
  water_relperm = RelPermWater
  water_viscosity = 0.8
  gas_density = DensityGas
  gas_relperm = RelPermGas
  gas_viscosity = 0.5
  diffusivity = 0.01
  output_total_masses_to = 'CSV'
  save_gas_flux_in_Q2PGasFluxResidual = true
  save_water_flux_in_Q2PWaterFluxResidual = true
  save_gas_Jacobian_in_Q2PGasJacobian = true
  save_water_Jacobian_in_Q2PWaterJacobian = true
[]


[AuxKernels]
  [./velocity_x]
    type = DarcyVelocity
    variable = velocity_x
    component = x
    execute_on = timestep_end
    darcy_pressure = pp
  [../]
  [./velocity_y]
    type = DarcyVelocity
    variable = velocity_y
    component = y
    execute_on = timestep_end
    darcy_pressure = pp
  [../]

[]


#[Kernels]
#Keep pressure in central region constant
#[./central_drop]
#  type = ConstantCoupled
#  variable = pp
#  epsilon = 0.1
#  shapes = 'circle'
#  centers_tips = '0.0 0.0 0.5'
#  press_init_val = 5.0
#[../]

#[]




[ICs]
[./saT_init]
  type = readinic
  variable = sat
dataFile = PrimaryFiles/Tube/MOOSEValues_sat_init.txt
[../]
[./press_init]
  type = readinic
  variable = pp
dataFile = PrimaryFiles/Tube/MOOSEValues_press_init.txt
[../]
[./porosity_init]
  type = readinic
  variable = porosity
dataFile = PrimaryFiles/Tube/Porosity.txt
[../]
[]
  
[Materials]
  [./rock]
    type = Q2PMaterialC
    block = 1
    por_var = porosity
    mat_permeability = '1E-2 0 0  0 1E-2 0  0 0 1E-2'
    gravity = '0 0 0'
  [../]
[]

[Postprocessors]
  [./dofs]
    type = NumDOFs
  [../]
  [./veltyx]
    type = ElementalVelocity
    variable = velocity_x
    output = PrimaryFiles/Tube/velocitiesX
  [../]
  [./veltyy]
    type = ElementalVelocity
    variable = velocity_y
    output = PrimaryFiles/Tube/velocitiesY
  [../]
  [./sat_updated_out]
    type = NodalPrintOut
    variable = sat
     output = PrimaryFiles/Tube/MOOSEValues_sat_updated
  [../]
  [./press_updated_out]
    type = NodalPrintOut
    variable = pp
     output = PrimaryFiles/Tube/MOOSEValues_press_updated
  [../]
[]


[Preconditioning]
  active = 'andy'
  [./andy]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = 'bcgs bjacobi 1E-12 1E-10 10000'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = Newton
  dt = 0.1
  end_time = 0.1
[]

[Outputs]
file_base = PrimaryFiles/Tube/MOOSEFILES/MOOSEOutput

  exodus = true
  [./CSV]
    type = CSV
  [../]
[]
