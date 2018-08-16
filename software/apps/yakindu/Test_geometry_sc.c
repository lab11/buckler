
#include <stdlib.h>
#include <string.h>
#include "sc_types.h"
#include "Test_geometry_sc.h"
/*! \file Implementation of the state machine 'test_geometry_sc'
*/

/* prototypes of all internal functions */
static sc_boolean check_main_region_State_x_tr0_tr0(const Test_geometry_sc* handle);
static sc_boolean check_main_region_State_x_tr1_tr1(const Test_geometry_sc* handle);
static sc_boolean check_main_region_State_y_tr0_tr0(const Test_geometry_sc* handle);
static void effect_main_region_State_x_tr0(Test_geometry_sc* handle);
static void effect_main_region_State_x_tr1(Test_geometry_sc* handle);
static void effect_main_region_State_y_tr0(Test_geometry_sc* handle);
static void enact_main_region_State_x(Test_geometry_sc* handle);
static void enact_main_region_State_y(Test_geometry_sc* handle);
static void enseq_main_region_State_x_default(Test_geometry_sc* handle);
static void enseq_main_region_State_y_default(Test_geometry_sc* handle);
static void enseq_main_region__final__default(Test_geometry_sc* handle);
static void enseq_main_region_default(Test_geometry_sc* handle);
static void exseq_main_region_State_x(Test_geometry_sc* handle);
static void exseq_main_region_State_y(Test_geometry_sc* handle);
static void exseq_main_region__final_(Test_geometry_sc* handle);
static void exseq_main_region(Test_geometry_sc* handle);
static void react_main_region_State_x(Test_geometry_sc* handle);
static void react_main_region_State_y(Test_geometry_sc* handle);
static void react_main_region__final_(Test_geometry_sc* handle);
static void react_main_region__entry_Default(Test_geometry_sc* handle);
static void clearInEvents(Test_geometry_sc* handle);
static void clearOutEvents(Test_geometry_sc* handle);


void test_geometry_sc_init(Test_geometry_sc* handle)
{
		sc_integer i;
	
		for (i = 0; i < TEST_GEOMETRY_SC_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Test_geometry_sc_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
		/* Default init sequence for statechart test_geometry_sc */
		handle->iface.count = 0;
}

void test_geometry_sc_enter(Test_geometry_sc* handle)
{
	/* Default enter sequence for statechart test_geometry_sc */
	enseq_main_region_default(handle);
}

void test_geometry_sc_exit(Test_geometry_sc* handle)
{
	/* Default exit sequence for statechart test_geometry_sc */
	exseq_main_region(handle);
}

sc_boolean test_geometry_sc_isActive(const Test_geometry_sc* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < TEST_GEOMETRY_SC_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Test_geometry_sc_last_state;
	}
	
	return result;
}

sc_boolean test_geometry_sc_isFinal(const Test_geometry_sc* handle)
{
	return (handle->stateConfVector[0] == Test_geometry_sc_main_region__final_);

}

static void clearInEvents(Test_geometry_sc* handle)
{
	handle->iface.tick_raised = bool_false;
}

static void clearOutEvents(Test_geometry_sc* handle)
{
}

void test_geometry_sc_runCycle(Test_geometry_sc* handle)
{
	
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < TEST_GEOMETRY_SC_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Test_geometry_sc_main_region_State_x:
		{
			react_main_region_State_x(handle);
			break;
		}
		case Test_geometry_sc_main_region_State_y:
		{
			react_main_region_State_y(handle);
			break;
		}
		case Test_geometry_sc_main_region__final_:
		{
			react_main_region__final_(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}


sc_boolean test_geometry_sc_isStateActive(const Test_geometry_sc* handle, Test_geometry_scStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Test_geometry_sc_main_region_State_x :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TEST_GEOMETRY_SC_MAIN_REGION_STATE_X] == Test_geometry_sc_main_region_State_x
			);
			break;
		case Test_geometry_sc_main_region_State_y :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TEST_GEOMETRY_SC_MAIN_REGION_STATE_Y] == Test_geometry_sc_main_region_State_y
			);
			break;
		case Test_geometry_sc_main_region__final_ :
			result = (sc_boolean) (handle->stateConfVector[SCVI_TEST_GEOMETRY_SC_MAIN_REGION__FINAL_] == Test_geometry_sc_main_region__final_
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}

void test_geometry_scIface_raise_tick(Test_geometry_sc* handle)
{
	handle->iface.tick_raised = bool_true;
}


