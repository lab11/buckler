
#ifndef TEST_GEOMETRY_SC_H_
#define TEST_GEOMETRY_SC_H_

#include "sc_types.h"
#include "point.h"
		
#ifdef __cplusplus
extern "C" { 
#endif 

/*! \file Header of the state machine 'test_geometry_sc'.
*/


/*! Enumeration of all states */ 
typedef enum
{
	Test_geometry_sc_last_state,
	Test_geometry_sc_main_region_State_x,
	Test_geometry_sc_main_region_State_y,
	Test_geometry_sc_main_region__final_
} Test_geometry_scStates;

/*! Type definition of the data structure for the Test_geometry_scIface interface scope. */
typedef struct
{
	int8_t count;
	Point pointA;
	sc_boolean tick_raised;
} Test_geometry_scIface;


/*! Define dimension of the state configuration vector for orthogonal states. */
#define TEST_GEOMETRY_SC_MAX_ORTHOGONAL_STATES 1


/*! Define indices of states in the StateConfVector */
#define SCVI_TEST_GEOMETRY_SC_MAIN_REGION_STATE_X 0
#define SCVI_TEST_GEOMETRY_SC_MAIN_REGION_STATE_Y 0
#define SCVI_TEST_GEOMETRY_SC_MAIN_REGION__FINAL_ 0

/*! 
 * Type definition of the data structure for the Test_geometry_sc state machine.
 * This data structure has to be allocated by the client code. 
 */
typedef struct
{
	Test_geometry_scStates stateConfVector[TEST_GEOMETRY_SC_MAX_ORTHOGONAL_STATES];
	sc_ushort stateConfVectorPosition; 
	
	Test_geometry_scIface iface;
} Test_geometry_sc;


/*! Initializes the Test_geometry_sc state machine data structures. Must be called before first usage.*/
extern void test_geometry_sc_init(Test_geometry_sc* handle);

/*! Activates the state machine */
extern void test_geometry_sc_enter(Test_geometry_sc* handle);

/*! Deactivates the state machine */
extern void test_geometry_sc_exit(Test_geometry_sc* handle);

/*! Performs a 'run to completion' step. */
extern void test_geometry_sc_runCycle(Test_geometry_sc* handle);


/*! Gets the value of the variable 'count' that is defined in the default interface scope. */ 
extern int8_t test_geometry_scIface_get_count(const Test_geometry_sc* handle);
/*! Sets the value of the variable 'count' that is defined in the default interface scope. */ 
extern void test_geometry_scIface_set_count(Test_geometry_sc* handle, int8_t value);
/*! Gets the value of the variable 'pointA' that is defined in the default interface scope. */ 
extern Point test_geometry_scIface_get_pointA(const Test_geometry_sc* handle);
/*! Sets the value of the variable 'pointA' that is defined in the default interface scope. */ 
extern void test_geometry_scIface_set_pointA(Test_geometry_sc* handle, Point value);
/*! Raises the in event 'tick' that is defined in the default interface scope. */ 
extern void test_geometry_scIface_raise_tick(Test_geometry_sc* handle);


/*!
 * Checks whether the state machine is active (until 2.4.1 this method was used for states).
 * A state machine is active if it was entered. It is inactive if it has not been entered at all or if it has been exited.
 */
extern sc_boolean test_geometry_sc_isActive(const Test_geometry_sc* handle);

/*!
 * Checks if all active states are final. 
 * If there are no active states then the state machine is considered being inactive. In this case this method returns false.
 */
extern sc_boolean test_geometry_sc_isFinal(const Test_geometry_sc* handle);

/*! Checks if the specified state is active (until 2.4.1 the used method for states was called isActive()). */
extern sc_boolean test_geometry_sc_isStateActive(const Test_geometry_sc* handle, Test_geometry_scStates state);



#ifdef __cplusplus
}
#endif 

#endif /* TEST_GEOMETRY_SC_H_ */
