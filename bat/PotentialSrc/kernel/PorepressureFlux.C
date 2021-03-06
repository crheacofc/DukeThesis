/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "PorepressureFlux.h"

// MOOSE includes
#include "Assembly.h"
#include "MooseVariable.h"
#include "SystemBase.h"

// libMesh includes
#include "libmesh/quadrature.h"

// C++ includes
#include <iostream>

template <>
InputParameters
validParams<PorepressureFlux>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredParam<UserObjectName>(
      "fluid_density",
      "A RichardsDensity UserObject that defines the fluid density as a function of pressure.");
  params.addRequiredCoupledVar("saturation_variable", "The variable representing fluid saturation");
  params.addRequiredParam<UserObjectName>(
      "fluid_relperm",
      "A RichardsRelPerm UserObject (eg RichardsRelPermPower) that defines the "
      "fluid relative permeability as a function of the saturation variable");
  params.addRequiredParam<Real>("fluid_viscosity", "The fluid dynamic viscosity");
  params.addClassDescription("Flux according to Darcy-Richards flow.  The Variable for this Kernel "
                             "should be the porepressure.");
  return params;
}

PorepressureFlux::PorepressureFlux(const InputParameters & parameters)
  : Kernel(parameters),
    _density(getUserObject<RichardsDensity>("fluid_density")),
    _sat(coupledNodalValue("saturation_variable")),
    _sat_var(coupled("saturation_variable")),
    _relperm(getUserObject<RichardsRelPerm>("fluid_relperm")),
    _viscosity(getParam<Real>("fluid_viscosity")),
    _gravity(getMaterialProperty<RealVectorValue>("gravity")),
    _permeability(getMaterialProperty<RealTensorValue>("permeability"))

{
}



Real
PorepressureFlux::computeQpResidual()
{
  // note this is not the complete residual:
  // the upwind mobility parts get added in computeResidual
  Real coeff =  _relperm.relperm(_u[_qp]) * _density.density(_u[_qp]) / _viscosity;

  return -_grad_test[_i][_qp] *
         (coeff * _permeability[_qp] * (_grad_u[_qp] - _density.density(_u[_qp]) * _gravity[_qp]));
}
Real
PorepressureFlux::computeQpJacobian()
{
    return 0;
}

Real
PorepressureFlux::computeQpOffDiagJacobian()
{
  return 0.0;

}
