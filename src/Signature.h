#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;

template <class IntegerType>
const IntegerType stringHash(const string& word)
{
    IntegerType hashVal= 0;
    unsigned l = word.length();
    for(unsigned i = 0; i < l; ++i)
        hashVal = (hashVal * 29) + (word[i] - 'a');
    return hashVal;
}

template <class IntegerType>
class Signature
{
public:
    void clear()
    {
        sigData.clear();
    }

    void sortSigs()
    {
        sort(sigData.begin(), sigData.end());
    }

    const unsigned size() const
    {
        return sigData.size();
    }

    void appendSigData(const string& word)
    {
        sigData.push_back(stringHash<IntegerType>(word));
    }

    const IntegerType operator[](unsigned pos) const
    {
        return sigData[pos];
    }

private:
    vector<IntegerType> sigData;
};

template <class IntegerType>
const double matchSignatures(const Signature<IntegerType>& A, const Signature<IntegerType>& B)
{
    IntegerType sigValue;
    unsigned posA, posB, totalSize, similarSize;

    posA = posB = similarSize = 0;
    totalSize = A.size() + B.size();

    while(posA < A.size() && posB < B.size())
    {
        // If a signature hash matches
        if(A[posA] == B[posB])
        {
            // Forward till match continues
            sigValue = A[posA];
            while(A[posA] == sigValue && posA < A.size())
            {
                ++posA;
                ++similarSize;
            }
            while(B[posB] == sigValue && posB < B.size())
            {
                ++posB;
                ++similarSize;
            }
        }
        else if(A[posA] < B[posB])
            posA++;
        else
            posB++;
    }

    double similarSizeD = similarSize / 2.0;
    return (similarSizeD / (totalSize - similarSizeD));
}

#endif // SIGNATURE_H
