#ifndef VPTREE
#define VPTREE

#include <stdexcept>
#include <algorithm>
#include <deque>
#include <vector>
#include <queue>
#include "Rcpp.h"

/* Adapted from http://stevehanov.ca/blog/index.php?id=130 */

struct DataPoint {
    DataPoint();
    DataPoint(int, const double*);

    const double* ptr;
    int index;
};

class VpTree {
public:    
    VpTree(Rcpp::NumericMatrix); 
    VpTree(Rcpp::NumericMatrix, Rcpp::List);
    Rcpp::List save();

    void find_nearest_neighbors(size_t, int, const bool, const bool);
    void find_nearest_neighbors(const double*, int, const bool, const bool);

    int get_nobs() const;
    int get_ndims() const;

    std::deque<size_t>& get_neighbors ();
    std::deque<double>& get_distances ();
private:
    int ndim;
    Rcpp::NumericMatrix reference;
    std::vector<DataPoint> items;
    static const int LEAF_MARKER=-1;

    // Single node of a VP tree (has a point and radius; left children are closer to point than the radius)
    struct Node
    {
        double threshold;       // radius(?)
        int index;              // index of point in node
        int left;               // node: points closer by than threshold
        int right;              // node: points farther away than threshold
        Node(int i=0) : index(i), threshold(0.), left(LEAF_MARKER), right(LEAF_MARKER) {}
    };
    std::deque<Node> nodes;

    int buildFromPoints(int, int);
private:
    std::deque<size_t> neighbors;
    std::deque<double> distances;
    double tau;

    std::priority_queue<std::pair<double, int> > nearest;
    void search(int, const double*, int);

    bool diagnose_ties;
};

#endif
