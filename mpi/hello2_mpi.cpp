#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

int
main (int argc, char *argv[])
{
   int rank, size;
   MPI_Init(&argc, &argv);

   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   char hostname[256];
   gethostname(hostname, sizeof(hostname));

   printf("rank %d of %d on <%s> \n", rank, size, hostname);

   MPI_Finalize();
}