int8_t test_geometry_scIface_get_count(const Test_geometry_sc* handle)
{
	return handle->iface.count;
}
void test_geometry_scIface_set_count(Test_geometry_sc* handle, int8_t value)
{
	handle->iface.count = value;
}
Point test_geometry_scIface_get_pointA(const Test_geometry_sc* handle)
{
	return handle->iface.pointA;
}
void test_geometry_scIface_set_pointA(Test_geometry_sc* handle, Point value)
{
	handle->iface.pointA = value;
}

/* implementations of all internal functions */

static sc_boolean check_main_region_State_x_tr0_tr0(const Test_geometry_sc* handle)
{
	return ((bool_true) && (handle->iface.pointA.x + handle->iface.pointA.y >= 6)) ? bool_true : bool_false;
}

static sc_boolean check_main_region_State_x_tr1_tr1(const Test_geometry_sc* handle)
{
	return handle->iface.tick_raised;
}

static sc_boolean check_main_region_State_y_tr0_tr0(const Test_geometry_sc* handle)
{
	return handle->iface.tick_raised;
}

static void effect_main_region_State_x_tr0(Test_geometry_sc* handle)
{
	exseq_main_region_State_x(handle);
	enseq_main_region__final__default(handle);
}

static void effect_main_region_State_x_tr1(Test_geometry_sc* handle)
{
	exseq_main_region_State_x(handle);
	handle->iface.count += 1;
	enseq_main_region_State_y_default(handle);
}

static void effect_main_region_State_y_tr0(Test_geometry_sc* handle)
{
	exseq_main_region_State_y(handle);
	handle->iface.count += 1;
	enseq_main_region_State_x_default(handle);
}

/* Entry action for state 'State x'. */
static void enact_main_region_State_x(Test_geometry_sc* handle)
{
	/* Entry action for state 'State x'. */
	handle->iface.pointA.x += 1;
}

/* Entry action for state 'State y'. */
static void enact_main_region_State_y(Test_geometry_sc* handle)
{
	/* Entry action for state 'State y'. */
	handle->iface.pointA.y += 1;
}

/* 'default' enter sequence for state State x */
static void enseq_main_region_State_x_default(Test_geometry_sc* handle)
{
	/* 'default' enter sequence for state State x */
	enact_main_region_State_x(handle);
	handle->stateConfVector[0] = Test_geometry_sc_main_region_State_x;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for state State y */
static void enseq_main_region_State_y_default(Test_geometry_sc* handle)
{
	/* 'default' enter sequence for state State y */
	enact_main_region_State_y(handle);
	handle->stateConfVector[0] = Test_geometry_sc_main_region_State_y;
	handle->stateConfVectorPosition = 0;
}

/* Default enter sequence for state null */
static void enseq_main_region__final__default(Test_geometry_sc* handle)
{
	/* Default enter sequence for state null */
	handle->stateConfVector[0] = Test_geometry_sc_main_region__final_;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Test_geometry_sc* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state State x */
static void exseq_main_region_State_x(Test_geometry_sc* handle)
{
	/* Default exit sequence for state State x */
	handle->stateConfVector[0] = Test_geometry_sc_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for state State y */
static void exseq_main_region_State_y(Test_geometry_sc* handle)
{
	/* Default exit sequence for state State y */
	handle->stateConfVector[0] = Test_geometry_sc_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for final state. */
static void exseq_main_region__final_(Test_geometry_sc* handle)
{
	/* Default exit sequence for final state. */
	handle->stateConfVector[0] = Test_geometry_sc_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(Test_geometry_sc* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of test_geometry_sc.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Test_geometry_sc_main_region_State_x :
		{
			exseq_main_region_State_x(handle);
			break;
		}
		case Test_geometry_sc_main_region_State_y :
		{
			exseq_main_region_State_y(handle);
			break;
		}
		case Test_geometry_sc_main_region__final_ :
		{
			exseq_main_region__final_(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state State x. */
static void react_main_region_State_x(Test_geometry_sc* handle)
{
	/* The reactions of state State x. */
	if (check_main_region_State_x_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_State_x_tr0(handle);
	}  else
	{
		if (check_main_region_State_x_tr1_tr1(handle) == bool_true)
		{ 
			effect_main_region_State_x_tr1(handle);
		} 
	}
}

/* The reactions of state State y. */
static void react_main_region_State_y(Test_geometry_sc* handle)
{
	/* The reactions of state State y. */
	if (check_main_region_State_y_tr0_tr0(handle) == bool_true)
	{ 
		effect_main_region_State_y_tr0(handle);
	} 
}

/* The reactions of state null. */
static void react_main_region__final_(Test_geometry_sc* handle)
{
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Test_geometry_sc* handle)
{
	/* Default react sequence for initial entry  */
	handle->iface.count = 0;
	handle->iface.pointA.x = 0;
	handle->iface.pointA.y = 0;
	enseq_main_region_State_x_default(handle);
}


