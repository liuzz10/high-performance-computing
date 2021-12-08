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

   int the_number = -1;
   if (rank == 0)
      the_number = 42;
   
   int sender_rank = 0;
   MPI_Bcast(&the_number, 1, MPI_INT, sender_rank, MPI_COMM_WORLD);

   if (rank == 0)
   {     
      int return_values[size];
      MPI_Status status;

      return_values[0] = 0;
      for (int i = 1; i < size; i++) {
         MPI_Recv(&(return_values[i]), 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
         printf("primary rank received %d from rank %d \n", return_values[i], i);
      }
   }
   else
   {
      printf("rank %d of %d total ranks, received %d from rank zero \n", rank, size, the_number);
      
      the_number += rank;
      MPI_Send(&the_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }

   MPI_Finalize();
}
