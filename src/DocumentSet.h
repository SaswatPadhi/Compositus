#ifndef DOCUMENTSET_H
#define DOCUMENTSET_H

#include <set>
#include <cstdio>
#include "Document.h"
#include "Cluster.h"

using std::set;
using std::cout;

template <class IntegerType>
class DocumentSet
{
public:
    ~DocumentSet()
    {
        for(unsigned i = 0; i < allDocuments.size(); ++i)
            delete allDocuments[i];
        for(unsigned i = 0; i < allClusters.size(); ++i)
            delete allClusters[i];
    }

    void printMatchingClusters(string outputDestination) const
    {
        FILE* outputFile = stdout;
        if(outputDestination != "")
            outputFile = fopen(outputDestination.c_str(), "a");

        unsigned clusterNum = 0;
        set< Document<IntegerType>* > clusterDocs;
        set< clusterEdge<IntegerType> > clusterGraph;
        typename set< Document<IntegerType>* >::iterator clusterDocsIt;
        typename set< clusterEdge<IntegerType> >::iterator clusterGraphIt;

        for(unsigned i = 0; i < allClusters.size(); ++i)
        {
            if(!allClusters[i]->isPrintable())
                continue;

            clusterGraph = allClusters[i]->getEdges();
            clusterDocs = allClusters[i]->getDocuments();

            fprintf(outputFile, " [$] Cluster %u (V = %u, E = %u) :\n", ++clusterNum, clusterDocs.size(), clusterGraph.size());
            clusterDocsIt = clusterDocs.begin();
            fprintf(outputFile, "    [+] Documents = {%s", ((*clusterDocsIt)->getName()).c_str());
            for(unsigned i = 1; i < clusterDocs.size(); ++i)
            {
                ++clusterDocsIt;
                fprintf(outputFile, ", %s", ((*clusterDocsIt)->getName()).c_str());
            }
            fprintf(outputFile, "}\n");

            fprintf(outputFile, "    [+] Cluster statistics :\n");
            for(clusterGraphIt = clusterGraph.begin(); clusterGraphIt != clusterGraph.end(); ++clusterGraphIt)
                fprintf(outputFile, "       [.] %24s : %24s ==> %6.2f%%\n", ((*clusterGraphIt).firstDoc->getName()).c_str(), ((*clusterGraphIt).secondDoc->getName()).c_str(), 100.0*(*clusterGraphIt).matchedFraction);
            fprintf(outputFile, "\n");
        }

        if(outputDestination != "")
            fclose(outputFile);
    }

    void printComparisionMatrix() const
    {
        for(unsigned i = 0; i < weightedResults.size(); ++i)
        {
            for(unsigned j = 0; j < weightedResults.size(); ++j)
            {
                if(i == j)
                    printf(" [%02u] ", i);
                else if(i < j)
                {
                    if(weightedResults[j][i] > thresholdMatch)
                        printf("%5.2f ", 100 * weightedResults[j][i]);
                    else
                        printf("      ");
                }
                else
                {
                    if(weightedResults[i][j] > thresholdMatch)
                        printf("%5.2f ", 100 * weightedResults[i][j]);
                    else
                        printf("      ");
                }
            }
            putchar(10);
        }
    }

    const vector< vector<double> > getWeightedResults() const
    {
        return weightedResults;
    }


    DocumentSet(vector<string> docPaths, vector<unsigned> sigLens, double minMatch, double discardFraction)
    {
        thresholdMatch = minMatch;
        double totalWeight = 0.0;
        for(unsigned i = 0; i < sigLens.size(); ++i)
            totalWeight += (sigLens[i] * sigLens[i]);

        for(unsigned i = 0; i < sigLens.size(); ++i)
            sigWeights.push_back((sigLens[i] * sigLens[i]) / totalWeight);

        double weightedMatchFraction;
        const Signature<IntegerType>* docSignatures_A;
        const Signature<IntegerType>* docSignatures_B;
        vector<double> docWeightedMatchFraction;

        for(unsigned i = 0; i < docPaths.size(); ++i)
        {
            docWeightedMatchFraction.clear();

            // Prepare the ith document
            allDocuments.push_back(new Document<IntegerType>(docPaths[i], sigLens, discardFraction));
            docSignatures_A = allDocuments[i]->getSignatures();
            allClusters.push_back(new Cluster<IntegerType>(allDocuments[i]));

            // Compare ith doc with 0 .. i-1 th docs
            for(unsigned j = 0; j < i; ++j)
            {
                docSignatures_B = allDocuments[j]->getSignatures();

                // Match for each different signature length
                weightedMatchFraction = 0.0;
                for(unsigned k = 0; k < sigLens.size(); ++k)
                    weightedMatchFraction += sigWeights[k] * matchSignatures(docSignatures_A[k], docSignatures_B[k]);

                docWeightedMatchFraction.push_back(weightedMatchFraction);

                if(weightedMatchFraction > thresholdMatch)
                    allClusters[i]->importCluster(*(allClusters[j]), weightedMatchFraction);
            }

            weightedResults.push_back(docWeightedMatchFraction);
        }
    }

private:
    double thresholdMatch;
    vector<double> sigWeights;
    vector<Cluster<IntegerType>*> allClusters;
    vector<Document<IntegerType>*> allDocuments;
    vector< vector<double> > weightedResults;
};

#endif // DOCUMENTSET_H
