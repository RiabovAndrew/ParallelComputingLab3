#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char* argv[])
{
	int task_quantity, rank;
	int *arr;
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &task_quantity);

	printf("Hello MPI From process = %d, total number of processes: %d\n", rank, task_quantity);

	if (task_quantity < 2)
	{
		printf("Not enough processes. Choose 2 or more.\n");
	}
	else
	{
		int arr_size = 3;
		if (argc > 1)
		{
			char* end;
			arr_size = argc - 1;
			arr = (int*)malloc(arr_size * sizeof(int));
			for (int i = 0; i < argc - 1; i++)
			{
				arr[i] = strtol(argv[i + 1], &end, 10);
			}
		}
		else 
		{
			arr = (int*)malloc(arr_size * sizeof(int));
			for (int i = 0; i < arr_size; i++)
			{
				arr[i] = rand() % 20 + -9;
			}
		}
		if (rank == 0)
		{
			if (!arr)
			{
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
			MPI_Send(arr, arr_size, MPI_INT, 1, 1, MPI_COMM_WORLD);
			free(arr);
			int sum = 0;
			MPI_Recv(&sum, arr_size, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
			printf("sum = %d\n", sum);
		}
		if (rank == 1)
		{
			arr = (int*)malloc(arr_size * sizeof(int));
			MPI_Recv(arr, arr_size, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
			int sum = 0;
			printf("Array to send:\n");
			for (int i = 0; i < arr_size; i++)
			{
				sum += arr[i];
				printf("arr[%i] = %i\n", i, arr[i]);
			}
			free(arr);
			MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
	}


	MPI_Finalize();
}