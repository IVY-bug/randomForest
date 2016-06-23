#include "run.h"

void runOneThread(string trainData, string testData, string outputData) {
	rf* forest = rf::getInstance(trainData);
    forest->train(1);
    forest->predict(testData, outputData);
}

void runMpi(string trainData, int argc, char * argv[]) {
	/*init mpi*/
	int isInitialized = 0;
    int size, rank;
    MPI_Status status;
    MPI_Initialized(&isInitialized);
    if (!isInitialized) {
        MPI_Init(&argc, &argv);
    }
    // Get the number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Get the rank of this procressor.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	rf* forest = rf::getInstance(trainData);
	forest->train_mpi(argc, argv);
    if (rank == 0) {
        int dataOut = 1;
        for(int pr = 1; pr < size; pr++) {
            MPI_Send(&dataOut, 1, MPI_INT, pr, 0, MPI_COMM_WORLD);
        }
        Info("Finish training!");
    } else {
        int message;
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
	
	
}

void runMpi2(string trainData, string testData, string outputData, int argc, char* argv[]) {
    int isInitialized = 0;
    int size, rank;
    MPI_Status status;
    MPI_Initialized(&isInitialized);
    if (!isInitialized) {
        MPI_Init(&argc, &argv);
    }
    // Get the number of processors
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Get the rank of this procressor.
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    rf* forest = rf::getInstance(trainData);
    forest->predict_mpi(argc, argv, testData, outputData);
    if (rank == 0) {
        int dataOut = 1;
        for(int pr = 1; pr < size; pr++) {
            MPI_Send(&dataOut, 1, MPI_INT, pr, 0, MPI_COMM_WORLD);
        }
        Info("Finish predicting!");
    } else {
        int message;
        MPI_Recv(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

}