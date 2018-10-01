
#ifndef ROBOT_TEMPLATE_H_
#define ROBOT_TEMPLATE_H_

#include "../src/sc_types.h"
		
#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'robot_template'.
*/


/*! Enumeration of all states */ 
typedef enum
{
	Robot_template_last_state,
	Robot_template_main_region_StateA
} Robot_templateStates;


/*! Define dimension of the state configuration vector for orthogonal states. */
#define ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_ROBOT_TEMPLATE_MAIN_REGION_STATEA 0

/*! 
 * Type definition of the data structure for the Robot_template state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	Robot_templateStates stateConfVector[ROBOT_TEMPLATE_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
} Robot_template;


/*! Initializes the Robot_template state machine data structures. Must be called before first usage.*/
extern void robot_template_init(Robot_template* handle);

/*! Activates the state machine */
extern void robot_template_enter(Robot_template* handle);

/*! Deactivates the state machine */
extern void robot_template_exit(Robot_template* handle);

/*! Performs a 'run to completion' step. */
extern void robot_template_runCycle(Robot_template* handle);



/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean robot_template_isActive(const Robot_template* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean robot_template_isFinal(const Robot_template* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean robot_template_isStateActive(const Robot_template* handle, Robot_templateStates state);



#ifdef __cplusplus
}
#endif 

#endif /* ROBOT_TEMPLATE_H_ */
