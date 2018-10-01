
#include <stdlib.h>
#include <string.h>
#include "../src/sc_types.h"
#include "Robot_template.h"
/*! \file Implementation of the state machine 'robot_template'
*/

/* prototypes of all internal functions */
static void enseq_main_region_StateA_default(Robot_template* handle);
static void enseq_main_region_default(Robot_template* handle);
static void exseq_main_region_StateA(Robot_template* handle);
static void exseq_main_region(Robot_template* handle);
static void react_main_region_StateA(Robot_template* handle);
static void react_main_region__entry_Default(Robot_template* handle);
static void clearInEvents(Robot_template* handle);
static void clearOutEvents(Robot_template* handle);


void robot_template_init(Robot_template* handle)
{
		sc_integer i;
	
		for (i = 0; i < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES; ++i)
		{
			handle->stateConfVector[i] = Robot_template_last_state;
		}
		
		
		handle->stateConfVectorPosition = 0;
	
		clearInEvents(handle);
		clearOutEvents(handle);
	
}

void robot_template_enter(Robot_template* handle)
{
	/* Default enter sequence for statechart robot_template */
	enseq_main_region_default(handle);
}

void robot_template_exit(Robot_template* handle)
{
	/* Default exit sequence for statechart robot_template */
	exseq_main_region(handle);
}

sc_boolean robot_template_isActive(const Robot_template* handle)
{
	sc_boolean result = bool_false;
	int i;
	
	for(i = 0; i < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES; i++)
	{
		result = result || handle->stateConfVector[i] != Robot_template_last_state;
	}
	
	return result;
}

/* 
 * Always returns 'false' since this state machine can never become final.
 */
sc_boolean robot_template_isFinal(const Robot_template* handle)
{
   return bool_false;
}

static void clearInEvents(Robot_template* handle)
{
}

static void clearOutEvents(Robot_template* handle)
{
}

void robot_template_runCycle(Robot_template* handle)
{
	
	clearOutEvents(handle);
	for (handle->stateConfVectorPosition = 0;
		handle->stateConfVectorPosition < ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES;
		handle->stateConfVectorPosition++)
		{
			
		switch (handle->stateConfVector[handle->stateConfVectorPosition])
		{
		case Robot_template_main_region_StateA:
		{
			react_main_region_StateA(handle);
			break;
		}
		default:
			break;
		}
	}
	
	clearInEvents(handle);
}


sc_boolean robot_template_isStateActive(const Robot_template* handle, Robot_templateStates state)
{
	sc_boolean result = bool_false;
	switch (state)
	{
		case Robot_template_main_region_StateA :
			result = (sc_boolean) (handle->stateConfVector[SCVI_ROBOT_TEMPLATE_MAIN_REGION_STATEA] == Robot_template_main_region_StateA
			);
			break;
		default:
			result = bool_false;
			break;
	}
	return result;
}




/* implementations of all internal functions */

/* 'default' enter sequence for state StateA */
static void enseq_main_region_StateA_default(Robot_template* handle)
{
	/* 'default' enter sequence for state StateA */
	handle->stateConfVector[0] = Robot_template_main_region_StateA;
	handle->stateConfVectorPosition = 0;
}

/* 'default' enter sequence for region main region */
static void enseq_main_region_default(Robot_template* handle)
{
	/* 'default' enter sequence for region main region */
	react_main_region__entry_Default(handle);
}

/* Default exit sequence for state StateA */
static void exseq_main_region_StateA(Robot_template* handle)
{
	/* Default exit sequence for state StateA */
	handle->stateConfVector[0] = Robot_template_last_state;
	handle->stateConfVectorPosition = 0;
}

/* Default exit sequence for region main region */
static void exseq_main_region(Robot_template* handle)
{
	/* Default exit sequence for region main region */
	/* Handle exit of all possible states (of robot_template.main_region) at position 0... */
	switch(handle->stateConfVector[ 0 ])
	{
		case Robot_template_main_region_StateA :
		{
			exseq_main_region_StateA(handle);
			break;
		}
		default: break;
	}
}

/* The reactions of state StateA. */
static void react_main_region_StateA(Robot_template* handle)
{
}

/* Default react sequence for initial entry  */
static void react_main_region__entry_Default(Robot_template* handle)
{
	/* Default react sequence for initial entry  */
	enseq_main_region_StateA_default(handle);
}


