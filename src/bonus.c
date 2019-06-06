#include <stdio.h>
#include <gsl/gsl_fit.h>
#include <stdlib.h>
#include <string.h>

void get_two_dimension(char* line, double** data, char *filename);
int get_row(char *filename);


int main (void)
{
  //readcsv
  char filename[] = "./data/spring_data.csv";
  char line[1024];
  double **data;
  int row, col;
  row = get_row(filename);
  col = 2;
  int n = row-1;
  double x[row],y[row];
  data = (double **)malloc(row * sizeof(int *));
  for (int i = 0; i < row; ++i){
      data[i] = (double *)malloc(col * sizeof(double));
  }//动态申请二维数组 
  get_two_dimension(line, data, filename);

  //fitting
  for(int i = 1; i<row; i++){
    x[i-1] = data[i][1] * 9.81;
    y[i-1] = data[i][0];
  }

  double c0, c1, cov00, cov01, cov11, sumsq;
  gsl_fit_linear (x, 1, y, 1, n,
                  &c0, &c1, &cov00, &cov01, &cov11,&sumsq);
  
  printf ("best fit: Y = %g + %g X\n", c0, c1);
  printf ("covariance matrix:\n");
  printf ("[ %g, %g\n  %g, %g]\n", cov00, cov01, cov01, cov11);
  printf ("sumsq = %g\n", sumsq);
  printf ("\n");

  //plot
  FILE *pipe = popen("gnuplot -persist", "w");  // Open a pipe to gnuplot

  if (pipe) {   // If gnuplot is found
    fprintf(pipe, "set term wx\n");         // set the terminal               
    fprintf(pipe, "set datafile separator ','\n"); //datafile separator ','
    fprintf(pipe, "set xlabel '|Force| (Newtons)'\n");
    fprintf(pipe, "set ylabel 'Distance (meters)'\n");
    fprintf(pipe, "set key box \n");
    fprintf(pipe, "set key top horizontal left\n");
    fprintf(pipe, "set title 'Measured Displacement of Spring\n");
    fprintf(pipe, "plot './data/spring_data.csv' using  ($2*9.81):1   title '' \n");
    fprintf(pipe, "replot './data/spring_data.csv' using ($2*9.81):($2*9.81*%.4f+%.4f) title 'y=%.4f*x+%.4f' with linespoints\n",c1,c0,c1,c0);
    fflush(pipe); //flush pipe
    fprintf(pipe,"exit \n");   // exit gnuplot
    pclose(pipe);    //close pipe
  }
  return 0;
};

void get_two_dimension(char* line, double** data, char *filename)
{
    FILE* stream = fopen(filename, "r");
    int i = 0;
    while (fgets(line, 1024, stream))//逐行读取
    {
        int j = 0;
        char *tok;
        char* tmp = strdup(line);
        for (tok = strtok(line, ","); tok && *tok; j++, tok = strtok(NULL, ",\n")){
            data[i][j] = atof(tok);//转换成浮点数 
        }//字符串拆分操作 
        i++;
        free(tmp);
    }
    fclose(stream);//文件打开后要进行关闭操作
};

int get_row(char *filename)
{
    char line[1024];
    int i;
    FILE* stream = fopen(filename, "r");
    while(fgets(line, 1024, stream)){
        i++;
    }
    fclose(stream);
    return i;
};

