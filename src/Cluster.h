#ifndef CLUSTER_H
#define CLUSTER_H

#include <set>
#include <iostream>
#include <algorithm>
#include "Document.h"

using std::set;
using std::set_union;
using std::inserter;

template <class IntegerType>
struct clusterEdge
{
    const Document<IntegerType>* firstDoc;
    const Document<IntegerType>* secondDoc;
    const double matchedFraction;
    clusterEdge(const Document<IntegerType>* F,const Document<IntegerType>* S,const double M) : firstDoc(F), secondDoc(S), matchedFraction(M) {}
};

template <class IntegerType>
bool operator< (clusterEdge<IntegerType> A, clusterEdge<IntegerType> B)
{
    if(A.firstDoc < B.firstDoc)
        return true;
    else if(A.secondDoc < B.secondDoc)
        return true;
    else if(A.matchedFraction < B.matchedFraction)
        return true;

    return false;
}

template <class IntegerType>
class Cluster
{
public:
    Cluster(Document<IntegerType>* own)
    {
        ownDocument = own;
        printable = false;
        documentsCluster.insert(ownDocument);
    }

    const bool isPrintable() const
    {
        return printable;
    }

    const set< clusterEdge<IntegerType> >& getEdges() const
    {
        return clusterGraph;
    }

    const set< Document<IntegerType>* >& getDocuments() const
    {
        return documentsCluster;
    }

    void importCluster(Cluster& C, const double M)
    {
        printable = true;
        C.printable = false;

        clusterGraph.insert(clusterEdge<IntegerType>(ownDocument, C.ownDocument, M));

        set< Document<IntegerType>* > result;
        set_union(documentsCluster.begin(), documentsCluster.end(), C.documentsCluster.begin(), C.documentsCluster.end(), inserter(result, result.end()));
        documentsCluster = result;

        set< clusterEdge<IntegerType> > resultGraph;
        set_union(clusterGraph.begin(), clusterGraph.end(), C.clusterGraph.begin(), C.clusterGraph.end(), inserter(resultGraph, resultGraph.end()));
        clusterGraph = resultGraph;
    }

private:
    bool printable;
    Document<IntegerType>* ownDocument;
    set< clusterEdge<IntegerType> > clusterGraph;
    set< Document<IntegerType>* > documentsCluster;
};

#endif // CLUSTER_H
