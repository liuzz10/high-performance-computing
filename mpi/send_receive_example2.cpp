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

   if (rank == 0)
   {
      int send_number = 42;
      printf("rank %d of %d total ranks, sending %d to all te other ranks \n", rank, size, send_number);
      for (int i=1; i<size; i++)
         MPI_Send(&send_number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      
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
      int recv_number;
      MPI_Status status;
      MPI_Recv(&recv_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      printf("rank %d of %d total ranks, received %d from rank zero \n", rank, size, recv_number);
      
      recv_number += rank;
      MPI_Send(&recv_number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }

   MPI_Finalize();
}
