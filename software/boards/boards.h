// Allows for switching board-specific header files without changing library files

#pragma once

#if defined(BOARD_BUCKLER_REVA)
    #include "buckler.h"
#elif defined(BOARD_BUCKLER_REVB)
    #include "buckler.h"
#else
    #error "Board is not defined in boards.h"
#endif
