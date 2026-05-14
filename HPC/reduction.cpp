#include <iostream>
using namespace std;

void reduction_seq(vector <int> arr,int n)
{
    chrono::high_resolution_clock::time_point start,end; 
    chrono::duration <double> diff;

    int sum=0;
    start=chrono::high_resolution_clock::now();
    for(int i=0;i<n;i++)
    {
        sum+=arr[i];
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Sum is: "<<sum;
    cout<<"Sum time: "<<diff.count()<<endl;

    int min_val=INT_MAX,max_val=INT_MIN;

    start=chrono::high_resolution_clock::now();
    for(int i=0;i<n;i++)
    {
        if(arr[i]<min_val)
        {
            min_val=arr[i];
        }
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Min is: "<<min_val;
    cout<<"Min time: "<<diff.count()<<endl;

    start=chrono::high_resolution_clock::now();
    for(int i=0;i<n;i++)
    {
        if(arr[i]>max_val)
        {
            max_val=arr[i];
        }
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Max is: "<<max_val;
    cout<<"Max time: "<<diff.count()<<endl;

    start=chrono::high_resolution_clock::now();
    double avg=(double)sum/n;
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Avg is: "<<avg;
    cout<<"Avg time: "<<diff.count()<<endl;
}

void reduction_parallel(vector <int> arr,int n)
{
    chrono::high_resolution_clock::time_point start,end; 
    chrono::duration <double> diff;

    int sum=0;
    start=chrono::high_resolution_clock::now();
    #pragma omp parallel for reduction(+ : sum)
    for(int i=0;i<n;i++)
    {
        sum+=arr[i];
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Sum is: "<<sum;
    cout<<"Sum time: "<<diff.count()<<endl;

    int min_val=INT_MAX,max_val=INT_MIN;

    start=chrono::high_resolution_clock::now();
    #pragma omp parallel for reduction(min : min_val)
    for(int i=0;i<n;i++)
    {
        if(arr[i]<min_val)
        {
            min_val=arr[i];
        }
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Min is: "<<min_val;
    cout<<"Min time: "<<diff.count()<<endl;

    start=chrono::high_resolution_clock::now();
    #pragma omp parallel for reduction(max : max_val)
    for(int i=0;i<n;i++)
    {
        if(arr[i]>max_val)
        {
            max_val=arr[i];
        }
    }
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Max is: "<<max_val;
    cout<<"Max time: "<<diff.count()<<endl;

    start=chrono::high_resolution_clock::now();
    double avg=(double)sum/n;
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<"Avg is: "<<avg;
    cout<<"Avg time: "<<diff.count()<<endl;
}

int main()
{
    int n=10000;
    vector <int> arr(n);
    for(int i=0;i<n;i++)
    {
        arr[i]=rand()%10000;
    }
    cout<<"SEQUENTIAL"<<endl;
    reduction_seq(arr,n);
    cout<<"PARALLEL"<<endl;
    reduction_parallel(arr,n);
}


//How to run
// g++ -fopenmp your_program.cpp -o program.exe
//.\program.exe
