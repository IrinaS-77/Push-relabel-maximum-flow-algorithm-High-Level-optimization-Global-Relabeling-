#include <iostream>
#include <fstream>
#include <iomanip>
#include <deque>
#include <ctime>
#include <chrono>
#include <climits>
#include <vector>
#include <cmath> 

using namespace std;

int HLO(int*, double*, double);

void Push(double** matrix_pixel, double* matrix_S,double* matrix_T,int col,int img_cols, int i, int j, double value, double* e)
{
    //cout <<"Push ������:"<< i << " " << j << " " <<"�� �����: "<<value<< endl;
    if (j == 0)
    {
        matrix_pixel[i - 1][0] -= value;
        matrix_S[i] += value;
        e[j] += value;
        e[i] -= value;
        return;
    }
    if (j == col - 1)
    {
        matrix_pixel[i - 1][5] -= value;
        matrix_T[i] += value;
        e[j] += value;
        e[i] -= value;
        return;
    }
    if (j - i == 1)
    {
        matrix_pixel[i - 1][3] -= value;
        matrix_pixel[j - 1][1] += value;
        //������ ������� i � �� ������ �����
        e[j] += value;
        e[i] -= value;
        return;
    }
    if (i - j == 1)
    {
        matrix_pixel[i - 1][1] -= value;
        matrix_pixel[j - 1][3] += value;
        //������ ������� i � �� ����� �����
        e[j] += value;
        e[i] -= value;
        return;
    }
    if (j - i == img_cols)
    {
        matrix_pixel[i - 1][4]-= value;
        matrix_pixel[j - 1][2] += value;

        //������ ������� i � �� ������ �����
        e[j] += value;
        e[i] -= value;
        return;
    }
    if (i - j == img_cols)
    {
        matrix_pixel[i - 1][2] -= value;
        matrix_pixel[j - 1][4] += value;
        //������ ������� i � �� ������� �����
        e[j] += value;
        e[i] -= value;
        return;
    }
}

