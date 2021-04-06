# ParallelComputingLab3
 
## Отчёт по лабораторной работе №3   

по дисциплине "Параллельные вычисления"   
студента группы ПА-18-2   
Рябова Андрея Дмитриевича   

### Постановка задачи:   

Инструкции:

![image](https://user-images.githubusercontent.com/43186510/113683828-4da43000-96cd-11eb-9bda-a16f21ecbf64.png)

### Выполнение:   

#### 1. Инициализация массива данных передачи

Без указания данных:

![image](https://user-images.githubusercontent.com/43186510/113684415-e20e9280-96cd-11eb-96a5-dc483b271390.png)

С данными:

![image](https://user-images.githubusercontent.com/43186510/113684437-e935a080-96cd-11eb-9bf7-de8d9d8f903c.png)

#### 2. Отправка данных с первого потока

Выбор первого потока, проверка ошибок, отправка, освобождение памяти:

![image](https://user-images.githubusercontent.com/43186510/113684574-0b2f2300-96ce-11eb-8f3e-372498821bff.png)

#### 3. Прием данных на втором потоке

![image](https://user-images.githubusercontent.com/43186510/113684920-6234f800-96ce-11eb-8abc-6b229cf235f2.png)

#### 4. Суммирование и обратная отправка данных

![image](https://user-images.githubusercontent.com/43186510/113685001-7973e580-96ce-11eb-913c-cc4030bd508f.png)

#### 5. Вывод данных на первом потоке

![image](https://user-images.githubusercontent.com/43186510/113685121-96a8b400-96ce-11eb-88ca-3aaea1d41b29.png)

### Результат:

Откроем папку с **exe** проекта: `C:\Users\dorge>cd /d P:\GitHub\ParallelComputingLab3\Code\x64\Debug`

Запустим программу с 2 потокам: `mpiexec -n 2 ParallelComputingLab2.exe`

![image](https://user-images.githubusercontent.com/43186510/113685544-09b22a80-96cf-11eb-8e91-4236da704cd5.png)

Введем собственные данные: `mpiexec -n 2 ParallelComputingLab2.exe -5 14 100 -45 0 2`

![image](https://user-images.githubusercontent.com/43186510/113685732-38300580-96cf-11eb-8b3c-4ee347a59589.png)

Кстати, порядок вывода случайный: зависит от того, какой первый поток доступился до **cmd**.

Попробуем сломать программу:

![image](https://user-images.githubusercontent.com/43186510/113685952-6f9eb200-96cf-11eb-9595-b2c630fb7e24.png)

![image](https://user-images.githubusercontent.com/43186510/113686009-7decce00-96cf-11eb-8c9b-fb65cf14351f.png)

Всё работает корректно.

### Код:

```
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
```
