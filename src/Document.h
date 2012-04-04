#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <fstream>
#include <cstdlib>
#include "Signature.h"

using std::ifstream;

string& cleanWord(string&);

template <class IntegerType>
class Document
{
public:
    Document(string docPath, vector<unsigned> n, double df)
    {
        unsigned minDiscard = df * RAND_MAX;

        vector<string>* allWords = new vector<string>;
        docSigs = new Signature<IntegerType> [n.size()];

        // Save the document path and word lengths
        wLen = n;
        path = docPath;
        name = path.substr(path.find_last_of("/")+1);

        // Store all valid words in allWords
        string myWord;
        ifstream docFile(path.c_str());
        docFile >> myWord;
        while(!docFile.eof())
        {
            myWord = cleanWord(myWord);
            if(myWord.length())
                allWords->push_back(myWord);
            docFile >> myWord;
        }
        docFile.close();

        // Construct the signatures
        string sigData;
        unsigned maxStartPos;
        for(unsigned i = 0; i < wLen.size(); ++i)
        {
            maxStartPos = allWords->size() - wLen[i];
            for(unsigned j = 0; j <= maxStartPos; ++j)
            {
                unsigned random = rand();
                if(random > minDiscard)
                {
                    sigData = "";
                    for(unsigned k = 0; k < wLen[i]; ++k)
                        sigData += (*allWords)[j+k];
                    docSigs[i].appendSigData(sigData);
                }
            }
        }

        // Sort the signatures hashes
        for(unsigned i = 0; i < wLen.size(); ++i)
            docSigs[i].sortSigs();

        delete allWords;
    }

    ~Document()
    {
        delete [] docSigs;
    }

    const string& getName() const
    {
        return name;
    }

    const Signature<IntegerType>* getSignatures() const
    {
        return (const Signature<IntegerType>*)docSigs;
    }

private:
    string path;
    string name;
    Signature<IntegerType>* docSigs;
    vector<unsigned> wLen;
};

#endif // DOCUMENT_H
