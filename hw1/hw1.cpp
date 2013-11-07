#include<iostream>
#include<vector>

using namespace std;

//return sum of integers in an array
//p - accumulator
//v - vector of values
inline void sum(int& p, vector<int> v)
{
    p = 0;
    for(vector<int>::iterator it = v.begin(); it != v.end(); ++it) //using iterators
        p += *it;
}

int main()
{
    const int SIZE = 40;
    int acc = 0;
    vector<int> data;
    data.reserve(SIZE);            //for perf increase

    for(int i = 0; i < SIZE; i++)
        data.push_back(i);         // add i to end of vector

    sum(acc, data);
    cout << "sum is " << acc << endl;
    return 0;

}
