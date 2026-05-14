#include <vector>
#include <iostream>
#include <chrono>
using namespace std;

void bubble(vector <int> arr,int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;i++)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void bubble_parallel(vector <int> arr,int n)
{
    for(int i=0;i<n-1;i++)
    {
        #pragma omp parallel for
        for(int j=0;j<n-1;j+=2)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
        #pragma omp parallel for
        for(int j=1;j<n-1;j+=2)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge_sort_common(vector <int> arr, int start, int end,int mid)
{
    int n1=mid-start+1;
    int n2=end-mid;

    vector<int> left(n1);
    vector<int> right(n2);

    for(int i=0;i<n1;i++)
    {
        left[i]=arr[start+i];
    }
    for(int i=0;i<n2;i++)
    {
        right[i]=arr[mid+i+1];
    }

    int i=0,j=0,k=start;
    while(i<n1 && j<n2)
    {
        if(left[i]<=right[j])
        {
            arr[k]=left[i];
            i++;
        }
        else
        {
            arr[k]=right[j];
            j++;
        }
        k++;
    }

    while(i<n1)
    {
        arr[k]=left[i];
        i++;
        k++;
    }

    while(j<n2)
    {
        arr[k]=right[j];
        j++;
        k++;
    }
}

void merge_seq(vector <int> arr, int start, int end)
{
    if(start>=end)
    {
        return;
    }
    int mid=start+(end-start)/2;
    merge_seq(arr,start,mid);
    merge_seq(arr,mid+1,end);
    merge_sort_common(arr,start,end,mid);
}

void merge_parallel(vector <int> arr, int start, int end)
{
    if(start>=end)
    {
        return;
    }
    int mid=start+(end-start)/2;
    #pragma omp paralle sections
    {
        #pragma omp section
        {merge_seq(arr,start,mid);}

        #pragma omp section
        {merge_seq(arr,mid+1,end);}
    }
    merge_sort_common(arr,start,end,mid);
}

int main()
{
    cout<<"Enter number of elements: ";
    int n;
    cin>>n;

    vector <int> arr1(n),arr2(n),arr3(n),arr4(n);
    cout<<"Enter elements: ";
    for(int i=0;i<n;i++)
    {
        cin>>arr1[i];
        arr2[i]=arr3[i]=arr4[i]=arr1[i];
    }
    chrono::high_resolution_clock::time_point start, end;
    chrono::duration<double> diff;

    cout<<"Bubble sort sequential "<<endl;
    start=chrono::high_resolution_clock::now();
    bubble(arr1,n);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<diff.count()<<endl;

    cout<<"Bubble sort parallel"<<endl;
    start=chrono::high_resolution_clock::now();
    bubble_parallel(arr2,n);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<diff.count()<<endl;

    cout<<"Merge sort sequential"<<endl;
    start=chrono::high_resolution_clock::now();
    merge_seq(arr3,0,n-1);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<diff.count()<<endl;

    cout<<"Merge sort parallel"<<endl;
    start=chrono::high_resolution_clock::now();
    merge_parallel(arr4,0,n-1);
    end=chrono::high_resolution_clock::now();
    diff=end-start;
    cout<<diff.count()<<endl;
}