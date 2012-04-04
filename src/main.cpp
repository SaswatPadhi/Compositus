#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include "DocumentSet.h"

using namespace std;

enum hash_limits {SHORT, UNSIGNED, UNSIGNED_LONG, UNSIGNED_LL};

#define long_set DocumentSet<unsigned long>
#define short_set DocumentSet<unsigned short>
#define unsigned_set DocumentSet<unsigned int>
#define long_long_set DocumentSet<unsigned long long>

void badParameters()
{
    // Insufficient arguments. Abnormal exit.
    cout << "Compositus 1.43 for i386 compiled on " << __DATE__ << " @ " << __TIME__ << ".\n";
    cout << "Created by Saswat Padhi, Harshita Meena : Fall 2011.\n\n";

    cout << "Compositus is a simple command-line utility to match a set of plaintext documents and identify groups of similar documents.\n\n";

    cout << "Usage : Compositus [OPTIONS] FILE_1 FILE_2 ...\n\n";

    cout << "OPTIONS :\n";
    cout << "    -t0.xx\n";
    cout << "        The minimum matching fraction for the documents to be considered a positive match pair.\n";
    cout << "        For example, -t0.4 sets 0.4 to be threshold matching fraction for positive matches.\n";
    cout << "        Default value : -t0.1\n\n";
    cout << "    -n[a,b,...]\n";
    cout << "        The values of n for n-gram (n-word sequences) generation and comparision from plaintext files.\n";
    cout << "        The final matching fraction of 2 documents is the weighted average of all given values of n.\n";
    cout << "        For example, -n[4,7] generates all 4-grams and 7-grams and compares them.\n";
    cout << "        Default value : -n[3,5]\n\n";
    cout << "    -oOutputFile\n";
    cout << "        Sets the output stream.\n";
    cout << "        For example, -oMyOutput writes all output to MyOutput file in current folder.\n";
    cout << "        Default value : ~none~ | Output is written to the screen.\n\n";
    cout << "    -hn\n";
    cout << "        The maximum storage limit of hash values. n can vary from 0 to 3.\n";
    cout << "        Lower value of n => Lower storage size + Faster hash calculations + Greater inaccuracy.\n";
    cout << "        This option is useful, when you have an idea about of diversity the hash values (roughly related to file sizes).\n";
    cout << "        For example, -h0 sets the storage limit to the minimum.\n";
    cout << "        Default value : -h1\n\n";
    cout << "    -d0.xx\n";
    cout << "        Randomly discards the specified percentage of hash values to reduce comparisions.\n";
    cout << "        For example, -d0.2 discards 20% hash values from all files.\n";
    cout << "        Default value : -d0.00\n";

    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
        badParameters();

    // Default values
    double threshold_match = 0.1;
    vector<unsigned> wordSequences;
    wordSequences.push_back(3);
    wordSequences.push_back(5);
    hash_limits hash_limit = UNSIGNED;
    string OutputFile = "";
    double discard_fraction = 0.0;

    // Obtain all the filenames and options
    int filled, N;
    char *value, *values;
    string token;
    vector<string> documentPaths;
    while(argc-- > 1)
    {
        token = argv[argc];

        // Analyse token
        if(token[0] == '-')
        {
            // Small token
            switch(token[1])
            {
            case 't' : // -t0.xx
                if(token.length() < 5)
                    badParameters();
                filled = sscanf((token.substr(2)).c_str(), "%lf", &threshold_match);
                if(threshold_match > 0.99 || filled < 1)
                    badParameters();
                break;
            case 'n' : // -n[a,b,..]
                if(token[2] != '[' || *(token.rbegin()) != ']')
                    badParameters();
                wordSequences.clear();
                values = (char*) token.substr(3, token.length()-4).c_str();
                value = strtok(values, ",");
                while(value != NULL)
                {
                    filled = sscanf(value, "%d", &N);
                    if(filled < 1)
                        badParameters();
                    wordSequences.push_back(N);
                    value = strtok(NULL, ",");
                }
                if(!wordSequences.size())
                    badParameters();
                break;
            case 'h' :
                filled = sscanf((token.substr(2)).c_str(), "%d" , &hash_limit);
                if(hash_limit > 3 || filled < 1)
                    badParameters();
                break;
            case 'd' : // -d0.xx
                filled = sscanf((token.substr(2)).c_str(), "%lf", &discard_fraction);
                if(discard_fraction > 0.99 || filled < 1)
                    badParameters();
                break;
            case 'o' : //-oOutputFile
                if(token.length() < 3)
                    badParameters();
                OutputFile = token.substr(2);
                break;
            }
        }
        else
            documentPaths.push_back(token);
    }

    ofstream file;
    if(OutputFile.length())
    {
        file.open(OutputFile.c_str(), ios::out);
        cout.rdbuf(file.rdbuf());
    }

    cout << "Compositus 1.43 for i386 compiled on " << __DATE__ << " @ " << __TIME__ << ".\n";
    cout << "Created by Saswat Padhi, Harshita Meena : Fall 2011.\n\n";

    cout << "[#] Comparing " << documentPaths.size() << " documents ...\n\n[$] Possible plagiarist clusters found :\n";

    if(OutputFile.length())
        file.close();

    if(hash_limit == SHORT)
    {
        short_set* filteredDocuments = new short_set (documentPaths, wordSequences, threshold_match, discard_fraction);
        filteredDocuments->printMatchingClusters(OutputFile);
        delete filteredDocuments;
    }
    else if(hash_limit == UNSIGNED)
    {
        unsigned_set* filteredDocuments = new unsigned_set (documentPaths, wordSequences, threshold_match, discard_fraction);
        filteredDocuments->printMatchingClusters(OutputFile);
        delete filteredDocuments;
    }
    else if(hash_limit == UNSIGNED_LONG)
    {
        long_set* filteredDocuments = new long_set (documentPaths, wordSequences, threshold_match, discard_fraction);
        filteredDocuments->printMatchingClusters(OutputFile);
        delete filteredDocuments;
    }
    else
    {
        long_long_set* filteredDocuments = new long_long_set (documentPaths, wordSequences, threshold_match, discard_fraction);
        filteredDocuments->printMatchingClusters(OutputFile);
        delete filteredDocuments;
    }

    return EXIT_SUCCESS;
}
