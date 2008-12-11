/*
 *  Override MPI_Finalize in C/C++.
 *
 *  $Id$
 */

#include "config.h"
#include "common.h"
#include "monitor.h"

typedef int mpi_finalize_fcn_t(void);
#ifdef MONITOR_STATIC
extern mpi_finalize_fcn_t  __real_MPI_Finalize;
#endif
static mpi_finalize_fcn_t  *real_mpi_finalize = NULL;

int
MONITOR_WRAP_NAME(MPI_Finalize)(void)
{
    int ret;

    MONITOR_DEBUG1("\n");
    MONITOR_GET_REAL_NAME_WRAP(real_mpi_finalize, MPI_Finalize);
    if (monitor_mpi_fini_count(1) == 1) {
	MONITOR_DEBUG("calling monitor_fini_mpi(), size = %d, rank = %d, ...\n",
		      monitor_mpi_comm_size(), monitor_mpi_comm_rank());
	monitor_fini_mpi();
    }
    ret = (*real_mpi_finalize)();
    monitor_mpi_fini_count(-1);

    return (ret);
}