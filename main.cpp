#include <iostream>
#include <fstream>
#include <cuda_runtime.h>
#include <cublas_v2.h>
#include <Eigen/Eigen>
#include <boost/timer.hpp>

using namespace std;
using namespace Eigen;

void copyFromMatrix(const MatrixXd &mat, double *array);
void copyToMatrix(const double *array, MatrixXd &mat);
double multByEigen(const MatrixXd &matA, const MatrixXd &matB, const MatrixXd &matC);
double multByCublas(const MatrixXd &matA, const MatrixXd &matB, const MatrixXd &matC, cublasHandle_t handle);
void saveMatrix(const string &file_name, const MatrixXd &mat);

int main()
{
  srand((unsigned int)time(NULL));
  cublasHandle_t handle;
  cublasCreate_v2(&handle);

  ofstream ofs("time2.csv");

  for(int i = 10; i < 3000; i = i + 10)
  {
    // Input Data Preparation
    MatrixXd matA(i, i);
    MatrixXd matB(i, i);
    MatrixXd matC(i, i);

    matA.setRandom();
    matB.setRandom();
    matC.setZero();

    double eigen_time = multByEigen(matA, matB, matC);
    double cublas_time = multByCublas(matA, matB, matC, handle);

    ofs << i << "," << eigen_time << "," << cublas_time << endl;
    cout << i << "\t: " << eigen_time << "\t" << cublas_time << endl;
  }

  cublasDestroy_v2(handle);

  return 0;
}

double multByEigen(const MatrixXd &matA, const MatrixXd &matB, const MatrixXd &matC)
{
  boost::timer time;
  MatrixXd retA = matA * matB + matC;
  return time.elapsed();
}

double multByCublas(const MatrixXd &matA, const MatrixXd &matB, const MatrixXd &matC, cublasHandle_t handle)
{
  // Host
  double *h_matA, *h_matB, *h_matC;
  h_matA = (double *)malloc(matA.rows() * matA.cols() * sizeof(double));
  h_matB = (double *)malloc(matB.rows() * matB.cols() * sizeof(double));
  h_matC = (double *)malloc(matC.rows() * matC.cols() * sizeof(double));
  copyFromMatrix(matA, h_matA);
  copyFromMatrix(matB, h_matB);
  copyFromMatrix(matC, h_matC);

  // Device
  boost::timer time;

  double *d_matA, *d_matB, *d_matC;
  double alpha = 1.0, beta = 1.0;

  cudaMalloc((void **)&d_matA, matA.rows() * matA.cols() * sizeof(double));
  cudaMalloc((void **)&d_matB, matB.rows() * matB.cols() * sizeof(double));
  cudaMalloc((void **)&d_matC, matC.rows() * matC.cols() * sizeof(double));

  cublasSetMatrix(matA.rows(), matA.cols(), sizeof(double), h_matA, matA.rows(), d_matA, matA.rows());
  cublasSetMatrix(matB.rows(), matB.cols(), sizeof(double), h_matB, matB.rows(), d_matB, matB.rows());
  cublasSetMatrix(matC.rows(), matC.cols(), sizeof(double), h_matC, matC.rows(), d_matC, matC.rows());

  cublasDgemm_v2(handle, CUBLAS_OP_N, CUBLAS_OP_N,
                 matA.rows(), matB.cols(), matB.rows(),
                 &alpha, d_matA, matA.rows(),
                 d_matB, matB.rows(),
                 &beta, d_matC, matC.rows());
  cublasGetMatrix(matC.rows(), matC.cols(), sizeof(double), d_matC, matC.rows(), h_matC, matC.rows());

  cudaFree(d_matA);
  cudaFree(d_matB);
  cudaFree(d_matC);

  double elapsed_time = time.elapsed();

  MatrixXd result(matC.rows(), matC.cols());
  copyToMatrix(h_matC, result);

  free(h_matA);
  free(h_matB);
  free(h_matC);

  return elapsed_time;
}

void saveMatrix(const string &file_name, const MatrixXd &mat)
{
  ofstream ofs(file_name.c_str());
  for(int y = 0; y < mat.rows(); ++y)
  {
    for(int x = 0; x < mat.cols(); ++x)
    {
      ofs << mat(y, x) << ",";
    }
    ofs << endl;
  }
}

void copyFromMatrix(const MatrixXd &mat, double *array)
{
  int rows = mat.rows();
  int cols = mat.cols();
  for(int x = 0; x < cols; ++x)
  {
    for(int y = 0; y < rows; ++y)
    {
      array[x * rows + y] = mat(y, x);
    }
  }
}

void copyToMatrix(const double *array, MatrixXd &mat)
{
  int rows = mat.rows();
  int cols = mat.cols();
  for(int x = 0; x < cols; ++x)
  {
    for(int y = 0; y < rows; ++y)
    {
      mat(y, x) = array[x * rows + y];
    }
  }
}
