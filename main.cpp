#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

void DownHeap(int *ar, int k, int n)
{
    int new_el;
    int child;
    new_el = ar[k];

    while(k <= n/2)
    {
        child = 2*k;

        if( child < n && ar[child] < ar[child+1] )
            child++;
        if( new_el >= ar[child] )
            break;
        ar[k] = ar[child];
        k = child;
    }
    ar[k] = new_el;
}

void HeapSort(int *ar, int n)
{
    int i;
    int temp;
    for(i = n / 2 - 1; i >= 0; --i)
        DownHeap(ar, i, n-1);
    for(i=n-1; i > 0; --i)
    {
        temp = ar[i];
        ar[i] = ar[0];
        ar[0] = temp;
        DownHeap(ar, 0, i-1);
    }
}

void SortPartsOfFile(char* way, int part_size)      //part_size указывает величину в килобайтах которую мы готовы выделить для первичной сортировки
{
    int *ar,use_size = 256 * part_size;
    ar = new int[use_size];
    int i = 0;
    ifstream fin (way);
    ofstream fout_t (".test", ios::trunc);
    while(!fin.eof())
    {
        for(; i < use_size && !fin.eof(); ++i )
        {
            fin >> ar[i];
        }
        HeapSort(ar, i);
        for(int p =0 ; p < i; ++p)
            fout_t <<' ' << ar[p] ;
        i=0;
    }
    fin.close();
    fout_t.close();
    ifstream fin_t (".test");
    ofstream fout (way, ios::trunc);
    while(!fin_t.eof())
    {
        fin_t >> ar[0];
        fout  <<' '<< ar[0];
    }
    delete [] ar;
    fout.close();
    fin_t.close();
    fout_t.open(".test", ios::trunc);
    fout_t.close();
}

bool Sorting(char* way)
{
    int previous, current;
    bool fl = true, ret = true;
    ifstream fin (way);
    ofstream fout_1 (".1", ios::trunc);
    ofstream fout_2 (".2", ios::trunc);
    if(!fin.eof())
    {
        fin >> previous;
        fout_1 << ' ' << previous ;
        while(!fin.eof())
        {
            fin >> current;
            if(current < previous)
            {
                fl = !fl;
                ret =false;
            }
            if(fl)
                fout_1 << ' ' << current ;
            else
                fout_2 << ' ' << current;
            previous = current;
        }
        fin.close();
        fout_1.close();
        fout_2.close();
        if(ret)
        {
            ofstream fout_1 (".1", ios::trunc);
            return true;
        }

        ofstream fout (way, ios::trunc);                    // дальше начинается ужас...
        ifstream fin_1 (".1");
        ifstream fin_2 (".2");
        int current_1, current_2, previous_1, previous_2;
        fin_1 >> current_1;
        fin_2 >> current_2;
        previous_1 = current_1;
        previous_2 = current_2;
        do
        {
            if(fin_1.eof())
            {
                fout << ' ' << current_2;
                fin_2 >> current_2;
                if(fin_2.eof()) fout << ' ' << current_2;  // затычечка:3
            }
            else if(fin_2.eof())
            {
                fout << ' ' << current_1;
                fin_1 >> current_1;
                if(fin_1.eof()) fout << ' ' << current_1;  //еще одна)
            }
            else if(current_1 <= current_2)
            {
                if(previous_1 <= current_1)
                {
                    fout << ' ' << current_1;
                    previous_1 = current_1;
                    fin_1 >> current_1;
                    if(fin_1.eof()) fout << ' ' << current_1;  // и еще..
                }
                else if(previous_2 <= current_2)
                {
                    fout << ' ' << current_2;
                    previous_2 = current_2;
                    fin_2 >> current_2;
                    if(fin_2.eof()) fout << ' ' << current_2;  // ого четвертая...
                }
                else
                {
                    previous_1 = current_1;
                    previous_2 = current_2;
                }
            }
            else if(previous_2 <= current_2)
            {
                fout << ' ' << current_2;
                previous_2 = current_2;
                fin_2 >> current_2;
                if(fin_2.eof()) fout << ' ' << current_2;  // да сколько тут их???
            }
            else if(previous_1 <= current_1)
            {
                fout << ' ' << current_1;
                previous_1 = current_1;
                fin_1 >> current_1;
                if(fin_1.eof()) fout << ' ' << current_1;   // ААААА!!! затычки захватили мой код!!!
            }
            else
            {
                previous_1 = current_1;
                previous_2 = current_2;
            }

        }
        while(!fin_1.eof() || !fin_2.eof());
        return false;
    }
    return true;
}

void FileSort(char* way, int part_size)
{
    SortPartsOfFile(way, part_size);
    bool fl = false;
    while(!fl)
    {
        cout << '*';
        fl = Sorting(way);
    }
}

int main()
{
    FileSort("test", 1);
}