void Breadth_first_search(double** matrix_pixel,double* matrix_S,double* matrix_T, int col, int* d,int img_cols, bool type = 1) {
    deque<int> dec;
    if (type == 1) {
        d[col - 1] = 0;
        dec.push_back(col - 1);
    }
    else if (type == 0) {
        d[0] = col;
        dec.push_back(0);
    }
    int max_value = 2 * col;

    int i = dec.front();
    dec.pop_front();
    if (i == 0) 
    {
        //��������� ��� �� ������, � ���� ��� ����� �������� � ����
        for (int j = 1;j < col - 1;++j) 
        {
            if (matrix_pixel[j-1][0] > 0) 
            {
                if (d[j] == max_value) 
                {
                    d[j] = d[i] + 1;
                    dec.push_back(j);
                }
            }
        }

        int curr_i;
        while (!dec.empty()) 
        {
            //���� �� ����� ����, ��� ������ ������� ����� ������������� 4 ����� �������� � ���� �� ��� �������
            int i = dec.front();
            dec.pop_front();

            //������� �����
            curr_i = i - img_cols;
            if (curr_i > 0) 
            {
                //���� ����������� ������� ������ ����� ���� ����������
                if (matrix_pixel[curr_i - 1][4] > 0) 
                {
                    if (d[curr_i] == max_value)
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //������ �����
            curr_i = i + img_cols;
            if (curr_i < col - 1) 
            {
                //���� ����������� ������� ���� �� ������ ����� ����� ����������
                if (matrix_pixel[curr_i - 1][2] > 0) 
                {
                    if (d[curr_i] == max_value) 
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //����� �����
            curr_i = i - 1;
            //��� ������ ���� i=1????
            if (curr_i % img_cols != 0) 
            {
                //���� ����������� ������� ���� �� ����� ����� ����������
                if (matrix_pixel[curr_i - 1][3] > 0) 
                {
                    if (d[curr_i] == max_value) 
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }

            }
            //������ �����
            curr_i = i + 1;
            if ((curr_i-1) % img_cols != 0) 
            {
                //���� ����������� ������� ���� �� ������ ����� ����������
                if (matrix_pixel[curr_i - 1][1] > 0) 
                {
                    if (d[curr_i] == max_value) 
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //���� T
            if (d[col - 1] == max_value) 
            {
                //���� ���� ��� ��� �� ���������
                //��������� ����� ��� �� ���� �������
                if (matrix_T[i] > 0) 
                {
                    d[col - 1] = d[i] + 1;
                }
            }

        }

    }
    else 
    {
        //��������� ��� �� ����� � ���� ��� ����� �������� � ����
        for (int j = 1;j < col - 1;++j)
        {
            if (matrix_pixel[j-1][5] > 0) 
            {
                if (d[j] == max_value) 
                {
                    d[j] = d[i] + 1;
                    dec.push_back(j);
                }
            }
        }

        int curr_i;
        while (!dec.empty())
        {
            //���� �� ����� ����, ��� ������ ������� ����� ������������� 4 ����� �������� � ���� �� ��� �������
            int i = dec.front();
            dec.pop_front();

            //������� �����
            curr_i = i - img_cols;
            if (curr_i > 0)
            {
                //���� ����������� ������� ������ ����� ���� ����������
                if (matrix_pixel[curr_i - 1][4] > 0)
                {
                    if (d[curr_i] == max_value)
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //������ �����
            curr_i = i + img_cols;
            if (curr_i < col - 1)
            {
                //���� ����������� ������� ���� �� ������ ����� ����� ����������
                if (matrix_pixel[curr_i - 1][2] > 0)
                {
                    if (d[curr_i] == max_value)
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //����� �����
            curr_i = i - 1;
            //��� ������ ���� i=1???
            if (curr_i % img_cols != 0)
            {
                //���� ����������� ������� ���� �� ����� ����� ����������
                if (matrix_pixel[curr_i - 1][3] > 0)
                {
                    if (d[curr_i] == max_value)
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }

            }
            //������ �����
            curr_i = i + 1;
            if ((curr_i-1) % img_cols != 0)
            {
                //���� ����������� ������� ���� �� ������ ����� ����������
                if (matrix_pixel[curr_i - 1][1] > 0)
                {
                    if (d[curr_i] == max_value)
                    {
                        d[curr_i] = d[i] + 1;
                        dec.push_back(curr_i);
                    }
                }
            }
            //����� S
            if (d[0] == max_value)
            {
                //���� ����� ��� ��� �� ���������
                //��������� ����� ��� �� ���� �������
                if (matrix_S[i] > 0)
                {
                    d[0] = d[i] + 1;
                }
            }
        }




    }
    //����� �������������� ���� ������ ��������� ������, �� ��� ����� �� ������ �����������
}

int HLO(int* h, double* e, double col) {
    int max_h = 0, max_e = 0, vertex = 0;
    for (int i = 1;i < col-1;++i) {
        if (e[i] > 0) {
            if (h[i] >= max_h) {
                vertex = i;
                max_e = e[i];
                max_h = h[i];
            }
        }
    }
    return vertex;
}

void G_R(double** matrix_pixel,double* matrix_S,double* matrix_T,int col,int img_cols,int* h) {
    int* d1 = new int[col];
    int* d2 = new int[col];

    int max_value = 2 * col;

    for (int i = 0;i < col;++i) {
        d1[i] = max_value;
        d2[i] = max_value;
    }

    Breadth_first_search(matrix_pixel,matrix_S,matrix_T, col, d1,img_cols, 1);
    //cout << "����������" << endl;
    Breadth_first_search(matrix_pixel, matrix_S, matrix_T, col, d2, img_cols, 0);
    //cout << "����������" << endl;
    for (int i = 0; i < col; ++i) {
        if (d1[i] == max_value) {
            h[i] = d2[i];
        }
        else
            h[i] = d1[i];
    }
    delete[] d1;
    delete[] d2;
}

int* algoritm(double** matrix_pixel,double* matrix_S,double* matrix_T,int col,int img_cols) {
    int* h = new int[col];
    double* e = new double[col];
    for (int i = 1;i < col-1;++i) {
        if (matrix_S[i] > 0) {
            e[i] = matrix_S[i];
            matrix_pixel[i-1][0] += matrix_S[i];
            matrix_S[i] = 0;
        }
        else {
            e[i] = 0;
        }
    }
    e[0] = 0;
    G_R(matrix_pixel,matrix_S,matrix_T, col,img_cols, h);

    h[0] = col;

    int operations, chosen_vertex, count = 0, j;
    int minH;
    int globalrelabe_tick = 500;
    double ghoul;
    int curr_i;
    int check_;
    double value;
    double sum_e;
    do {
        chosen_vertex = HLO(h, e, col);
        if ((chosen_vertex == col - 1)||(chosen_vertex==0)) break;
        while (e[chosen_vertex] > 0) {
            operations = 1;

            //��������� �������� ���� �����
            //����� S
            ghoul = matrix_pixel[chosen_vertex - 1][0];
            if ((ghoul > 0) && (h[chosen_vertex] - 1 == h[0]))
            {
                //���� ����������� ������� ����, �� �� ������ ����������� � �����
                Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, 0, min(ghoul, e[chosen_vertex]), e);
                operations--;
            }

            //������� �����
            curr_i = chosen_vertex - img_cols;
            if ((curr_i > 0)&&(operations>0))
            {
                //���� ����������� ������� ������ ����� ���� ����������
                ghoul = matrix_pixel[chosen_vertex - 1][2];
                if ((ghoul > 0) && (h[chosen_vertex] - 1 == h[curr_i]))
                {
                    Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, curr_i, min(ghoul, e[chosen_vertex]), e);
                    operations--;
                }
            }
            //����� �����
            curr_i = chosen_vertex - 1;
            if ((curr_i % img_cols != 0) && (operations > 0))
            {
                //���� ����������� ������� ���� �� ����� ����� ����������
                ghoul = matrix_pixel[chosen_vertex - 1][1];
                if ((ghoul > 0) && (h[chosen_vertex] - 1 == h[curr_i]))
                {
                    Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, curr_i, min(ghoul, e[chosen_vertex]), e);
                    operations--;
                }

            }
            //������ �����
            curr_i = chosen_vertex + 1;
            if (((curr_i - 1) % img_cols != 0) && (operations > 0))
            {
                //���� ����������� ������� ���� �� ������ ����� ����������
                ghoul = matrix_pixel[chosen_vertex - 1][3];
                if ((ghoul > 0) && (h[chosen_vertex] - 1 == h[curr_i]))
                {
                    Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, curr_i, min(ghoul, e[chosen_vertex]), e);
                    operations--;
                }
            }
            //������ �����
            curr_i = chosen_vertex + img_cols;
            if ((curr_i < col - 1) && (operations > 0))
            {
                //���� ����������� ������� ���� �� ������ ����� ����� ����������
                ghoul = matrix_pixel[chosen_vertex - 1][4];
                if ((ghoul > 0) && (h[chosen_vertex] - 1 == h[curr_i]))
                {
                    Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, curr_i, min(ghoul, e[chosen_vertex]), e);
                    operations--;
                }
            }

            //���� T
            ghoul = matrix_pixel[chosen_vertex - 1][5];
            if (((ghoul > 0) && (h[chosen_vertex] - 1 == h[col-1])) && (operations > 0))
            {
                Push(matrix_pixel, matrix_S, matrix_T, col, img_cols, chosen_vertex, col - 1, min(ghoul, e[chosen_vertex]), e);
                operations--;
            }
            

            if (operations == 1) {
                minH = 2 * col;
                //��������� �������� ���� �����
                //����� S
                j = 0;
                if ((matrix_pixel[chosen_vertex - 1][j] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                {
                    minH = h[j];
                }

                //������� �����
                j = chosen_vertex - img_cols;
                if (j > 0)
                {
                    //���� ����������� ������� ������ ����� ���� ����������
                    if ((matrix_pixel[chosen_vertex - 1][2] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                    {
                        minH = h[j];
                    }
                }
                //����� �����
                j = chosen_vertex - 1;
                if (j % img_cols != 0)
                {
                    //���� ����������� ������� ���� �� ����� ����� ����������
                    if ((matrix_pixel[chosen_vertex - 1][1] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                    {
                        minH = h[j];
                    }
                }
                //������ �����
                j = chosen_vertex + 1;
                if ((j - 1) % img_cols != 0)
                {
                    //���� ����������� ������� ���� �� ������ ����� ����������
                    if ((matrix_pixel[chosen_vertex - 1][3] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                    {
                        minH = h[j];
                    }
                }
                //������ �����
                j = chosen_vertex + img_cols;
                if (j < col - 1)
                {
                    //���� ����������� ������� ���� �� ������ ����� ����� ����������
                    if ((matrix_pixel[chosen_vertex - 1][4] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                    {
                        minH = h[j];
                    }
                }

                //���� T
                j = col - 1;
                if ((matrix_pixel[chosen_vertex - 1][5] > 0) && (h[j] >= h[chosen_vertex]) && (h[j] <= minH))
                {
                    minH = h[j];
                }

                h[chosen_vertex] = minH + 1;

                //cout << endl<<"������ ������� �������  "<<h[chosen_vertex] << endl;
            }
        }
        count++;
        if (count == globalrelabe_tick) {
            count = 0;
            G_R(matrix_pixel, matrix_S, matrix_T, col, img_cols, h);
            
            /*
            sum_e = 0;
            for (int i = 1;i < col - 1;++i) {
                sum_e += e[i];
            }
            cout << sum_e << endl;
            */
            
            
        }
    } while (chosen_vertex != col - 1);
    //cout << "�����: " << e[col - 1] << endl;

    int* d2 = new int[col];
    for (int i = 0;i < col;++i) 
    {
        d2[i] = 2 * col;
    }
    Breadth_first_search(matrix_pixel, matrix_S, matrix_T, col, d2, img_cols, 1);

    //for (int i = 1;i < col - 1;++i)
    //{
    //   cout << d2[i] << " ";
    //}
    return d2;
}
